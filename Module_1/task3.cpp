// Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
// Формат входных данных.
// В первой строке количество команд n. n ≤ 1000000.
// Каждая команда задаётся как 2 целых числа: a b.
// a = 1 - push front
// a = 2 - pop front
// a = 3 - push back
// a = 4 - pop back
// Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
// Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
// Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.
// Формат выходных данных.
// Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.


// 3_2. Реализовать дек с динамическим зацикленным буфером (на основе динамического массива).
// Требования: Дек должен быть реализован в виде класса.

#include <iostream>
#include <cassert>

class Deque {
private:
    int* buffer;
    int current_capacity;
    int index_start_of_deque;
    int index_end_of_deque;
    int size;


    void resize() {
        int new_capacity;

        if (current_capacity == 0) {
            new_capacity = 1;
        }
        else {
            new_capacity = current_capacity * 2;
        }

        int* new_buffer = new int[new_capacity];

        if (size > 0) {
            if (index_start_of_deque <= index_end_of_deque) {
                for (int i = index_start_of_deque; i <= index_end_of_deque; ++i) {
                    new_buffer[i - index_start_of_deque] = buffer[i];
                }
            }
            else {
                int first_part = current_capacity - index_start_of_deque;

                for (int i = 0; i < first_part; ++i) {
                    new_buffer[i] = buffer[index_start_of_deque + i];
                }
                for (int i = 0; i <= index_end_of_deque; ++i) {
                    new_buffer[first_part + i] = buffer[i];
                }
            }
        }

        delete[] buffer;
        buffer = new_buffer;
        current_capacity = new_capacity;
        index_start_of_deque = 0;
        index_end_of_deque = size - 1;
    }

public:
    Deque() : buffer(nullptr), current_capacity(0), index_start_of_deque(0), index_end_of_deque(-1), size(0) {}

    ~Deque() {
        delete[] buffer;
    }

    void push_front(int value) {
        if (size == current_capacity) {
            resize();
        }

        if (size == 0) {
            index_start_of_deque = index_end_of_deque = 0;
        } else {
            index_start_of_deque = (index_start_of_deque - 1 + current_capacity) % current_capacity;
        }

        buffer[index_start_of_deque] = value;
        size++;
    }

    void push_back(int value) {
        if (size == current_capacity) {
            resize();
        }

        if (size == 0) {
            index_start_of_deque = index_end_of_deque = 0;
        } else {
            index_end_of_deque = (index_end_of_deque + 1) % current_capacity;
        }

        buffer[index_end_of_deque] = value;
        size++;
    }


    int pop_front() {
        if (size == 0) {
            return -1;
        }

        int value = buffer[index_start_of_deque];
        if (size == 1) {
            index_start_of_deque = index_end_of_deque = -1;
        } else {
            index_start_of_deque = (index_start_of_deque + 1) % current_capacity;
        }

        size--;
        return value;
    }


    int pop_back() {
        if (size == 0) {
            return -1;
        }

        int value = buffer[index_end_of_deque];
        if (size == 1) {
            index_start_of_deque = index_end_of_deque = -1;
        } else {
            index_end_of_deque = (index_end_of_deque - 1 + current_capacity) % current_capacity;
        }

        --size;
        return value;
    }


    bool is_empty() const {
        return size == 0;
    }
};

int main() {
    int n;
    std::cin >> n;
    assert(n <= 1000000);

    Deque deque;
    bool answer = true;

    for (int i = 0; i < n; ++i) {
        int a, b;
        std::cin >> a >> b;

        switch (a) {
            case 1:
                deque.push_front(b);
                break;

            case 2:
                {
                    int value = deque.pop_front();
                    if (value != b) {
                        answer = false;
                    }
                }
                break;

            case 3:
                deque.push_back(b);
                break;

            case 4:
                {
                    int value = deque.pop_back();
                    if (value != b) {
                        answer = false;
                    }
                }
                break;
            default:
                assert(false);
        }
    }

    std::cout << (answer ? "YES" : "NO") << std::endl;

    return 0;
}