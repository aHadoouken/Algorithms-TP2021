//Напишите программу, которая использует кучу для слияния K отсортированных массивов суммарной длиной N.
//
//Требования:
//Время работы O(N * logK)
//Куча должна быть реализована в виде шаблонного класса.
//Решение должно поддерживать передачу функции сравнения снаружи.
//Куча должна быть динамической.
#include <iostream>
#include <cassert>
#include <fstream>

#define INIT_BUFF_SIZE 5

template<typename T>
class Array {
public:
    Array() : actual_size(0), total_size(0), arr(nullptr) {
    }

    explicit Array(size_t size);

    explicit Array(T val, size_t size = INIT_BUFF_SIZE);

    Array(const Array<T> &original);

    ~Array() { delete[]arr; };

    T &operator[](const int &i);

    T operator[](const int &i) const;

    Array<T> &operator=(const Array<T> &r_arr);

    void swap(const int &i, const int &j);

    void append(const T &val);

    T pop();

    size_t size() const { return actual_size; }

    void printArr() {
        for (int i = 0; i < this->size(); i++) {
            std::cout << arr[i] << ' ';
        }
        std::cout << std::endl;
    }

private:
    T *arr;
    size_t total_size;
    size_t actual_size;
    size_t grow_factor = 2;

    void resize();
};

template<typename T, typename Comparator = std::less<T>>
class Heap {
public:
    Heap() {};

    explicit Heap(const Array<T> &original) : arr(original) { buildHeap(); };

    ~Heap() {};

    void Insert(const T &val);

    T ExtractMax();

    T PeakMax() const { return arr[0]; }

    size_t size() const { return arr.size(); }

    void printArr() {
        for (int i = 0; i < arr.size(); i++) {
            std::cout << arr[i] << ' ';
        }
        std::cout << std::endl;
    }

private:
    Comparator cmp;
    Array<T> arr;

    void buildHeap();

    void siftDown(int i);

    void siftUp(int i);
};

template<typename T>
struct ArrayWrapper {
    ArrayWrapper() : arr(), size(0), curr_index(0) {};

    explicit ArrayWrapper(int size) : arr(size), size(size), curr_index(0) {};
    Array<T> arr;
    size_t size;
    int curr_index;

    ArrayWrapper<T> &operator=(const ArrayWrapper<T> &r_arr) {
        if (this == &r_arr) {
            return *this;
        }
        arr = r_arr.arr;
        size = r_arr.size;
        curr_index = r_arr.curr_index;
        return *this;
    }
};

template<typename T>
class ArrayWrapperComparator {
public:
    bool operator()(const ArrayWrapper<T> &l, const ArrayWrapper<T> &r) const {
        return l.arr[l.curr_index] > r.arr[r.curr_index];
    }
};

int main() {
    std::ifstream infile("../input4_5.txt");
    size_t n_arrays;
    size_t total_elems = 0;
    infile >> n_arrays;
    Array<ArrayWrapper<double>> arrays(n_arrays);
    int val;

    for (int i = 0; i < n_arrays; i++) {
        infile >> arrays[i].size;
        total_elems += arrays[i].size;
        for (int j = 0; j < arrays[i].size; j++) {
            infile >> val;
            arrays[i].arr.append(val);
        }
    }

    Heap<ArrayWrapper<double>, ArrayWrapperComparator<double>> heap(arrays);
    ArrayWrapper<double> tmp;

    for (int i = 0; i < total_elems; i++) {
        tmp = heap.ExtractMax();
        std::cout << tmp.arr[tmp.curr_index] << ' ';
        tmp.curr_index++;
        if (tmp.curr_index != tmp.size) {
            heap.Insert(tmp);
        }
    }

    return 0;
};

template<typename T, typename Comparator>
void Heap<T, Comparator>::Insert(const T &val) {
    arr.append(val);
    siftUp(arr.size() - 1);
}

template<typename T, typename Comparator>
T Heap<T, Comparator>::ExtractMax() {
    arr.swap(0, arr.size() - 1);
    T return_val = arr.pop();
    siftDown(0);
    return return_val;
}

template<typename T, typename Comparator>
void Heap<T, Comparator>::buildHeap() {
    for (int i = arr.size() / 2 - 1; i >= 0; i--) {
        siftDown(i);
    }
}

template<typename T, typename Comparator>
void Heap<T, Comparator>::siftDown(int i) {
    int left, right, largest;
    while (true) {
        left = 2 * i + 1;
        right = 2 * i + 2;
        largest = i;
        if (left < arr.size() && cmp(arr[largest], arr[left])) {
            largest = left;
        }
        if (right < arr.size() && cmp(arr[largest], arr[right])) {
            largest = right;
        }
        if (largest == i) {
            break;
        }
        arr.swap(i, largest);
        i = largest;
    }
}

template<typename T, typename Comparator>
void Heap<T, Comparator>::siftUp(int i) {
    int parent;
    do {
        parent = (i - 1) / 2;
        if (cmp(arr[parent], arr[i])) {
            arr.swap(parent, i);
            i = parent;
        } else { break; }
    } while (parent);
}

template<typename T>
Array<T>::Array(T val, size_t size) {
    assert(size > 0);
    actual_size = size;
    total_size = size;
    arr = new T[size];
    for (int i = 0; i < size; i++) {
        arr[i] = val;
    }
}

template<typename T>
Array<T>::Array(size_t size) {
    assert(size > 0);
    actual_size = size;
    total_size = size;
    arr = new T[size];
}

template<typename T>
Array<T>::Array(const Array<T> &original) {
    actual_size = original.size();
    total_size = original.size();
    arr = new T[total_size];
    for (int i = 0; i < total_size; i++) {
        arr[i] = original[i];
    }
}

template<typename T>
T &Array<T>::operator[](const int &i) {
    assert(i >= 0 && i < actual_size);
    return arr[i];
}

template<typename T>
T Array<T>::operator[](const int &i) const {
    assert(i >= 0 && i < actual_size);
    return arr[i];
}

template<typename T>
Array<T> &Array<T>::operator=(const Array<T> &r_arr) {
    if (this == &r_arr) {
        return *this;
    }
    total_size = r_arr.total_size;
    actual_size = r_arr.actual_size;
    delete[]arr;
    T *new_arr = new T[total_size];
    for (int i = 0; i < actual_size; i++) {
        new_arr[i] = r_arr[i];
    }
    arr = new_arr;
    return *this;
}

template<typename T>
void Array<T>::swap(const int &i, const int &j) {
    T temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

template<typename T>
void Array<T>::append(const T &val) {
    if (total_size == actual_size) {
        resize();
    }
    arr[actual_size] = val;
    actual_size++;
}

template<typename T>
T Array<T>::pop() {
    actual_size--;
    return arr[actual_size];
}

template<typename T>
void Array<T>::resize() {
    if (!total_size) {
        total_size = INIT_BUFF_SIZE;
        arr = new T[total_size];
        return;
    }
    total_size = total_size * grow_factor;
    T *new_arr = new T[total_size];
    for (int i = 0; i < actual_size; i++) {
        new_arr[i] = arr[i];
    }
    delete[]arr;
    arr = new_arr;
}
