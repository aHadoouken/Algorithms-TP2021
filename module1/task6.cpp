/*
Дано множество целых чисел из [0..109] размера n.
Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
10% перцентиль
        медиана
90% перцентиль
        Требования:

К дополнительной памяти: O(n).
Среднее время работы: O(n)
Должна быть отдельно выделенная функция partition.
Рекурсия запрещена.
Решение должно поддерживать передачу функции сравнения снаружи.
*/
#include <iostream>
#include <fstream>


template<typename T, typename Comparator = std::less<T>>
int find_pivot(T *arr, int l, int r, Comparator cmp) {
    int len = r - l;
    int ind1 = rand() % len + l;
    int ind2 = rand() % len + l;
    int ind3 = rand() % len + l;
    //std::cout << " " <<len << " " <<ind1 << " " <<ind2 << " " <<ind3 << std::endl;
    T p1 = arr[ind1];
    T p2 = arr[ind2];
    T p3 = arr[ind3];
    if (cmp(p2, p1)) {
        if (cmp(p3, p1)) {
            if (cmp(p2, p3)) {
                return ind3;
            } else { return ind2; }
        } else { return ind1; }
    } else {
        if (cmp(p1, p3)) {
            if (cmp(p2, p3)) {
                return ind2;
            } else { return ind3; }
        } else { return ind1; }
    }
}

template<typename T, typename Comparator = std::less<T>>
int partition(T *arr, int l, int r, Comparator cmp) {
    int pivot_ind = find_pivot(arr, l, r, cmp);
    //std::cout << pivot_ind << std::endl;
    T pivot = arr[pivot_ind];
    std::swap(arr[pivot_ind], arr[r - 1]);
    int i = l;
    int j = l;

    while (j != r - 1) {
        if (arr[j] <= pivot){
            std::swap(arr[i], arr[j]);
            i++;
        }
        j++;
    }
    std::swap(arr[i], arr[j]);
    return i;
}

template<typename T, typename Comparator = std::less<T>>
T kth_stat(T *arr, int k, int l, int r, Comparator cmp = Comparator()) {
    int pivot_pos = -1;
    //std::cout << k << std::endl;
    while (l < r && pivot_pos != k) {
        pivot_pos = partition(arr, l, r, cmp);
        //std::cout << pivot_pos << " " << l << " " << r << std::endl;
        if (k < pivot_pos) {
            r = pivot_pos;
        } else {
            l = pivot_pos + 1;
        }
        //std::cout << pivot_pos << " " << l << " " << r << std::endl;
    }
    return arr[pivot_pos];
}

int main(int argc, const char *argv[]) {
    std::ifstream infile("../input6_1.txt");
    int n_elems;
    infile >> n_elems;
    auto arr = new int[n_elems];
    for (int i = 0; i < n_elems; i++) {
        infile >> arr[i];
    }
    int ten_perc = kth_stat(arr, int(n_elems * 0.1), 0, n_elems);
    int median = kth_stat(arr, n_elems / 2, 0, n_elems);
    int ninety_perc = kth_stat(arr, int(n_elems * 0.9), 0, n_elems);
    std::cout << ten_perc << std::endl;
    std::cout << median << std::endl;
    std::cout << ninety_perc << std::endl;

    delete[]arr;
    return 0;
}
