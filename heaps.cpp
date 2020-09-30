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

template<class keytype,class valuetype>
struct node {
  keytype key;
  valuetype val;
  int degree;
  node *child;
  node *sibling;
  node *parent;
};

template<class keytype,class valuetype>
node<keytype,valuetype> *newNode(keytype k, valuetype v) {
  node<keytype,valuetype> *a = new node<keytype,valuetype>;
  a->key = k;
  a->val = v;
  a->degree = 0;
  a->child = NULL;
  a->sibling = NULL;
  a->parent = NULL;
  return a;
}

template<class keytype,class valuetype>
node<keytype,valuetype> *mergeTree(node<keytype,valuetype> *a, node<keytype,valuetype> *b) {
  if (a->key < b->key) {
    b->parent = a;
    b->sibling = a->child;
    a->child = b;
    a->degree++;
    return a;
  }
  else {
    a->parent = b;
    a->sibling = b->child;
    b->child = a;
    b->degree++;
    return b;
  }
}

template<class keytype,class valuetype>
void preorder(node<keytype,valuetype> *a) {
  if (a == NULL)
    return;
  cout << a->key << " ";
  preorder(a->child);
  preorder(a->sibling);
}

template<class keytype,class valuetype>
void deleteTree(node<keytype,valuetype> *a) {
  if (a == NULL)
    return;
  deleteTree(a->child);
  deleteTree(a->sibling);
  delete a;
  return;
}

template<class keytype,class valuetype>
node<keytype,valuetype>* copyTree(node<keytype,valuetype> *a) {
  if (a == NULL)
    return NULL;
  node<keytype,valuetype> *r = newNode(a->key, a->val);
  r->degree = a->degree;
  r->child = copyTree(a->child);
  r->sibling = copyTree(a->sibling);
  return r;
}

template<class keytype,class valuetype>
class BHeap {
  private:
    CircularDynamicArray<node<keytype,valuetype>*> BinHeap;

  public:
    BHeap() {
      BinHeap = CircularDynamicArray<node<keytype,valuetype>*> ();
    }

    BHeap(keytype k[], valuetype v[], int s) {
      BinHeap = CircularDynamicArray<node<keytype,valuetype>*> ();
      for (int i = 0; i < s; i++) {
        insert(k[i], v[i]);
      }
    }

    BHeap(CircularDynamicArray<node<keytype,valuetype>*> a) {
      BinHeap = a;
    }

    ~BHeap() {
    }

    BHeap(BHeap &other) {
      for (int i = 0; i < other.BinHeap.length(); i++) {
        BinHeap.addEnd(copyTree(other.BinHeap[i]));
      }
		}

    BHeap& operator=(BHeap& other) {
			if (this != &other) {
        for (int i = 0; i < other.BinHeap.length(); i++) {
          BinHeap.addEnd(copyTree(other.BinHeap[i]));
        }
			}
			return *this;
    }

    void insert(keytype k, valuetype v) {
      bool inserted = false;
      node<keytype,valuetype> *a = newNode(k, v);
      for (int i = 0; i < BinHeap.length(); i++) {
        if (BinHeap[i] == NULL) {
          BinHeap[i] = a;
          inserted = true;
          break;
        }
        else {
          a = mergeTree(a, BinHeap[i]);
          BinHeap[i] = NULL;
        }
      }
      if (!inserted)
        BinHeap.addEnd(a);
    }

    keytype peakKey() {
      bool minStarted = false;
      keytype minn;
      for (int i = 0; i < BinHeap.length(); i++) {
        if (BinHeap[i] == NULL)
          continue;
        if (!minStarted)
          minn = BinHeap[i]->key;
        else if (BinHeap[i]->key < minn)
          minn = BinHeap[i]->key;
      }
      return minn;
    }

    valuetype peakValue() {
      bool minStarted = false;
      keytype minn;
      valuetype minnVal;
      for (int i = 0; i < BinHeap.length(); i++) {
        if (BinHeap[i] == NULL)
          continue;
        if (!minStarted) {
          minn = BinHeap[i]->key;
          minnVal = BinHeap[i]->val;
        }
        else if (BinHeap[i]->key < minn) {
          minn = BinHeap[i]->key;
          minnVal = BinHeap[i]->val;
        }
      }
      return minnVal;
    }

    void merge(BHeap H2) {
      node<keytype,valuetype> *carry = NULL;
      for (int i = 0; i < H2.BinHeap.length(); i++) {
        if (i >= this->BinHeap.length()) {
          if (carry == NULL && H2.BinHeap[i] == NULL)
            this->BinHeap.addEnd(NULL);
          else if (carry == NULL)
            this->BinHeap.addEnd(H2.BinHeap[i]);
          else if (H2.BinHeap[i] == NULL) {
            this->BinHeap.addEnd(carry);
            carry = NULL;
          }
          else {
            carry = mergeTree(carry, H2.BinHeap[i]);
          }
        }
        if (H2.BinHeap[i] == NULL && this->BinHeap[i] == NULL && carry == NULL)
          continue;
        else if (H2.BinHeap[i] == NULL && this->BinHeap[i] == NULL) {
          this->BinHeap[i] = carry;
          carry = NULL;
        }
        else if (H2.BinHeap[i] == NULL && carry == NULL)
          continue;
        else if (this->BinHeap[i] == NULL && carry == NULL)
          this->BinHeap[i] = H2.BinHeap[i];
        else if (carry == NULL)  {
            carry = mergeTree(this->BinHeap[i], H2.BinHeap[i]);
            this->BinHeap[i] = NULL;
        }
        else if (this->BinHeap[i] == NULL) {
          carry = mergeTree(carry, H2.BinHeap[i]);
        }
        else if (H2.BinHeap[i] == NULL) {
          carry = mergeTree(carry, this->BinHeap[i]);
          this->BinHeap[i] = NULL;
        }
        else {
          carry = mergeTree(carry, H2.BinHeap[i]);
        }
      }
      if (carry != NULL && H2.BinHeap.length() >= this->BinHeap.length()) {
        this->BinHeap.addEnd(carry);
        carry = NULL;
      }
      else if (carry != NULL) {
        for (int i = H2.BinHeap.length(); i < this->BinHeap.length(); i++) {
          if (this->BinHeap[i] == NULL) {
            this->BinHeap[i] = carry;
            carry = NULL;
            break;
          }
          else {
            carry = mergeTree(carry, this->BinHeap[i]);
            this->BinHeap[i] = NULL;
          }
        }
        if (carry != NULL)
          this->BinHeap.addEnd(carry);
      }
    }

    keytype extractMin() {
      bool minStarted = false;
      keytype minn;
      int minnIndex = 0;
      for (int i = 0; i < BinHeap.length(); i++) {
        if (BinHeap[i] == NULL)
          continue;
        if (!minStarted) {
          minn = BinHeap[i]->key;
          minnIndex = i;
        }
        else if (BinHeap[i]->key < minn) {
          minn = BinHeap[i]->key;
          minnIndex = i;
        }
      }
      if (minnIndex != 0) {
        CircularDynamicArray<node<keytype,valuetype>*> tempCDA(BinHeap[minnIndex]->degree);
        node<keytype,valuetype> *start = BinHeap[minnIndex]->child;
        tempCDA[BinHeap[minnIndex]->degree - 1] = start;
        int k = BinHeap[minnIndex]->degree - 2;
        while (start->sibling != NULL) {
          start = start->sibling;
          tempCDA[k] = start;
          k--;
        }
        for (int i = 0; i < tempCDA.length(); i++) {
          tempCDA[i]->sibling = NULL;
        }
        BinHeap[minnIndex] = NULL;
        BHeap temp(tempCDA);
        merge(temp);
      }
      else
        BinHeap[minnIndex] = NULL;
      return minn;
    }

    void printKey() {
      for (int i = 0; i < BinHeap.length(); i++) {
        if (BinHeap[i] == NULL)
          continue;
        cout << "B" << BinHeap[i]->degree << endl;
        preorder(BinHeap[i]);
        cout << endl << endl;
      }
    }
};
