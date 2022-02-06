#include <iostream>
#include <cassert>

#define BUFF_SIZE 256
#define INIT_BUFF_SIZE 10

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

    void clean() { actual_size = 0; };

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

void MSD(Array<Array<char>> &strings, int d, int l, int r) {
    if (r - l < 2) {
        return;
    }
    auto *countBuf = new int[BUFF_SIZE]();
    for (int i = l; i < r; i++) {
        if (d < strings[i].size()) {
            countBuf[strings[i][d]]++;
        } else {
            strings.swap(l, i);
            l++;
        }
    }
    if (r - l < 2) {
        delete[] countBuf;
        return;
    }
    for (int i = 1; i < BUFF_SIZE; i++) {
        countBuf[i] += countBuf[i - 1];
    }
    Array<Array<char>> tmpBuf(r - l);

    for (int i = r - 1; i >= l; i--) {
        char c = strings[i][d];
        countBuf[c]--;
        tmpBuf[countBuf[c]] = strings[i];
    }
    for (int i = l; i < r; i++) {
        strings[i] = tmpBuf[i - l];
    }
    for (int i = 0; i < BUFF_SIZE - 1; i++) {
        MSD(strings, d + 1, l + countBuf[i], l + countBuf[i + 1]);
    }
    MSD(strings, d + 1, l + countBuf[BUFF_SIZE - 1], r);

    delete[] countBuf;
}

int readlines(FILE *fp, Array<Array<char>> &strings) {
    Array<char> tmp;
    int str_num = 0;
    for (char c; (c = fgetc(fp)) != EOF;) {
        tmp.append(c);
        if (c == '\0' || c == '\n') {
            tmp.pop();
            strings.append(tmp);
            tmp.clean();
            str_num++;
        }
    }
    return str_num;
}

int main(int argc, const char *argv[]) {
    FILE *fp = fopen("../input7_1.txt", "r");
    char *line[20];
    size_t read;
    size_t len;
    Array<Array<char>> strings;
    int n_lines = readlines(fp, strings);
    fclose(fp);

    MSD(strings, 0, 0, strings.size());

    for (int i = 0; i < n_lines; i++) {
        strings[i].printArr();
    }
    return 0;
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
