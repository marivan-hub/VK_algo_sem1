// Требование для всех вариантов Задачи 5
// Во всех задачах данного раздела необходимо реализовать и использовать сортировку слиянием в виде шаблонной функции.
// Решение должно поддерживать передачу функции сравнения снаружи.
// Общее время работы алгоритма O(n log n).
// 5_3. Закраска прямой 1.
// На числовой прямой окрасили N отрезков. Известны координаты левого и правого концов каждого отрезка (Li и Ri). Найти длину окрашенной части числовой прямой.
#include <iostream>
#include <cassert>


struct Segment {
    int left;
    int right;
};


struct SegmentComparator {
    bool operator()(const Segment& a, const Segment& b) const {
        return a.left < b.left;
    }
};

template <typename T, typename Comparator>
void MergeSort(T* array, size_t size, Comparator comp) {
    if (size <= 1) return;

    size_t mid = size / 2;
    MergeSort(array, mid, comp);
    MergeSort(array + mid, size - mid, comp);

    T* temp = new T[size];
    size_t i = 0, j = mid, k = 0;

    while (i < mid && j < size) {
        if (comp(array[i], array[j])) {
            temp[k++] = array[i++];
        } else {
            temp[k++] = array[j++];
        }
    }

    while (i < mid) temp[k++] = array[i++];
    while (j < size) temp[k++] = array[j++];

    for (size_t m = 0; m < size; ++m) {
        array[m] = temp[m];
    }

    delete[] temp;
}

int CalculatePaintedLength(Segment* segments, size_t n) {
    if (n == 0) return 0;

    SegmentComparator comp;
    MergeSort(segments, n, comp);

    int total_length = 0;
    int current_left = segments[0].left;
    int current_right = segments[0].right;

    for (size_t i = 1; i < n; ++i) {
        if (segments[i].left <= current_right) {
            if (segments[i].right > current_right) {
                current_right = segments[i].right;
            }
        } else {
            total_length += current_right - current_left;
            current_left = segments[i].left;
            current_right = segments[i].right;
        }
    }

    total_length += current_right - current_left;

    return total_length;
}

int main() {
    int N;
    std::cin >> N;
    assert(N >= 0 );

    Segment* segments = new Segment[N];
    for (int i = 0; i < N; ++i) {
        std::cin >> segments[i].left >> segments[i].right;
        assert(segments[i].left <= segments[i].right );
    }

    std::cout << CalculatePaintedLength(segments, N) << std::endl;

    delete[] segments;
    return 0;
}