/*Реализуйте структуру данных типа “множество строк” на основе динамической
хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных
латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения
многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте
при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает
3/4.
Структура данных должна поддерживать операции добавления строки в множество,
удаления строки из множества и проверки принадлежности данной строки множеству.
1_2. Для разрешения коллизий используйте двойное хеширование.*/


#include <iostream>
#include <string>
#include <vector>

#define INITIAL_CAPACITY 8
#define MAX_LOAD_FACTOR 0.75
#define HASH_PRIME1 31
#define HASH_PRIME2 53

enum class CellState {
    EMPTY,
    FILLED,
    DELETED
};


struct Cell {
    std::string value;
    CellState state;

    Cell() : value(""), state(CellState::EMPTY) {}
};

class StringSet {
public:
    StringSet() : capacity_(INITIAL_CAPACITY), size_(0) {
        table_.resize(capacity_);
    }

    bool add(const std::string& key) {
        if ((double)size_ / capacity_ >= MAX_LOAD_FACTOR) {
            rehash();
        }

        int index = findIndex(key);
        if (index >= 0) {
            return false;
        }

        int insertIndex = findInsertIndex(key);
        table_[insertIndex].value = key;
        table_[insertIndex].state = CellState::FILLED;
        ++size_;
        return true;
    }

    bool remove(const std::string& key) {
        int index = findIndex(key);
        if (index < 0) {
            return false;
        }

        table_[index].state = CellState::DELETED;
        --size_;
        return true;
    }

    bool contains(const std::string& key) const {
        return findIndex(key) >= 0;
    }

private:
    std::vector<Cell> table_;
    int capacity_;
    int size_;

    int computeHash1(const std::string& key, int mod) const {
        unsigned int hash = 0;
        for (char ch : key) {
            hash = (hash * HASH_PRIME1 + ch) % mod;
        }
        return hash;
    }

    int computeHash2(const std::string& key, int mod) const {
        unsigned int hash = 0;
        for (char ch : key) {
            hash = (hash * HASH_PRIME2 + ch) % mod;
        }
        return (2 * hash + 1) % mod;
    }

    int findIndex(const std::string& key) const {
        int hash1 = computeHash1(key, capacity_);
        int hash2 = computeHash2(key, capacity_);

        for (int i = 0; i < capacity_; ++i) {
            int idx = (hash1 + i * hash2) % capacity_;
            if (table_[idx].state == CellState::EMPTY) {
                return -1;
            }
            if (table_[idx].state == CellState::FILLED && table_[idx].value == key) {
                return idx;
            }
        }
        return -1;
    }

    int findInsertIndex(const std::string& key) const {
        int hash1 = computeHash1(key, capacity_);
        int hash2 = computeHash2(key, capacity_);
        int firstDeleted = -1;

        for (int i = 0; i < capacity_; ++i) {
            int idx = (hash1 + i * hash2) % capacity_;
            if (table_[idx].state == CellState::FILLED && table_[idx].value == key) {
                return -1;
            }
            if (table_[idx].state == CellState::DELETED && firstDeleted == -1) {
                firstDeleted = idx;
            }
            if (table_[idx].state == CellState::EMPTY) {
                return firstDeleted != -1 ? firstDeleted : idx;
            }
        }
        return firstDeleted;
    }

    void rehash() {
        int newCapacity = capacity_ * 2;
        std::vector<Cell> newTable(newCapacity);

        for (const auto& cell : table_) {
            if (cell.state == CellState::FILLED) {
                int hash1 = computeHash1(cell.value, newCapacity);
                int hash2 = computeHash2(cell.value, newCapacity);
                for (int i = 0; i < newCapacity; ++i) {
                    int idx = (hash1 + i * hash2) % newCapacity;
                    if (newTable[idx].state == CellState::EMPTY) {
                        newTable[idx].value = cell.value;
                        newTable[idx].state = CellState::FILLED;
                        break;
                    }
                }
            }
        }

        table_ = std::move(newTable);
        capacity_ = newCapacity;
    }
};

int main() {
    StringSet stringSet;
    char op;
    std::string key;

    while (std::cin >> op >> key) {
        bool result = false;
        switch (op) {
            case '+':
                result = stringSet.add(key);
                break;
            case '-':
                result = stringSet.remove(key);
                break;
            case '?':
                result = stringSet.contains(key);
                break;
        }
        std::cout << (result ? "OK" : "FAIL") << std::endl;
    }

    return 0;
}
