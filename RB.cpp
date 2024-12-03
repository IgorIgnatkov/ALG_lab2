#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

enum Color { RED, BLACK };

class Node {
public:
    int value;
    bool color;
    Node* left, * right, * parent;

    // Конструктор
    Node(int val) : value(val), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}

    // Метод для вывода узла
    void print() {
        std::cout << "Node(" << value << ", color=" << (color == RED ? "RED" : "BLACK") << ")" << std::endl;
    }

    // Симметричный обход
    void inorder() {
        if (left) left->inorder();
        print();
        if (right) right->inorder();
    }

    // Префиксный обход (NLR)
    void preorder() {
        print(); // Выводим текущий узел
        if (left) left->preorder(); // Рекурсивно обходим левое поддерево
        if (right) right->preorder(); // Рекурсивно обходим правое поддерево
    }

    // Постфиксный обход (LRN)
    void postorder() {
        if (left) left->postorder(); // Рекурсивно обходим левое поддерево
        if (right) right->postorder(); // Рекурсивно обходим правое поддерево
        print(); // Выводим текущий узел
    }

    // Обход в ширину
    void levelOrder() {
        std::queue<Node*> q;
        q.push(this);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            current->print();

            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
    }
};

class RedBlackTree {
private:
    Node* root;
    Node* TNULL;

    // Вспомогательные функции для вращений
    void initializeNULLNode(Node* node, Node* parent) {
        node->value = 0;
        node->color = BLACK;
        node->left = nullptr;
        node->right = nullptr;
        node->parent = parent;
    }

    void leftRotate(Node*& pt, Node*& ppt) {
        Node* y = pt->right;
        pt->right = y->left;
        if (y->left != TNULL) {
            y->left->parent = pt;
        }
        y->parent = ppt;
        if (pt == root) {
            root = y;
        }
        else if (pt == ppt->left) {
            ppt->left = y;
        }
        else {
            ppt->right = y;
        }
        y->left = pt;
        pt->parent = y;
    }

    void rightRotate(Node*& pt, Node*& ppt) {
        Node* y = pt->left;
        pt->left = y->right;
        if (y->right != TNULL) {
            y->right->parent = pt;
        }
        y->parent = ppt;
        if (pt == root) {
            root = y;
        }
        else if (pt == ppt->left) {
            ppt->left = y;
        }
        else {
            ppt->right = y;
        }
        y->right = pt;
        pt->parent = y;
    }

    void fixInsert(Node*& pt) {
        Node* parent_pt = nullptr;
        Node* grand_parent_pt = nullptr;

        while ((pt != root) && (pt->color != BLACK) && (pt->parent->color == RED)) {
            parent_pt = pt->parent;
            grand_parent_pt = pt->parent->parent;

            if (parent_pt == grand_parent_pt->left) {
                Node* uncle_pt = grand_parent_pt->right;
                if (uncle_pt != TNULL && uncle_pt->color == RED) {
                    grand_parent_pt->color = RED;
                    parent_pt->color = BLACK;
                    uncle_pt->color = BLACK;
                    pt = grand_parent_pt;
                }
                else {
                    if (pt == parent_pt->right) {
                        leftRotate(parent_pt, grand_parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rightRotate(grand_parent_pt, grand_parent_pt->parent);
                    std::swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            }
            else {
                Node* uncle_pt = grand_parent_pt->left;
                if ((uncle_pt != TNULL) && (uncle_pt->color == RED)) {
                    grand_parent_pt->color = RED;
                    parent_pt->color = BLACK;
                    uncle_pt->color = BLACK;
                    pt = grand_parent_pt;
                }
                else {
                    if (pt == parent_pt->left) {
                        rightRotate(parent_pt, grand_parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    leftRotate(grand_parent_pt, grand_parent_pt->parent);
                    std::swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            }
        }
        root->color = BLACK;
    }

    void fixDelete(Node* x) {
        Node* s;
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                s = x->parent->right;
                if (s->color == RED) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent, x->parent->parent);
                    s = x->parent->right;
                }
                if (s->right->color == BLACK && s->left->color == BLACK) {
                    s->color = RED;
                    x = x->parent;
                }
                else {
                    if (s->right->color == BLACK) {
                        s->left->color = BLACK;
                        s->color = RED;
                        rightRotate(s, s->parent);
                        s = x->parent->right;
                    }
                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->right->color = BLACK;
                    leftRotate(x->parent, x->parent->parent);
                    x = root;
                }
            }
            else {
                s = x->parent->left;
                if (s->color == RED) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent, x->parent->parent);
                    s = x->parent->left;
                }
                if (s->right->color == BLACK) {
                    s->color = RED;
                    x = x->parent;
                }
                else {
                    if (s->left->color == BLACK) {
                        s->right->color = BLACK;
                        s->color = RED;
                        leftRotate(s, s->parent);
                        s = x->parent->left;
                    }
                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->left->color = BLACK;
                    rightRotate(x->parent, x->parent->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    void rbTransplant(Node* u, Node* v) {
        if (u->parent == nullptr) {
            root = v;
        }
        else if (u == u->parent->left) {
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void deleteNodeHelper(Node* node, int key) {
        Node* z = TNULL;
        Node* x, * y;
        while (node != TNULL) {
            if (node->value == key) {
                z = node;
            }
            if (node->value <= key) {
                node = node->right;
            }
            else {
                node = node->left;
            }
        }
        if (z == TNULL) {
            std::cout << "Key not found in the tree" << std::endl;
            return;
        }
        y = z;
        int y_original_color = y->color;
        if (z->left == TNULL) {
            x = z->right;
            rbTransplant(z, z->right);
        }
        else if (z->right == TNULL) {
            x = z->left;
            rbTransplant(z, z->left);
        }
        else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            }
            else {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (y_original_color == BLACK) {
            fixDelete(x);
        }
    }

    Node* minimum(Node* node) {
        while (node->left != TNULL) {
            node = node->left;
        }
        return node;
    }

public:
    RedBlackTree() {
        TNULL = new Node(0);
        TNULL->color = BLACK;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        root = TNULL;
    }

    ~RedBlackTree() {
        deleteTree(root);
        delete TNULL;
    }

    void insert(const int& key) {
        Node* pt = new Node(key);
        pt->parent = nullptr;
        pt->value = key;
        pt->left = TNULL;
        pt->right = TNULL;
        pt->color = RED;

        Node* y = nullptr;
        Node* x = this->root;

        while (x != TNULL) {
            y = x;
            if (pt->value < x->value) {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }

        pt->parent = y;
        if (y == nullptr) {
            root = pt;
        }
        else if (pt->value < y->value) {
            y->left = pt;
        }
        else {
            y->right = pt;
        }

        if (pt->parent == nullptr) {
            pt->color = BLACK;
            return;
        }

        if (pt->parent->parent == nullptr) {
            return;
        }

        fixInsert(pt);
    }

    void deleteNode(int value) {
        deleteNodeHelper(this->root, value);
    }

    Node* search(int value) {
        Node* current = root;
        while (current != TNULL) {
            if (value == current->value) {
                return current;
            }
            else if (value < current->value) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        return nullptr;
    }

    void inorder() {
        if (root) root->inorder();
    }

    void preorder() {
        if (root) root->inorder();
    }

    void postorder() {
        if (root) root->inorder();
    }

    void levelOrder() {
        if (root) root->levelOrder();
    }

    int getHeight(Node* node) {
        if (node == TNULL) {
            return 0;
        }
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);
        return std::max(leftHeight, rightHeight) + 1;
    }

    int getHeight() {
        return getHeight(root);
    }

private:
    void deleteTree(Node* node) {
        if (node && node != TNULL) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
};

int main() {
    setlocale(LC_ALL, "Ru");

    RedBlackTree rbTree;

    // Вставка узлов
    rbTree.insert(10);
    rbTree.insert(5);
    rbTree.insert(15);
    rbTree.insert(3);
    rbTree.insert(7);
    rbTree.insert(12);
    rbTree.insert(18);

    std::cout << "Структура дерева:" << std::endl;
    rbTree.inorder();

    std::cout << "Поиск значений:" << std::endl;
    int searchValues[] = { 7, 12, 20 };
    for (int val : searchValues) {
        if (rbTree.search(val)) {
            std::cout << "Значение " << val << " найдено." << std::endl;
        }
        else {
            std::cout << "Значение " << val << " не найдено." << std::endl;
        }
    }

    rbTree.deleteNode(5);
    rbTree.deleteNode(15);

    std::cout << "Структура дерева после удаления:" << std::endl;
    rbTree.inorder();

    // Обходы
    std::cout << "Префиксный обход:" << std::endl;
    rbTree.preorder();

    std::cout << "Симметричный обход:" << std::endl;
    rbTree.inorder();

    std::cout << "Постфиксный обход:" << std::endl;
    rbTree.postorder();

    std::cout << "Обход в ширину:" << std::endl;
    rbTree.levelOrder();

    srand(time(0)); // Инициализация генератора случайных чисел

    std::vector<int> n_values = { 10000, 20000, 30000, 40000, 50000 }; // Различные значения n

    std::ofstream outputFile("tree_heights_RB.txt"); // Открываем файл для записи результатов
    if (!outputFile) {
        std::cerr << "Ошибка открытия файла для записи результатов." << std::endl;
        return 1;
    }

    for (int n : n_values) {
        RedBlackTree rbTree;
        for (int i = 0; i < n; ++i) {
            rbTree.insert(rand() % 1000000); // Вставляем случайные значения
            int tree_height = rbTree.getHeight(); // Измеряем высоту дерева
            
            std::cout << "n = " << i + 1 << ", height = " << tree_height << std::endl;
            outputFile << "n = " << i + 1 << ", height = " << tree_height << std::endl;
        }
        
    }

    outputFile.close(); // Закрываем файл

    return 0;
}
