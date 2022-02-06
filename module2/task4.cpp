/*В одной военной части решили построить в одну шеренгу по росту.
 * Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя,
 * а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
 * Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены
 * по росту – сначала самые высокие, а в конце – самые низкие. За расстановку
 * солдат отвечал прапорщик, который заметил интересную особенность – все солдаты
 * в части разного роста. Ваша задача состоит в том, чтобы помочь прапорщику
 * правильно расставлять солдат, а именно для каждого приходящего солдата указывать,
 * перед каким солдатом в строе он должен становится. Требуемая скорость выполнения
 * команды - O(log n).
*/
#include <iostream>
#include <fstream>
#include <stack>

template <typename T, typename Comparator>
class AvlTree
{
    struct Node
    {
        Node(const T &key)
                : key(key), height(1), count(1), left(nullptr), right(nullptr)
        {
        }

        T key;
        size_t height;
        int count;
        Node *left;
        Node *right;
    };
public:
    AvlTree()
            : root(nullptr)
    {
    }

    ~AvlTree()
    {
        destroyTree(root);
    }
    void Add(const T &key)
    {
        root = addInternal(root, key);
    }

    int Kstat(const T &key){
        Node *tmp = root;
        int pos = 0;
        while (tmp)
        {
            if (cmp.eq(tmp->key, key))
            {
                pos += getCount(tmp->left);
                return pos;
            }
            else if (cmp(tmp->key, key))
            {
                pos += getCount(tmp->left) + 1;
                tmp = tmp->right;
            }
            else
            {
                tmp = tmp->left;
            }
        }
        return -1;
    }

    void RemoveByKstat(const int &kstat){
        if (kstat > root->count - 1 || !root){
            return;
        }
        Node *tmp = root;
        int pos = 0;
        while (!(pos + getCount(tmp->left) == kstat))
        {
            if (pos + getCount(tmp->left) + 1 > kstat)
            {
                tmp = tmp->left;
            }
            else
            {
                pos += getCount(tmp->left) + 1;
                tmp = tmp->right;
            }
        }
        root = removeInternal(root, tmp->key);
    }

    bool Has(const T &key)
    {
        Node *tmp = root;
        while (tmp)
        {
            if (cmp.eq(tmp->key, key))
            {
                return true;
            }
            else if (cmp(tmp->key, key))
            {
                tmp = tmp->right;
            }
            else
            {
                tmp = tmp->left;
            }
        }
        return false;
    }

    void Remove(const T &key)
    {
        root = removeInternal(root, key);
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
            std::cout << tmp->key << "(" << tmp->count << ") ";
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
    void destroyTree(Node *node)
    {
        if (node)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    Node *addInternal(Node *node, const T &key)
    {
        if (!node)
            return new Node(key);
        if (cmp(node->key, key))
        {
            node->right = addInternal(node->right, key);
        }
        else
        {
            node->left = addInternal(node->left, key);
        }
        return doBalance(node);
    }

    Node *removeInternal(Node *node, const T &key)
    {
        if (!node)
            return nullptr;
        if (cmp(node->key, key))
            node->right = removeInternal(node->right, key);
        else if (!cmp(node->key, key) && !cmp.eq(node->key, key))
            node->left = removeInternal(node->left, key);
        else
        {
            Node *left = node->left;
            Node *right = node->right;

            delete node;

            if (!right)
                return left;
            if (!left)
                return right;

            Node *new_node;
            if (left->height > right->height) {
                auto pair = findRemoveMin(right);
                new_node = pair.first;
                new_node->right = pair.second;
                new_node->left = left;
            }else{
                auto pair = findRemoveMax(left);
                new_node = pair.first;
                new_node->left = pair.second;
                new_node->right = right;
            }
            return doBalance(new_node);

        }
        return doBalance(node);
    }

    std::pair<Node*, Node*> findRemoveMin(Node *node){
        if (!node->left)
            return std::pair<Node*, Node*> (node, node->right);
        auto pair = findRemoveMin(node->left);
        node->left = pair.second;
        pair.second = doBalance(node);
        return pair;
    }

    std::pair<Node*, Node*> findRemoveMax(Node *node){
        if (!node->right)
            return std::pair<Node*, Node*> (node, node->left);
        auto pair = findRemoveMax(node->right);
        node->right = pair.second;
        pair.second = doBalance(node);
        return pair;
    }

    Node *findMin(Node *node)
    {
        while (node->left)
            node = node->left;
        return node;
    }

    Node *removeMin(Node *node)
    {
        if (!node->left)
            return node->right;
        node->left = removeMin(node->left);
        return doBalance(node);
    }

    size_t getHeight(Node *node)
    {
        return node ? node->height : 0;
    }

    void fixHeight(Node *node)
    {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    size_t getCount(Node *node)
    {
        return node ? node->count : 0;
    }

    void fixCount(Node *node)
    {
        node->count = getCount(node->left) + getCount(node->right) + 1;
    }

    int getBalance(Node *node)
    {
        return getHeight(node->right) - getHeight(node->left);
    }

    Node *doBalance(Node *node)
    {
        fixHeight(node);
        fixCount(node);

        switch (getBalance(node))
        {
            case 2:
            {
                if (getBalance(node->right) < 0)
                    node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            case -2:
            {
                if (getBalance(node->left) > 0)
                    node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
            default:
                return node;
        }
    }

    Node *rotateLeft(Node *node)
    {
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        fixHeight(node);
        fixHeight(tmp);
        fixCount(node);
        fixCount(tmp);
        return tmp;
    }

    Node *rotateRight(Node *node)
    {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixHeight(node);
        fixHeight(tmp);
        fixCount(node);
        fixCount(tmp);
        return tmp;
    }

    Node *root;

    Comparator cmp;
};

template<typename T>
class Comparator {
public:
    bool operator()(const T &l, const T &r) const {
        return l > r;
    }

    bool eq(const T &l, const T &r) {
        return l == r;
    }
};

int main(int argc, const char * argv[]) {
    AvlTree<int, Comparator<int>> tree;
    std::ifstream file("../input4_2.txt");
    int n, command, desc;
    file >> n;
    for (int i = 0; i < n; i++){
        file >> command >> desc;
        switch (command) {
            case 1:
            {
                tree.Add(desc);
                std::cout << tree.Kstat(desc) << std::endl;
                break;
            }
            case 2:
            {
                tree.RemoveByKstat(desc);
                break;
            }
        }
    }
    return 0;
}
