/*
Дан отсортированный массив различных целых чисел A[0..n-1] и массив целых чисел
B[0..m-1]. Для каждого элемента массива B[i] найдите минимальный индекс элемента
массива A[k], ближайшего по значению к B[i]. n ≤ 110000, m ≤ 1000. Время работы
поиска для каждого элемента B[i]: O(log(k)).
 */
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

size_t read_size_t(istream &iFile);

int *read_arr(size_t size, istream &iFile);

size_t exp_search(int num, const int *A, size_t size);

size_t bin_search(int num, size_t first, const int *A, size_t size);

int main() {
    size_t n, m;
    int *A, *B;
    ifstream infile("../input2_1.txt");
    n = read_size_t(infile);
    A = read_arr(n, infile);
    m = read_size_t(infile);
    B = read_arr(m, infile);

    for (int i = 0; i < m; i++) {
        cout << exp_search(B[i], A, n) << " ";
    }

    infile.close();
    delete[] A;
    delete[] B;
    return 0;
}

size_t read_size_t(istream &iFile){
    size_t n;
    iFile >> n;
    return  n;
}

int *read_arr(size_t size, istream &iFile){
    int *arr = new int[size];
    for (int i = 0; i < size; i++) {
        iFile >> arr[i];
    }
    return arr;
}

size_t exp_search(int num, const int *A, size_t size) {
    if (size == 1 || A[0] >= num) {
        return 0;
    }
    size_t i = 1;
    while (i * 2 < size && A[i * 2] < num) {
        i = i * 2;
    }
    return bin_search(num, i, A, size);
}

size_t bin_search(int num, size_t first, const int *A, size_t size) {
    size_t m, return_index;
    size_t last = size - 1;
    while (first < last) {
        m = (last + first) / 2;
        if (A[m] < num) {
            first = m + 1;
        } else {
            last = m;
        }
    }
    if (abs(A[first] - num) < abs(A[first - 1] - num)) {
        return_index = first;
    } else { return_index = first - 1; }
    return return_index;
}