// Дан отсортированный массив различных целых чисел A[0..n-1] и массив целых чисел
// B[0..m-1]. Для каждого элемента массива B[i] найдите минимальный индекс элемента
// массива A[k], ближайшего по значению к B[i].
// Требования: Время работы поиска для каждого элемента B[i]: O(log(k)).

#include <iostream>
#include <cassert>

int FindRangeByExponentialSearch(const int* A, int n, int target) {
    int bound = 1;
    while (bound < n && A[bound] < target) {
        bound *= 2;
    }
    if (bound < n) {
        return bound;
    }
    else {
        return n - 1;
    }
}

int binarySearchClosestElement(const int* A, int left, int right, int target) {
    while (left < right) {
        int mid = (left + right) / 2;
        if (A[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    if (left == 0) {
        return 0;
    }
    if (target - A[left - 1] <= A[left] - target) {
        return left - 1;
    }
    return left;
}

void findClosestElements(const int* A, int n, const int* B, int m, int* result) {
    for (int i = 0; i < m; ++i) {
        int right = FindRangeByExponentialSearch(A, n, B[i]);
        int left;
        if (right == 0) {
            left = 0;
        } else {
            left = right / 2;
        }
        result[i] = binarySearchClosestElement(A, left, right, B[i]);
    }
}

int main() {
    int n;
    std::cin >> n;
    assert(n >= 0);
    assert(n <= 110000);

    int* A = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> A[i];
    }

    int m;
    std::cin >> m;
    assert(m >= 0);
    assert(m <= 1000);

    int* B = new int[m];
    for (int i = 0; i < m; ++i) {
        std::cin >> B[i];
    }

    int* result = new int[m];
    findClosestElements(A, n, B, m, result);

    for (int i = 0; i < m; ++i) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;

    delete[] A;
    delete[] B;
    delete[] result;
    return 0;
}
