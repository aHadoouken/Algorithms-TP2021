/*
Постройте B-дерево минимального порядка t и выведите его по слоям.
В качестве ключа используются числа, лежащие в диапазоне [0..232-1]
Требования:

B-дерево должно быть реализовано в виде шаблонного класса.
Решение должно поддерживать передачу функции сравнения снаружи.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

template<typename T, typename Comparator = std::less<T>>
class BTree {
public:
    struct Node {
        Node(bool leaf)
                : leaf(leaf) {
        }

        ~Node() {
            for (Node *child: children) {
                delete child;
            }
        }

        bool leaf;
        std::vector<T> keys;
        std::vector<Node *> children;
    };

    BTree(int min_degree)
            : t(min_degree), root(nullptr) {
    }

    ~BTree() {
        destroyTree(root);
    }

    void Insert(const T &key) {
        if (!root)
            root = new Node(true);

        // здесь и дальше: если при спуске по дереву находим
        // переполненный узел -- сначала разбиваем его, потом спускаемся
        if (isNodeFull(root)) {
            Node *newRoot = new Node(false);
            newRoot->children.push_back(root);
            root = newRoot;
            splitChild(root, 0);
        }

        // теперь корень точно не переполнен, можем вызвать insertNonFull
        insertNonFull(root, key);
    }

    void debugPrint() {
        debugPrintInternal(root, 0);
    }

    void Print() {
        std::queue<Node *> q;
        q.push(root);
        int n_count = 1, i_count;
        while (!q.empty()) {
            i_count = n_count;
            n_count = 0;
            for (int j = 0; j < i_count; j++) {
                Node *print_node = q.front();
                q.pop();
                for (int i = 0; i < print_node->keys.size(); i++) {
                    std::cout << print_node->keys[i] << " ";
                }
                for (int i = 0; i < print_node->children.size(); i++) {
                    q.push(print_node->children[i]);
                    n_count++;
                }
            }
            std::cout << std::endl;
        }
    }

private:
    void destroyTree(Node *node) {
        if (node) {
            if (node->leaf) {
                for (int i = 0; i < node->children.size(); i++) {
                    destroyTree(node->children[i]);
                }
            }
            delete node;
        }
    }

    void debugPrintInternal(Node *node, int indent) {
        std::cout << std::string(indent, ' ');
        std::cout << "keys: [";
        for (auto it = node->keys.begin(); it != node->keys.end(); it++) {
            std::cout << (*it);
            if (it + 1 != node->keys.end())
                std::cout << ", ";
        }
        std::cout << "]" << std::endl;

        for (auto child: node->children) {
            debugPrintInternal(child, indent + 4);
        }
    }

    bool isNodeFull(Node *node) {
        return node->keys.size() == 2 * t - 1;
    }

    // разбить переполненного потомка index узла node
    void splitChild(Node *node, int index) {
        node->children.resize(node->children.size() + 1);
        node->keys.resize(node->keys.size() + 1);
        for (int i = node->children.size() - 1; i > index; i--) {
            node->children[i] = node->children[i - 1];
        }
        for (int i = node->keys.size() - 1; i > index; i--) {
            node->keys[i] = node->keys[i - 1];
        }
        Node *l_node = node->children[index];
        Node *r_node = new Node(l_node->leaf);
        node->keys[index] = l_node->keys[t - 1];
        node->children[index + 1] = r_node;
        for (int i = t; i < 2 * t - 1; i++) {
            r_node->keys.push_back(l_node->keys[i]);
        }
        l_node->keys.resize(t - 1);
        if (!l_node->leaf) {
            for (int i = t; i < 2 * t; i++) {
                r_node->children.push_back(l_node->children[i]);
            }
            l_node->children.resize(t);
        }
    }

    // вставить ключ key в гарантированно не переполненную ноду node
    void insertNonFull(Node *node, const T &key) {
        int pos = node->keys.size() - 1;

        // гарантированно не перепеполненный лист -- запишем новый ключ в него
        if (node->leaf) {
            // расширили вектор ключей для вставки нового
            node->keys.resize(node->keys.size() + 1);
            while (pos >= 0 && cmp(key, node->keys[pos])) {
                // обходим ключи справа налево, сдвигая вправо на 1
                node->keys[pos + 1] = node->keys[pos];
                pos--;
            }
            // вставляем новый ключ на освобожденное в цикле место
            node->keys[pos + 1] = key;
        }
            // не лист, значит есть потомки, пишем в один из них
        else {
            // ищем позицию потомка, в которого добавим ключ
            while (pos >= 0 && cmp(key, node->keys[pos])) {
                pos--;
            }

            // если потомок и так полон, надо его сначала разбить
            if (isNodeFull(node->children[pos + 1])) {
                splitChild(node, pos + 1);
                // после разбиения потомка в текущий узел из него поднялся ключ
                // надо сравниться и с ним
                if (key > node->keys[pos + 1])
                    pos++;
            }
            insertNonFull(node->children[pos + 1], key);
        }
    }

    int t;
    Node *root;
    Comparator cmp;
    friend void test1();
};

template<typename T>
class Comparator {
public:
    bool operator()(const T &l, const T &r) const {
        return l < r;
    }

};

void test1()
{
    BTree<char> tree(3);

    tree.root = new BTree<char>::Node(false);
    tree.root->keys = {'G', 'M', 'P', 'X'};

    {
        auto child = new BTree<char>::Node(true);
        child->keys = {'A', 'C', 'D', 'E'};
        tree.root->children.push_back(child);
    }

    {
        auto child = new BTree<char>::Node(true);
        child->keys = {'J', 'K'};
        tree.root->children.push_back(child);
    }
    {
        auto child = new BTree<char>::Node(true);
        child->keys = {'N', 'O'};
        tree.root->children.push_back(child);
    }
    {
        auto child = new BTree<char>::Node(true);
        child->keys = {'R', 'S', 'T', 'U', 'V'};
        tree.root->children.push_back(child);
    }
    {
        auto child = new BTree<char>::Node(true);
        child->keys = {'Y', 'Z'};
        tree.root->children.push_back(child);
    }

    std::cout << "Initial tree:" << std::endl;
    tree.debugPrint();
    std::cout << std::endl;

    std::string insertKeys = "BQLF";
    // посимвольно добавляем в дерево ключи
    for (auto c: insertKeys)
    {
        tree.Insert(c);
        std::cout << "After inserting " << c << ":" << std::endl;
        tree.debugPrint();
        std::cout << std::endl;
    }
}

int main(int argc, const char *argv[]) {
    //test1();
    std::ifstream file("../input3_1.txt");
    int n, num;
    file >> n;
    BTree<int, Comparator<int>> tree(n);
    while (file >> num) {
        tree.Insert(num);
    }
    tree.Print();
    return 0;
}
