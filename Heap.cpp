#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <stdlib.h>
#include "CircularDynamicArray.cpp"
using namespace std;

template<class keytype,class valuetype>
struct entry {
  keytype key;
  valuetype val;
};

template<class keytype,class valuetype>
void swap(entry<keytype,valuetype> &a, entry<keytype,valuetype> &b) {
  entry<keytype,valuetype> temp = a;
  a = b;
  b = temp;
}

template<class keytype,class valuetype>
void heapify(entry<keytype,valuetype> arr[], int n, int i) {
  int min = i;
  int l = 2*i + 1;
  int r = 2*i + 2;
  if (l < n && arr[l].key < arr[min].key)
    min = l;
  if (r < n && arr[r].key < arr[min].key)
    min = r;
  if (min != i) {
    swap(arr[i], arr[min]);
    heapify(arr, n, min);
  }
}

template<class keytype,class valuetype>
class Heap {

  private:
    CircularDynamicArray<entry<keytype,valuetype>> minHeap;

  public:
    Heap() {
      minHeap = CircularDynamicArray<entry<keytype,valuetype>> ();
    }

    Heap(keytype k[], valuetype v[], int s) {
      entry<keytype,valuetype> *list = new entry<keytype,valuetype>[s];
      for (int i = 0; i < s; i++) {
        entry<keytype,valuetype> *a = new entry<keytype,valuetype>;
        a->key = k[i];
        a->val = v[i];
        list[i] = *a;
      }
      for (int i = (s/2) - 1; i >= 0; i--) {
        heapify(list, s, i);
      }
      minHeap = CircularDynamicArray<entry<keytype,valuetype>> ();
      for (int i = 0; i < s; i++) {
        minHeap.addEnd(list[i]);
      }
    }

    ~Heap() {
    }

    keytype peakKey() {
      return minHeap[0].key;
    }

    valuetype peakValue() {
      return minHeap[0].val;
    }

    keytype extractMin() {
      keytype minSave = minHeap[0].key;
      swap(minHeap[0], minHeap[minHeap.length() - 1]);
      minHeap.delEnd();
      int i = 0;
      int l = 2*i + 1;
      int r = 2*i + 2;
      while (l < minHeap.length()) {
        if (minHeap[l].key < minHeap[i].key && ((r < minHeap.length() && minHeap[l].key < minHeap[r].key) || (r == minHeap.length()))) {
          swap(minHeap[i], minHeap[l]);
          i = l;
          l = 2*i + 1;
          r = 2*i + 2;
        }
        else if (r < minHeap.length() && minHeap[r].key < minHeap[i].key && minHeap[r].key < minHeap[l].key) {
          swap(minHeap[i], minHeap[r]);
          i = r;
          l = 2*i + 1;
          r = 2*i + 2;
        }
        else
          break;
      }
      return minSave;
    }

    void insert(keytype k, valuetype v) {
      entry<keytype,valuetype> *a = new entry<keytype,valuetype>;
      a->key = k;
      a->val = v;
      minHeap.addEnd(*a);
      int i = minHeap.length() - 1;
      int parent = (i-1)/2;
      while (i != 0) {
        if (minHeap[i].key < minHeap[parent].key) {
          swap(minHeap[i], minHeap[parent]);
          i = parent;
          parent = (i-1)/2;
        }
        else
          break;
      }
    }

    void printKey() {
      for (int i = 0; i < minHeap.length(); i++)
        cout << minHeap[i].key << " ";
      cout << endl;
    }
};
