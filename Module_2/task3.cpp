/*
Постройте B-дерево минимального порядка t и выведите его по слоям.
В качестве ключа используются числа, лежащие в диапазоне 0..232 -1

Требования:
B-дерево должно быть реализовано в виде шаблонного класса.
Решение должно поддерживать передачу функции сравнения снаружи.

Формат входных данных
Сначала вводится минимальный порядок дерева t.
Затем вводятся элементы дерева.

Формат выходных данных
Программа должна вывести B-дерево по слоям. Каждый слой на новой строке, элементы должны выводится в том порядке, в котором они лежат в узлах.
*/

#include <iostream>
#include <vector>

template <typename T, typename Compare = std::less<T>>
class BTree {
private:
    struct Node {
        bool IsLeaf;
        std::vector<T> Keys;
        std::vector<Node*> Children;

        Node(bool isLeaf) : IsLeaf(isLeaf) {}

        ~Node() {
            for (Node* child : Children) {
                delete child;
            }
        }
    };

    Node* root;
    size_t t;
    Compare comp;

public:
    BTree(size_t order, const Compare& cmp = Compare())
        : root(nullptr), t(order), comp(cmp) {}

    ~BTree() {
        delete root;
    }

    void Add(const T& key) {
        if (root == nullptr) {
            root = new Node(true);
            root->Keys.push_back(key);
            return;
        }

        if (IsNodeFull(root)) {
            Node* newRoot = new Node(false);
            newRoot->Children.push_back(root);
            SplitChild(newRoot, 0);
            root = newRoot;
        }

        InsertNonFull(root, key);
    }

    void PrintByLevels() const {
        if (root == nullptr) return;

        std::vector<Node*> currentLevel;
        std::vector<Node*> nextLevel;
        currentLevel.push_back(root);

        while (!currentLevel.empty()) {
            for (Node* node : currentLevel) {
                for (const T& key : node->Keys) {
                    std::cout << key << " ";
                }

                if (!node->IsLeaf) {
                    for (Node* child : node->Children) {
                        nextLevel.push_back(child);
                    }
                }
            }
            std::cout << std::endl;

            currentLevel = nextLevel;
            nextLevel.clear();
        }
    }

private:
    bool IsNodeFull(Node* node) const {
        return node->Keys.size() == 2 * t - 1;
    }

    void SplitChild(Node* parent, size_t index) {
        Node* child = parent->Children[index];
        Node* newChild = new Node(child->IsLeaf);

        for (size_t i = t; i < child->Keys.size(); ++i) {
            newChild->Keys.push_back(child->Keys[i]);
        }
        child->Keys.resize(t - 1);

        if (!child->IsLeaf) {
            for (size_t i = t; i < child->Children.size(); ++i) {
                newChild->Children.push_back(child->Children[i]);
            }
            child->Children.resize(t);
        }

        parent->Keys.insert(parent->Keys.begin() + index, child->Keys[t - 1]);
        parent->Children.insert(parent->Children.begin() + index + 1, newChild);
    }

    void InsertNonFull(Node* node, const T& key) {
        int pos = node->Keys.size() - 1;

        if (node->IsLeaf) {
            node->Keys.resize(node->Keys.size() + 1);
            while (pos >= 0 && comp(key, node->Keys[pos])) {
                node->Keys[pos + 1] = node->Keys[pos];
                pos--;
            }
            node->Keys[pos + 1] = key;
        } else {
            while (pos >= 0 && comp(key, node->Keys[pos])) {
                pos--;
            }
            pos++;

            if (IsNodeFull(node->Children[pos])) {
                SplitChild(node, pos);
                if (comp(node->Keys[pos], key)) {
                    pos++;
                }
            }

            InsertNonFull(node->Children[pos], key);
        }
    }
};

int main() {
    int t;
    std::cin >> t;

    BTree<int> tree(t);
    int key;

    while (std::cin >> key) {
        tree.Add(key);
    }

    tree.PrintByLevels();

    return 0;
}