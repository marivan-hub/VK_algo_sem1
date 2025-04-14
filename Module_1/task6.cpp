// 6_2. Реализуйте стратегию выбора опорного элемента “медиана трёх”.
// Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.
#include <iostream>
#include <cassert>

struct StandartComparator {
    bool operator()(const int& a, const int& b) const {
        return a < b;
    }
};

template <typename T, typename Comparator>
size_t FindPyvotLikeMedianOfThree(T* arr, size_t left, size_t right, Comparator comp) {
    size_t mid = left + (right - left) / 2;

    if (comp(arr[right], arr[left])) {
        std::swap(arr[left], arr[right]);
    }

    if (comp(arr[mid], arr[left])) {
        std::swap(arr[mid], arr[left]);
    }

    if (comp(arr[right], arr[mid])) {
        std::swap(arr[mid], arr[right]);
    }
    return mid;
}


template <typename T, typename Comparator>
size_t Partition(T* arr, size_t left, size_t right, Comparator comp) {
    size_t pivot_idx = FindPyvotLikeMedianOfThree(arr, left, right, comp);
    std::swap(arr[pivot_idx], arr[left]);
    T pivot = arr[left];

    size_t i = right;
    size_t j = right;

    while (j > left) {
        if (comp(pivot, arr[j])) {
            std::swap(arr[i], arr[j]);
            i--;
        }
        j--;
    }
    std::swap(arr[left], arr[i]);

    return i;
}

template <typename T, typename Comparator>
T FindKStatistick(T* arr, size_t left, size_t right, size_t k, Comparator comp) {
    while (left < right) {
        size_t pivot_pos = Partition(arr, left, right, comp);

        if (pivot_pos == k) {
            return arr[k];
        } else if (pivot_pos < k) {
            left = pivot_pos + 1;
        } else {
            right = pivot_pos - 1;
        }
    }
    return arr[left];
}

int main() {
    size_t n;
    std::cin >> n;
    assert(n > 0);

    int* arr = new int[n];
    for (size_t i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    StandartComparator comp;

    size_t p10 = n / 10;
    size_t median = n / 2;
    size_t p90 = 9 * n / 10;

    int percentile10 = FindKStatistick(arr, 0, n - 1, p10, comp);
    int median_val = FindKStatistick(arr, 0, n - 1, median, comp);
    int percentile90 = FindKStatistick(arr, 0, n - 1, p90, comp);

    std::cout << percentile10 << "\n" << median_val << "\n" << percentile90 << std::endl;

    delete[] arr;
    return 0;
}