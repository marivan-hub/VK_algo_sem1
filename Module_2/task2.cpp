/*
Дано число N < 10^6 и последовательность целых чисел из [-231..231] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Требования: Рекурсия запрещена. Решение должно поддерживать передачу функции сравнения снаружи.
2_2. Выведите элементы в порядке pre-order (сверху вниз).
*/


#include <iostream>
#include <stack>
#include <vector>

template <typename T>
struct DefaultComparator {
    bool operator()(const T& l, const T& r) const {
        return l < r;
    }
};

template <typename T, typename Compare = DefaultComparator<T>>
class BinaryTree {
private:
    struct Node {
        T key;
        Node* left;
        Node* right;

        explicit Node(const T& k) : key(k), left(nullptr), right(nullptr) {}
    };

    Node* root;
    Compare comp;
    size_t size;

public:
    explicit BinaryTree(const Compare& cmp = Compare()) : root(nullptr), comp(cmp), size(0) {}

    ~BinaryTree() {
        Clear();
    }

    BinaryTree(const BinaryTree&) = delete;
    BinaryTree& operator=(const BinaryTree&) = delete;

    void Insert(const T& key) {
        Node* newNode = new Node(key);
        if (!root) {
            root = newNode;
            size++;
            return;
        }

        Node* current = root;
        while (true) {
            if (comp(key, current->key)) {
                if (!current->left) {
                    current->left = newNode;
                    break;
                }
                current = current->left;
            } else {
                if (!current->right) {
                    current->right = newNode;
                    break;
                }
                current = current->right;
            }
        }
        size++;
    }

    std::vector<T> PreOrderTraversal() const {
        std::vector<T> result;
        if (!root) return result;

        std::stack<Node*> nodes;
        nodes.push(root);

        while (!nodes.empty()) {
            Node* current = nodes.top();
            nodes.pop();
            result.push_back(current->key);

            if (current->right) {
                nodes.push(current->right);
            }
            if (current->left) {
                nodes.push(current->left);
            }
        }

        return result;
    }

    void Clear() {
        if (!root) return;

        std::stack<Node*> nodes;
        nodes.push(root);

        while (!nodes.empty()) {
            Node* current = nodes.top();
            nodes.pop();

            if (current->left) {
                nodes.push(current->left);
            }
            if (current->right) {
                nodes.push(current->right);
            }

            delete current;
        }

        root = nullptr;
        size = 0;
    }

    size_t Size() const {
        return size;
    }
};

int main() {
    BinaryTree<int> tree;
    int n;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        int num;
        std::cin >> num;
        tree.Insert(num);
    }

    std::vector<int> preOrder = tree.PreOrderTraversal();
    for (const auto& num : preOrder) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}