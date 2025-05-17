/*
Дано число N и N строк. Каждая строка содержит команду добавления или удаления натуральных чисел, а также запрос на получение k-ой порядковой статистики.
Команда добавления числа A задается положительным числом A, команда удаления числа A задается отрицательным числом "-A".
Запрос на получение k-ой порядковой статистики задается числом k. Требуемая скорость выполнения запроса - O(log n).
*/

#include <cassert>
#include <iostream>
#include <vector>

template <typename T>
struct DefaultComparator {
    int operator()(const T& l, const T& r) const {
        if (l < r) return -1;
        if (l > r) return 1;
        return 0;
    }
};

template <typename Key, typename Value>
struct AVLNode {
    AVLNode* left;
    AVLNode* right;
    Key key;
    Value value;
    uint height;
    size_t subtree_size;

    AVLNode(const Key& k, const Value& v)
        : left(nullptr),
          right(nullptr),
          key(k),
          value(v),
          height(1),
          subtree_size(1) {}

    ~AVLNode() {
        delete left;
        delete right;
    }
};

template <typename Key, typename Value, typename Comparator = DefaultComparator<Key>>
class AVLTree {
public:
    AVLTree(Comparator comp = Comparator()) : root(nullptr), comp(comp) {}

    ~AVLTree() {
        delete root;
    }

    void insert(const Key& key, const Value& value) {
        root = insert_aux(root, key, value);
    }

    void erase(const Key& key) {
        root = erase_aux(root, key);
    }

    const Key& find_kth(size_t k) const {
        assert(root != nullptr && k < root->subtree_size);
        return find_kth_aux(root, k);
    }

    bool contains(const Key& key) const {
        return find_aux(root, key) != nullptr;
    }

    size_t size() const {
        return root ? root->subtree_size : 0;
    }

private:
    AVLNode<Key, Value>* root;
    Comparator comp;

    AVLNode<Key, Value>* insert_aux(AVLNode<Key, Value>* node, const Key& key, const Value& value) {
        if (!node) {
            return new AVLNode<Key, Value>(key, value);
        }

        int cmp = comp(key, node->key);
        if (cmp == -1) {
            node->left = insert_aux(node->left, key, value);
        } else if (cmp == 1) {
            node->right = insert_aux(node->right, key, value);
        } else {
            node->value = value;
            return node;
        }

        return balance(node);
    }

    AVLNode<Key, Value>* erase_aux(AVLNode<Key, Value>* node, const Key& key) {
        if (!node) {
            return nullptr;
        }

        int cmp = comp(key, node->key);
        if (cmp == -1) {
            node->left = erase_aux(node->left, key);
        } else if (cmp == 1) {
            node->right = erase_aux(node->right, key);
        } else {
            if (!node->left && !node->right) {
                delete node;
                return nullptr;
            } else if (!node->left) {
                AVLNode<Key, Value>* right = node->right;
                node->right = nullptr;
                delete node;
                return right;
            } else if (!node->right) {
                AVLNode<Key, Value>* left = node->left;
                node->left = nullptr;
                delete node;
                return left;
            } else {
                AVLNode<Key, Value>* min_node = find_min(node->right);
                node->key = min_node->key;
                node->value = min_node->value;
                node->right = erase_aux(node->right, min_node->key);
            }
        }

        return balance(node);
    }

    const Key& find_kth_aux(AVLNode<Key, Value>* node, size_t k) const {
        size_t left_size = node->left ? node->left->subtree_size : 0;

        if (k < left_size) {
            return find_kth_aux(node->left, k);
        } else if (k == left_size) {
            return node->key;
        } else {
            return find_kth_aux(node->right, k - left_size - 1);
        }
    }

    AVLNode<Key, Value>* find_min(AVLNode<Key, Value>* node) const {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    AVLNode<Key, Value>* balance(AVLNode<Key, Value>* node) {
        fix_height(node);
        fix_subtree_size(node);

        int bf = bfactor(node);
        if (bf == 2) {
            if (bfactor(node->right) < 0) {
                node->right = rotate_right(node->right);
            }
            return rotate_left(node);
        } else if (bf == -2) {
            if (bfactor(node->left) > 0) {
                node->left = rotate_left(node->left);
            }
            return rotate_right(node);
        }
        return node;
    }

    AVLNode<Key, Value>* rotate_left(AVLNode<Key, Value>* node) {
        AVLNode<Key, Value>* new_root = node->right;
        node->right = new_root->left;
        new_root->left = node;

        fix_height(node);
        fix_height(new_root);
        fix_subtree_size(node);
        fix_subtree_size(new_root);

        return new_root;
    }

    AVLNode<Key, Value>* rotate_right(AVLNode<Key, Value>* node) {
        AVLNode<Key, Value>* new_root = node->left;
        node->left = new_root->right;
        new_root->right = node;

        fix_height(node);
        fix_height(new_root);
        fix_subtree_size(node);
        fix_subtree_size(new_root);

        return new_root;
    }

    int bfactor(AVLNode<Key, Value>* node) const {
        return height(node->right) - height(node->left);
    }

    uint height(AVLNode<Key, Value>* node) const {
        return node ? node->height : 0;
    }

    void fix_height(AVLNode<Key, Value>* node) {
        node->height = std::max(height(node->left), height(node->right)) + 1;
    }

    void fix_subtree_size(AVLNode<Key, Value>* node) {
        node->subtree_size = 1;
        if (node->left) node->subtree_size += node->left->subtree_size;
        if (node->right) node->subtree_size += node->right->subtree_size;
    }

    AVLNode<Key, Value>* find_aux(AVLNode<Key, Value>* node, const Key& key) const {
        if (!node) return nullptr;

        int cmp = comp(key, node->key);
        if (cmp == -1) {
            return find_aux(node->left, key);
        } else if (cmp == 1) {
            return find_aux(node->right, key);
        } else {
            return node;
        }
    }
};

int main() {
    AVLTree<int, int> tree;

    int N;
    std::cin >> N;

    for (int i = 0; i < N; ++i) {
        int A, k;
        std::cin >> A >> k;

        if (A >= 0) {
            tree.insert(A, 0);
        } else {
            tree.erase(-A);
        }

        std::cout << tree.find_kth(k) << " ";
    }

    return 0;
}