//На числовой прямой окрасили N отрезков. Известны координаты левого и правого
//концов каждого отрезка (Li и Ri). Найти длину окрашенной части числовой прямой.
#include <iostream>
#include <fstream>

template<typename T>
void print_arr(const T *arr, size_t size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;
}

template<typename T, typename Comparator = std::less<T>>
void Merge(T *arr, int l, int m, int r, Comparator cmp = Comparator()) {
    int it1 = l;
    int it2 = m;
    int index = 0;
    size_t tmp_size = r - l;
    auto tmp = new T[tmp_size];
    while (it1 < m && it2 < r) {
        if (cmp(arr[it1], arr[it2])) {
            tmp[index] = arr[it1];
            it1++;
        } else {
            tmp[index] = arr[it2];
            it2++;
        }
        index++;
    }
    while (it1 < m) {
        tmp[index] = arr[it1];
        it1++;
        index++;
    }
    while (it2 < r) {
        tmp[index] = arr[it2];
        it2++;
        index++;
    }
    for (int i = 0; i < tmp_size; i++) {
        arr[l + i] = tmp[i];
    }
    delete[]tmp;
}

template<typename T, typename Comparator = std::less<T>>
void MergeSortRecursive(T *arr, int l, int r, Comparator cmp = Comparator()) {
    if (l + 1 >= r) {
        return;
    }
    int mid = (l + r) / 2;
    MergeSortRecursive(arr, l, mid, cmp);
    MergeSortRecursive(arr, mid, r, cmp);
    Merge(arr, l, mid, r, cmp);
}

struct ColorPoints {
    int coord;
    int val;
};

class PointsComparator {
public:
    bool operator()(const ColorPoints &l, const ColorPoints &r) const {
        return l.coord < r.coord;
    }
};

void findColorLen(ColorPoints *points, int arr_size) {
    MergeSortRecursive(points, 0, arr_size, PointsComparator());
    int color_len = 0;
    int layers_num = 0;
    int l, r;
    for (int i = 0; i < arr_size; i++) {
        if (layers_num == 0) {
            l = points[i].coord;
        }
        layers_num += points[i].val;
        if (layers_num == 0) {
            r = points[i].coord;
            color_len += r - l;
        }
    }
    std::cout << color_len << std::endl;
}

int main() {
    std::ifstream infile("../input5_1.txt");
    int n_colors;
    infile >> n_colors;
    auto points = new ColorPoints[2 * n_colors];

    for (int i = 0; i < 2 * n_colors; i += 2) {
        infile >> points[i].coord >> points[i + 1].coord;
        points[i].val = 1;
        points[i + 1].val = -1;
    }

    findColorLen(points, 2 * n_colors);

    delete[]points;
    return 0;
};
