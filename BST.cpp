#include <iostream>
#include <algorithm>
#include <queue>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

class Node {
public:
    int value;
    Node* left;
    Node* right;

    Node(int val) : value(val), left(nullptr), right(nullptr) {}

    void print() {
        std::cout << "Node(" << value << ")" << std::endl;
    }

    void insert(int val) {
        if (val < value) {
            if (left == nullptr) {
                left = new Node(val);
            }
            else {
                left->insert(val);
            }
        }
        else {
            if (right == nullptr) {
                right = new Node(val);
            }
            else {
                right->insert(val);
            }
        }
    }

    Node* search(int val) {
        if (val == value) {
            return this;
        }
        else if (val < value) {
            return left ? left->search(val) : nullptr;
        }
        else {
            return right ? right->search(val) : nullptr;
        }
    }

    Node* findMin() {
        Node* current = this;
        while (current && current->left) {
            current = current->left;
        }
        return current;
    }

    Node* remove(int val) {
        if (val < value) {
            if (left) {
                left = left->remove(val);
            }
        }
        else if (val > value) {
            if (right) {
                right = right->remove(val);
            }
        }
        else {
            if (!left && !right) {
                delete this;
                return nullptr;
            }
            else if (!left) {
                Node* temp = right;
                delete this;
                return temp;
            }
            else if (!right) {
                Node* temp = left;
                delete this;
                return temp;
            }
            else {
                Node* minNode = right->findMin();
                value = minNode->value;
                right = right->remove(minNode->value);
            }
        }
        return this;
    }

    int height() {
        if (this == nullptr) {
            return 0;
        }
        int leftHeight = left ? left->height() : 0;
        int rightHeight = right ? right->height() : 0;
        return std::max(leftHeight, rightHeight) + 1;
    }

    void preorder() {
        print();
        if (left) left->preorder();
        if (right) right->preorder();
    }

    void inorder() {
        if (left) left->inorder();
        print();
        if (right) right->inorder();
    }

    void postorder() {
        if (left) left->postorder();
        if (right) right->postorder();
        print();
    }

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

int main() {
    setlocale(LC_ALL, "Ru");

    // Создаем корень дерева
    Node* root = new Node(10);

    // Вставляем значения в дерево
    root->insert(5);
    root->insert(15);
    root->insert(3);
    root->insert(7);
    root->insert(12);
    root->insert(18);

    // Выводим структуру дерева
    std::cout << "Структура дерева:" << std::endl;
    root->inorder();

    // Выводим высоту дерева
    std::cout << "Высота дерева: " << root->height() << std::endl;

    // Поиск значений
    int searchValues[] = { 7, 12, 20 };
    for (int val : searchValues) {
        Node* result = root->search(val);
        if (result) {
            std::cout << "Значение " << val << " найдено." << std::endl;
        }
        else {
            std::cout << "Значение " << val << " не найдено." << std::endl;
        }
    }

    // Удаление узлов
    root = root->remove(5);
    root = root->remove(15);

    // Выводим структуру дерева после удаления
    std::cout << "Структура дерева после удаления:" << std::endl;
    root->inorder();

    // Выводим высоту дерева после удаления
    std::cout << "Высота дерева после удаления: " << root->height() << std::endl;

    // Обходы
    std::cout << "Предзаказный обход:" << std::endl;
    root->preorder();

    std::cout << "Симметричный обход:" << std::endl;
    root->inorder();

    std::cout << "Постзаказный обход:" << std::endl;
    root->postorder();

    std::cout << "Обход в ширину:" << std::endl;
    root->levelOrder();

    srand(time(0)); // Инициализация генератора случайных чисел

    std::vector<int> n_values = { 10000, 20000, 30000, 40000, 50000 }; // Различные значения n

    std::ofstream outputFile("tree_heights.txt"); // Открываем файл для записи результатов
    if (!outputFile) {
        std::cerr << "Ошибка открытия файла для записи результатов." << std::endl;
        return 1;
    }

    for (int n : n_values) {
        Node* root = new Node(rand() % 50000); // Создаем корень с случайным значением

        for (int i = 1; i < n; ++i) {
            root->insert(rand() % 50000); // Вставляем случайные значения
            int tree_height = root->height(); // Измеряем высоту дерева
            outputFile << "n = " << i + 1 << ", height = " << tree_height << std::endl;
        }

        delete root; // Освобождаем память
    }

    outputFile.close(); // Закрываем файл
    return 0;
}
