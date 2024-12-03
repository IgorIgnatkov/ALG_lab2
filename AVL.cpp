#include <iostream>
#include <algorithm> // Для std::max
#include <queue> // Для обхода в ширину
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

class Node {
public:
    int value;      // Значение узла
    Node* left;     // Указатель на левого потомка
    Node* right;    // Указатель на правого потомка
    int height;     // Высота узла

    // Конструктор
    Node(int val) : value(val), left(nullptr), right(nullptr), height(1) {}

    // Метод для нахождения высоты узла
    int getHeight() {
        return height;
    }

    // Метод для обновления высоты узла
    void updateHeight() {
        height = 1 + std::max(left ? left->getHeight() : 0, right ? right->getHeight() : 0);
    }

    // Метод для получения баланса узла
    int getBalance() {
        return (left ? left->getHeight() : 0) - (right ? right->getHeight() : 0);
    }

    // Правый поворот
    Node* rightRotate() {
        Node* newRoot = left; // Новый корень — левый потомок
        Node* temp = newRoot->right; // Сохраняем правое поддерево нового корня

        newRoot->right = this; // Перемещаем текущий узел вправо
        left = temp; // Перемещаем правое поддерево нового корня в левое поддерево текущего узла

        // Обновляем высоты
        updateHeight();
        newRoot->updateHeight();

        return newRoot; // Возвращаем новый корень
    }

    // Левый поворот
    Node* leftRotate() {
        Node* newRoot = right; // Новый корень — правый потомок
        Node* temp = newRoot->left; // Сохраняем левое поддерево нового корня

        newRoot->left = this; // Перемещаем текущий узел влево
        right = temp; // Перемещаем левое поддерево нового корня в правое поддерево текущего узла

        // Обновляем высоты
        updateHeight();
        newRoot->updateHeight();

        return newRoot; // Возвращаем новый корень
    }

    // Метод для вставки значения в AVL-дерево
    Node* insert(int val) {
        if (val < value) {
            // Если значение меньше, идем в левое поддерево
            left = left ? left->insert(val) : new Node(val);
        }
        else {
            // Если значение больше или равно, идем в правое поддерево
            right = right ? right->insert(val) : new Node(val);
        }

        // Обновляем высоту узла
        updateHeight();

        // Балансируем дерево
        int balance = getBalance();

        // Левый левый случай
        if (balance > 1 && val < left->value) {
            return rightRotate();
        }

        // Правый правый случай
        if (balance < -1 && val > right->value) {
            return leftRotate();
        }

        // Левый правый случай
        if (balance > 1 && val > left->value) {
            left = left->leftRotate();
            return rightRotate();
        }

        // Правый левый случай
        if (balance < -1 && val < right->value) {
            right = right->rightRotate();
            return leftRotate();
        }

        return this; // Возвращаем текущий узел
    }

    // Метод для поиска значения в AVL-дереве
    Node* search(int val) {
        if (val == value) {
            return this; // Значение найдено
        }
        else if (val < value) {
            return left ? left->search(val) : nullptr; // Рекурсивный поиск в левом поддереве
        }
        else {
            return right ? right->search(val) : nullptr; // Рекурсивный поиск в правом поддереве
        }
    }

    // Метод для нахождения минимального узла в дереве
    Node* findMin() {
        Node* current = this;
        while (current && current->left) {
            current = current->left; // Идем в левое поддерево
        }
        return current; // Возвращаем узел с минимальным значением
    }

    // Метод для удаления узла из AVL-дерева
    Node* remove(int val) {
        if (val < value) {
            // Если значение меньше, идем в левое поддерево
            if (left) {
                left = left->remove(val);
            }
        }
        else if (val > value) {
            // Если значение больше, идем в правое поддерево
            if (right) {
                right = right->remove(val);
            }
        }
        else {
            // Узел найден
            if (!left && !right) {
                // Случай 1: Узел не имеет потомков
                delete this;
                return nullptr;
            }
            else if (!left) {
                // Случай 2: Узел имеет только правого потомка
                Node* temp = right;
                delete this;
                return temp;
            }
            else if (!right) {
                // Случай 2: Узел имеет только левого потомка
                Node* temp = left;
                delete this;
                return temp;
            }
            else {
                // Случай 3: Узел имеет двух потомков
                Node* minNode = right->findMin();
                value = minNode->value;
                right = right->remove(minNode->value);
            }
        }

        // Обновляем высоту узла
        updateHeight();

        // Балансируем дерево
        int balance = getBalance();

        // Левый левый случай
        if (balance > 1 && left->getBalance() >= 0) {
            return rightRotate();
        }

        // Левый правый случай
        if (balance > 1 && left->getBalance() < 0) {
            left = left->leftRotate();
            return rightRotate();
        }

        // Правый правый случай
        if (balance < -1 && right->getBalance() <= 0) {
            return leftRotate();
        }

        // Правый левый случай
        if (balance < -1 && right->getBalance() > 0) {
            right = right->rightRotate();
            return leftRotate();
        }

        return this; // Возвращаем текущий узел
    }

    // Метод для вывода узла
    void print() {
        std::cout << "Node(" << value << ", height=" << height << ")" << std::endl;
    }

    // Префиксный обход (NLR)
    void preorder() {
        print(); // Выводим текущий узел
        if (left) left->preorder(); // Рекурсивно обходим левое поддерево
        if (right) right->preorder(); // Рекурсивно обходим правое поддерево
    }

    // Симметричный обход
    void inorder() {
        if (left) left->inorder(); // Рекурсивно обходим левое поддерево
        print(); // Выводим текущий узел
        if (right) right->inorder(); // Рекурсивно обходим правое поддерево
    }

    // Постфиксный обход (LRN)
    void postorder() {
        if (left) left->postorder(); // Рекурсивно обходим левое поддерево
        if (right) right->postorder(); // Рекурсивно обходим правое поддерево
        print(); // Выводим текущий узел
    }

    // Обход в ширину
    void levelOrder() {
        std::queue<Node*> q; // Создаем очередь для хранения узлов
        q.push(this); // Добавляем корень в очередь

        while (!q.empty()) {
            Node* current = q.front(); // Получаем узел из начала очереди
            q.pop(); // Удаляем узел из очереди
            current->print(); // Выводим текущий узел

            if (current->left) q.push(current->left); // Добавляем левого потомка в очередь
            if (current->right) q.push(current->right); // Добавляем правого потомка в очередь
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
    std::cout << "Высота дерева: " << root->getHeight() << std::endl;

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
    std::cout << "Высота дерева после удаления: " << root->getHeight() << std::endl;

    // Обходы
    std::cout << "Префиксный обход:" << std::endl;
    root->preorder();

    std::cout << "Симметричный обход:" << std::endl;
    root->inorder();

    std::cout << "Постфиксный обход:" << std::endl;
    root->postorder();

    std::cout << "Обход в ширину:" << std::endl;
    root->levelOrder();

    srand(time(0)); // Инициализация генератора случайных чисел

    std::vector<int> n_values = { 10000, 20000, 30000, 40000, 50000 }; // Различные значения n

    std::ofstream outputFile("tree_heights_AVL.txt"); // Открываем файл для записи результатов
    if (!outputFile) {
        std::cerr << "Ошибка открытия файла для записи результатов." << std::endl;
        return 1;
    }

    for (int n : n_values) {
        Node* root = new Node(rand() % 100000); // Создаем корень с случайным значением

        for (int i = 1; i < n; ++i) {
            root->insert(rand() % 100000); // Вставляем случайные значения
            int tree_height = root->getHeight(); // Измеряем высоту дерева
            std::cout << "n = " << i + 1 << ", height = " << tree_height << std::endl;
            outputFile << "n = " << i + 1 << ", height = " << tree_height << std::endl;
        }

        delete root; // Освобождаем память
    }

    outputFile.close(); // Закрываем файл
    return 0;
}
