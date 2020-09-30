#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <stdlib.h>
using namespace std;

template <class T>
class CircularDynamicArray {
	private:
		int size;
		int cap;
		int head;
		T *list;
		T error;

	public:
		CircularDynamicArray() {
			size = 0;
			cap = 2;
			head = 0;
			list = new T[cap];
		}

		CircularDynamicArray(int s) {
			if (s < 1) {
				cout << "Size must be 1 or greater" << endl;
			}
			else {
				size = s;
				cap = s;
				head = 0;
				list =  new T[cap];
			}
		}

		CircularDynamicArray(const CircularDynamicArray &other) {
			size = other.size;
			cap = other.cap;
			head = other.head;
			T* newList = new T[cap];
			for (int i = 0; i < size; i++) {
				newList[i] = other.list[realIndex(i)];
			}
			head = 0;
			list = newList;
		}

		CircularDynamicArray& operator=(const CircularDynamicArray& other) {
			if (this != &other) {
				size = other.size;
				cap = other.cap;
				head = other.head;
				T* newList = new T[cap];
				for (int i = 0; i < size; i++) {
					newList[i] = other.list[realIndex(i)];
				}
				head = 0;
				list = newList;
			}
			return *this;
		}

		~CircularDynamicArray() {
			delete[] list;
		}

		T& operator[](int i) {
			if (i < 0 || i >= size) {
				cout << "Out of bounds reference: " << i << endl;
				return error;
			}
			else {
				int index = (head+i)%cap;
				return list[index];
			}
		}

		int realIndex(int a) {
			return ((head+a)%cap);
		}

		void swap(T *a, T *b) {
	    T temp = *a;
	    *a = *b;
	    *b = temp;
		}

		void printArray(){
			string a = "[";
			for (int i = 0; i < size; i++) {
				a = a + to_string(list[i]);
				if (i != size - 1) {
					a = a + ",";
				}
			}
			a = a + "]";
			cout << a;
		}

		void addEnd(T v) {
			if (size == cap) {
				T *newList = new T[2*cap];
				int j = head;
				for (int i = 0; i < size; i++) {
					newList[i] = list[j];
					j = (j+1)%cap;
				}
				newList[size] = v;
				cap = cap * 2;
				delete[] list;
				list = newList;
				head = 0;
			}
			else {
			list[(head+size)%cap] = v;
			}
			size++;
		}

		void addFront(T v) {
			if (size == cap) {
				T *newList = new T[2*cap];
				int j = head;
				for (int i = 0; i < size; i++) {
					newList[i+1] = list[j];
					j = (j+1)%cap;
				}
				newList[0] = v;
				cap = cap * 2;
				delete[] list;
				list = newList;
				head = 0;
			}
			else {
				if (head - 1 >= 0) {
					head = head - 1;
				}
				else {
					head = cap - 1;
				}
				list[head] = v;
			}
			size++;
		}

		int capacity() {
			return cap;
		}

		int length() {
			return size;
		}

		void delEnd() {
			size--;
			if (size <= cap/4 && cap > 2) {
				T *newList = new T[cap/2];
				int j = head;
				for (int i = 0; i < size; i++) {
					newList[i] = list[j];
					j = (j+1)%cap;
				}
				cap = cap / 2;
				delete[] list;
				list = newList;
				head = 0;
			}
		}

		void delFront() {
			head = (head+1)%cap;
			size--;
			if (size <= cap/4 && cap > 2) {
				T *newList = new T[cap/2];
				int j = head;
				for (int i = 0; i < size; i++) {
					newList[i] = list[j];
					j = (j+1)%cap;
				}
				cap = cap / 2;
				delete[] list;
				list = newList;
				head = 0;
			}
		}

		void clear() {
			delete[] list;
			size = 0;
			cap = 2;
			head = 0;
			list = new T[cap];
		}

		int partition(T arr[], int l, int r) {
			T x = arr[realIndex(r)], i = l;
		    for (int j = l; j <= r - 1; j++) {
		        if (arr[realIndex(j)] <= x) {
		            T temp = arr[realIndex(i)];
								arr[realIndex(i)] = arr[realIndex(j)];
								arr[realIndex(j)] = temp;
		            i++;
		        }
		    }
				T temp = arr[realIndex(i)];
				arr[realIndex(i)] = arr[realIndex(r)];
				arr[realIndex(r)] = temp;
		    return i;
		}

		T kthSmallest(T arr[], int l, int r, int k) {
		    if (k > 0 && k <= r - l + 1) {
		        int index = partition(arr, l, r);
		        if (index - l == k - 1)
		            return arr[realIndex(index)];
		        if (index - l > k - 1)
		            return kthSmallest(arr, l, index - 1, k);
		        return kthSmallest(arr, index + 1, r, k - index + l - 1);
		    }
				else {
					cout << "Invalid k-term for QuickSelect";
				}
		    return error;
		}

		T QuickSelect(int k) {
			return (kthSmallest(list, 0, size - 1, k));
			}

    void merge(T arr[], int l, int m, int r) {
	    int i, j, k;
	    int n1 = m - l + 1;
	    int n2 =  r - m;
	    T L[n1], R[n2];
	    for (i = 0; i < n1; i++)
	        L[i] = arr[realIndex(l + i)];
	    for (j = 0; j < n2; j++)
	        R[j] = arr[realIndex(m + 1+ j)];
	    i = 0;
	    j = 0;
	    k = l;
	    while (i < n1 && j < n2)
	    {
	        if (L[i] <= R[j])
	        {
	            arr[realIndex(k)] = L[i];
	            i++;
	        }
	        else
	        {
	            arr[realIndex(k)] = R[j];
	            j++;
	        }
	        k++;
	    }
	    while (i < n1)
	    {
	        arr[realIndex(k)] = L[i];
	        i++;
	        k++;
	    }
	    while (j < n2)
	    {
	        arr[realIndex(k)] = R[j];
	        j++;
	        k++;
	    }
	}

		void mergeSort(T arr[], int l, int r) {
	    if (l < r) {
	        int m = l+(r-l)/2;
	        mergeSort(arr, l, m);
	        mergeSort(arr, m+1, r);
	        merge(arr, l, m, r);
	    }
		}

		void stableSort() {
			mergeSort(list, 0, size - 1);
		}

		T findMedian(T arr[], int n) {
			int minInd = 0;
			for (int i = 0; i < n-1; i++) {
				minInd = i;
				for (int j = i+1; j < n; j++) {
					if (arr[j] < arr[minInd])
						minInd = j;
				}
				T temp = arr[minInd];
				arr[minInd] = arr[i];
				arr[i] = temp;
			}
			return arr[n/2];
		}

		T kthSmallestWC(T arr[], int l, int r, int k) {
			if (k > 0 && k <= r - l + 1) {
				int n = r-l+1;
        int i;
				T median[(n+4)/5];
        for (i=0; i<n/5; i++)
            median[i] = findMedian(arr+l+i*5, 5);
        if (i*5 < n)
        {
            median[i] = findMedian(arr+l+i*5, n%5);
            i++;
        }
        int medOfMed = (i == 1)? median[i-1]:
                                 kthSmallestWC(median, 0, i-1, i/2);
        int pos = partitionWC(arr, l, r, medOfMed);
        if (pos-l == k-1)
            return arr[pos];
        if (pos-l > k-1)
            return kthSmallestWC(arr, l, pos-1, k);
        return kthSmallestWC(arr, pos+1, r, k-pos+l-1);
    	}
			else {
				cout << "Invalid k-term for WCSelect";
			}
			return error;
		}

		int partitionWC(T arr[], int l, int r, T x) {
	    int i;
	    for (i=l; i<r; i++)
	        if (arr[i] == x)
	           break;
	    swap(&arr[i], &arr[r]);
	    i = l;
	    for (int j = l; j <= r - 1; j++)
	    {
	        if (arr[j] <= x)
	        {
	            swap(&arr[i], &arr[j]);
	            i++;
	        }
	    }
	    swap(&arr[i], &arr[r]);
	    return i;
		}

		T WCSelect(int k) {
			T *temp = new T[size];
			for (int i = 0; i < size; i++) {
				temp[i] = list[realIndex(i)];
			}
			return kthSmallestWC(temp, 0, size - 1, k);
		}

		int linearSearch(T e) {
			for (int i = head; i < size+head; i++) {
				if (list[i%cap] == e) {
					return (i-head);
				}
			}
			return -1;
		}

		int binSearchRecur(int arr[], int l, int r, T x) {
			if (r >= l) {
        int mid = l + (r - l) / 2;
        if (arr[realIndex(mid)] == x)
            return mid;
        if (arr[realIndex(mid)] > x)
            return binSearchRecur(arr, l, mid - 1, x);
        return binSearchRecur(arr, mid + 1, r, x);
			}
			return -1;
		}

		int binSearch(T e) {
			int min=0;
			int max=this->size-1;
			int mid=(max-min)/2;
			for (;e!=this->list[realIndex(mid)] && max > min;) {
				if (e < this->list[realIndex(mid)]) {
					max = mid-1;
					mid = (max+min)/2;
				}
				else {
					min = mid+1;
					mid = (max+min)/2;
				}
			}
			if (e == this->list[realIndex(mid)])
				return mid;
			else return -1;
		}

		void radixSort(int bits) {
			int base=pow(2, bits);
			int comp=base-1;
			bool bool1=1;
			bool1=countingSort(base,comp, 0);
		}

		bool countingSort(int base, int comp, int exp) {
			bool bool1=0;
			int countArr[base];
			int indexArr[base];
			int shift=pow(base, exp);

			for (int i=0;i<base;i++) countArr[i]=0;

			for (int i=0;i<size;i++) {
				countArr[((list[realIndex(i)])/shift)&comp]++;
				if (bool1 || list[realIndex(i)]/shift != 0) bool1=1;
			}

			indexArr[base-1]=size-countArr[base-1];
			for (int i=base-2;i>=0;i--) {
				indexArr[i]=indexArr[i+1]-countArr[i];
			}

			T sorted[cap];
			for (int i=0;i<size;i++) {
				sorted[indexArr[((list[realIndex(i)])/shift)&comp]]=list[realIndex(i)];
				indexArr[((list[realIndex(i)])/shift)&comp]++;
			}

			for (int i=0;i<size;i++) {
				list[realIndex(i)]=sorted[i];
			}
			return bool1;
		}

		T* getList() {
			return list;
		}

		int getHead() {
			return head;
		}

		void setList(T* a) {
			list = a;
		}

		void setCap(int a) {
			cap = a;
		}

		void setSize(int a) {
			size = a;
		}

		void setHead(int a) {
			head = a;
		}
	};
