// 4.2 Топ K пользователей из лога
// Имеется лог-файл, в котором хранятся пары для N пользователей (Идентификатор пользователя, посещаемость сайта).
// Напишите программу, которая выбирает K пользователей, которые чаще других заходили на сайт, и выводит их в порядке возрастания посещаемости. Количество заходов и идентификаторы пользователей не повторяются.
// Требования: время работы O(N * logK), где N - кол-во пользователей. Ограничение на размер кучи O(K).
// Формат входных данных: Сначала вводятся N и K, затем пары (Идентификатор пользователя, посещаемость сайта).
// Формат выходных данных: Идентификаторы пользователей в порядке возрастания посещаемости

#include <iostream>
#include <cassert>

struct UserLogsInfo {
    int id;
    int visits;

    UserLogsInfo() : id(0), visits(0) {}
    UserLogsInfo(int id, int visits) : id(id), visits(visits) {}
};

struct ComparatorByVisits {
    bool operator()(const UserLogsInfo& a, const UserLogsInfo& b) const {
        return a.visits < b.visits;
    }
};

template<typename T, typename Comparator>
class BinaryHeap {
public:
    BinaryHeap(Comparator cmp) : data(nullptr), size(0), capacity(0), cmp(cmp) {}

    ~BinaryHeap() {
        delete[] data;
    }

    void Push(const T& value) {
        if (size == capacity) {
            Grow();
        }
        data[size] = value;
        SiftUp(size);
        ++size;
    }

    T Pop() {
        assert(size > 0);
        T result = data[0];
        data[0] = data[--size];
        SiftDown(0);
        return result;
    }

    const T& Top() const {
        assert(size > 0);
        return data[0];
    }

    size_t Size() const {
        return size;
    }

    void ExtractAllInOrder(T* output) {
        size_t original_size = size;
        for (size_t i = 0; i < original_size; ++i) {
            output[i] = Pop();
        }
    }

    void BuildHeap(T* array, size_t n) {
        delete[] data;
        data = new T[n];
        capacity = n;
        size = n;
        for (size_t i = 0; i < n; ++i) {
            data[i] = array[i];
        }
        for (int i = (int)n / 2 - 1; i >= 0; --i) {
            SiftDown(i);
        }
    }

private:
    T* data;
    size_t size;
    size_t capacity;
    Comparator cmp;

    void Grow() {
        size_t new_capacity = capacity == 0 ? 1 : capacity * 2;
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

    void SiftUp(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (!cmp(data[index], data[parent])) {
                break;
            }
            std::swap(data[index], data[parent]);
            index = parent;
        }
    }

    void SiftDown(size_t index) {
        while (true) {
            size_t left = 2 * index + 1;
            size_t right = 2 * index + 2;
            size_t smallest = index;

            if (left < size && cmp(data[left], data[smallest])) {
                smallest = left;
            }
            if (right < size && cmp(data[right], data[smallest])) {
                smallest = right;
            }

            if (smallest == index) {
                break;
            }

            std::swap(data[index], data[smallest]);
            index = smallest;
        }
    }
};

void FindTopKUsers(int n, int k, UserLogsInfo* input, UserLogsInfo* output) {
    ComparatorByVisits cmp;
    BinaryHeap<UserLogsInfo, ComparatorByVisits> heap(cmp);

    heap.BuildHeap(input, k);

    for (int i = k; i < n; ++i) {
        if (input[i].visits > heap.Top().visits) {
            heap.Pop();
            heap.Push(input[i]);
        }
    }

    heap.ExtractAllInOrder(output);
}

int main() {
    int n, k;
    std::cin >> n >> k;
    assert(n >= k);

    UserLogsInfo* users = new UserLogsInfo[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> users[i].id >> users[i].visits;
    }

    UserLogsInfo* top_users = new UserLogsInfo[k];
    FindTopKUsers(n, k, users, top_users);

    for (int i = 0; i < k; ++i) {
        std::cout << top_users[i].id << " ";;
    }
    std::cout << std::endl;

    delete[] users;
    delete[] top_users;

    return 0;
}