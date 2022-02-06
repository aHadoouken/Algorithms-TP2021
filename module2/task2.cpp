/*
 * Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
 * Требуется построить бинарное дерево, заданное наивным порядком вставки. Т.е.,
 * при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
 * то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
 * Выведите элементы в порядке pre-order (сверху вниз).
Рекурсия запрещена.
 */
#include <iostream>
#include <stack>
#include <fstream>

template<typename T, typename Comparator = std::less<T>>
class Tree {
    struct Node {
        Node(const T &key)
                : key(key), left(nullptr), right(nullptr) {
        }

        T key;
        Node *left;
        Node *right;
    };

public:
    Tree()
            : root(nullptr) {
    }

    ~Tree() {
        destroyTree(root);
    }

    void Add(const T &key) {
        Node *tmp = root;
        Node *parent = nullptr;
        while (tmp) {
            if (cmp(key, tmp->key)) {
                parent = tmp;
                tmp = tmp->right;
            } else {
                parent = tmp;
                tmp = tmp->left;
            }
        }
        auto node = new Node(key);
        if (parent) {
            if (cmp(key, parent->key)) {
                parent->right = node;
            } else {
                parent->left = node;
            }
        } else {
            root = node;
        }
    }

    bool Has(const T &key) {
        Node *tmp = root;
        while (tmp) {
            if (cmp.eq(tmp->key, key)) {
                return true;
            } else if (cmp(key, tmp->key)) {
                tmp = tmp->right;
            } else {
                tmp = tmp->left;
            }
        }
        return false;
    }

    void printPreOrder() {
        if (!root) {
            return;
        }
        std::stack<Node *> stack;
        stack.push(root);
        Node *tmp;
        while (!stack.empty()) {
            tmp = stack.top();
            std::cout << tmp->key << " ";
            stack.pop();
            if (tmp->right) {
                stack.push(tmp->right);
            }
            if (tmp->left) {
                stack.push(tmp->left);
            }
        }
        std::cout << std::endl;
    }

private:
    Node *root;

    Comparator cmp;

    void destroyTree(Node *node) {
        if (!root) {
            return;
        }
        std::stack<Node *> stack;
        stack.push(root);
        Node *tmp;
        while (!stack.empty()) {
            tmp = stack.top();
            stack.pop();
            if (tmp->right) {
                stack.push(tmp->right);
            }
            if (tmp->left) {
                stack.push(tmp->left);
            }
            delete tmp;
        }
    }
};

template<typename T>
class Comparator {
public:
    bool operator()(const T &l, const T &r) const {
        return l >= r;
    }

    bool eq(const T &l, const T &r) {
        return l == r;
    }
};

int main(int argc, const char *argv[]) {
    std::ifstream file("../input2_1.txt");
    Tree<int, Comparator<int>> tree;
    size_t num;
    int key;
    file >> num;
    for (int i = 0; i < num; i++) {
        file >> key;
        tree.Add(key);
    }
    tree.printPreOrder();
    return 0;
}
