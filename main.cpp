#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>

using namespace std;

const string filename = "tree.txt";
const string filename2 = "tree2.txt";

class Node {
public:
    int value;
    Node* left;
    Node* right;
    Node(int value) {
        this->value = value;
        left = nullptr;
        right = nullptr;
    }
};

Node* insert(Node* node, int value) {
    if (!node)
        return new Node(value);
    if (value < node->value) 
        node->left = insert(node->left, value);
    else if (value > node->value)
        node->right = insert(node->right, value);
    return node;
}

template <typename T>
T* search(T* node, int value) {
    if (node == nullptr) return nullptr;
    if (node->value == value) return node;
    else if (value < node->value) return search<T>(node->left, value);
    else if (value > node->value) return search<T>(node->right, value);
}

Node* getMin(Node* node) {
    if (node == nullptr) {
        return nullptr;
    }
    if (node->left == nullptr)
        return node;
    return getMin(node->left);
}

template<typename t>
t* getMax(t* node) {
    if (node == nullptr)
        return nullptr;
    if (node->right == nullptr) {
        return node;
    }
    return getMax(node->right);
}

Node* deleteNode(Node* node) {
    int value = node->value;
    if (node == nullptr)
        return nullptr;
    if (node->left == nullptr && node->right == nullptr)
        return nullptr;
    if (node->left == nullptr)
        return node->right;
    if (node->right == nullptr)
        return node->left;
    Node* maxInLeft = getMax(node->left);
    node->value = maxInLeft->value;
    deleteNode(maxInLeft);
    return node;
}

template <typename T>
void InOrderTraversal(T* node) { // симметричный
    if (node) {
        InOrderTraversal(node->left);
        cout << node->value << " ";
        InOrderTraversal(node->right);
    }
} 

template <typename T>
void PostOrderTraversal(T* node) { // обратный
    if (node == nullptr)
        return;
    PostOrderTraversal(node->left);
    PostOrderTraversal(node->right);
    cout << node->value << " ";
} 

template <typename T>
void PreOrderTraversal(T* node) { // прямой
    if (node == nullptr)
        return;
    cout << node->value << " ";
    PreOrderTraversal(node->left);
    PreOrderTraversal(node->right);
}

template <typename T>
void printTree(T* node, const string& prefix = "", bool isRight = true) {
    if (node != nullptr) {
        string currentPrefix = prefix;
        if (isRight) {
            currentPrefix += "    ";
        }
        else {
            currentPrefix += "│   ";
        }
        printTree<T>(node->right, currentPrefix, true);
        currentPrefix = prefix;
        cout << prefix;
        if (isRight) {
            cout << ".-->";
        }
        else {
            cout << "`-->";
        }
        cout << node->value << std::endl;

        printTree<T>(node->left, prefix + (isRight ? "    " : "│   "), false);
    }
}

template <typename T>
T* parseTree(const string& str, size_t& pos) {
    while (pos < str.size() && isspace(str[pos])) {
        pos++;
    }

    if (pos >= str.size() || str[pos] != '(') {
        cout << "Неверный формат дерева: отсутствует открывающая скобка.";
        exit(0);
    }
    pos++; // пропуск '('

    string valueStr;
    if (pos < str.size() && str[pos] == '-') {
        valueStr += str[pos++];
    }
    while (pos < str.size() && isdigit(str[pos])) {
        valueStr += str[pos++];
    }
    if (valueStr.empty()) {
        cout << "Неверный формат дерева: отсутствует значение узла.";
        exit(0);
    }
    int value = stoi(valueStr);
    T* node = new T(value);

    while (pos < str.size() && isspace(str[pos])) {
        pos++;
    }

    // левое поддерево
    if (pos < str.size() && str[pos] == '(') {
        node->left = parseTree<T>(str, pos);
    }
    while (pos < str.size() && isspace(str[pos])) {
        pos++;
    }

    // правое поддерево
    if (pos < str.size() && str[pos] == '(') {
        node->right = parseTree<T>(str, pos);
    }
    while (pos < str.size() && isspace(str[pos])) {
        pos++;
    }

    if (pos >= str.size() || str[pos] != ')') {
        cout << ("Неверный формат дерева: отсутствует закрывающая скобка.");
        exit(0);
    }
    pos++; // пропуск ')'

    return node;
}

template <typename T>
T* readTreeFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Не удалось открыть файл.";
        exit(0);
    }

    string treeStr;
    getline(file, treeStr);
    file.close();

    size_t pos = 0;
    T* root = parseTree<T>(treeStr, pos);

    if (pos != treeStr.size()) {
        cout << "Неверный формат дерева: лишние символы в конце.";
        exit(0);
    }

    return root;
}

///// upper - binary
//
////////////////////
//
///// lower  -  AVL

class AvlNode {
public:
    int value;
    AvlNode* left;
    AvlNode* right;
    int height;
    AvlNode(int val) {
        value = val;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

int getHeight(AvlNode* node) {
    return node ? node->height : 0;
}

int balanceFactor(AvlNode* node) {
    return getHeight(node->left) - getHeight(node->right);
}

void updateHeight(AvlNode* node) {
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
}

AvlNode* rotateRight(AvlNode* y) {
    AvlNode* x = y->left;
    AvlNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    updateHeight(y);
    updateHeight(x);

    return x;
}

AvlNode* rotateLeft(AvlNode* x) {
    AvlNode* y = x->right;
    AvlNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    updateHeight(x);
    updateHeight(y);

    return y;
}

AvlNode* balance(AvlNode* node) {
    updateHeight(node);
    if (balanceFactor(node) > 1) {
        if (balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }

    if (balanceFactor(node) < -1) {
        if (balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }

    return node;
}

AvlNode* insert(AvlNode* node, int value) {
    if (!node) {
        return new AvlNode(value);
    }
    if (value < node->value) {
        node->left = insert(node->left, value);
    }
    else if (value > node->value) {
        node->right = insert(node->right, value);
    }
    else {
        cout << "\nТакой элемент уже существует!\n";
        return node;
    }
    return balance(node);
}

AvlNode* deleteAvlNode(AvlNode* node, int value) {
    if (node == nullptr) return nullptr;
    if (value < node->value)
        node->left = deleteAvlNode(node->left, value);
    else if (value > node->value)
        node->right = deleteAvlNode(node->right, value);
    else {
        if (node->left == nullptr || node->right == nullptr) {
            AvlNode* temp = node->left ? node->left : node->right;
            delete node;
            return temp;
        }
        else {
            AvlNode* maxInLeft = getMax(node->left);
            node->value = maxInLeft->value;
            node->left = deleteAvlNode(node->left, maxInLeft->value);
        }
    }
    if (node == nullptr)
        return node;
    updateHeight(node);
    return balance(node);
}

template <typename T>
void writeTree(T* node) {
    cout << "симметричный обход: ";
    InOrderTraversal(node);
    cout << "\nобратный обход: ";
    PostOrderTraversal(node);
    cout << "\nпрямой обход: ";
    PreOrderTraversal(node);
    cout << "\n\n";
    printTree(node);
    cout << "\n\n";
}

void menu(AvlNode* node) {
    AvlNode* elem = nullptr;
    printTree(node);
    cout << endl;
    short int choice;
    cout << "1. Вставка элемента в АВЛ дерево\n";
    cout << "2. Удаление элемента в АВЛ дереве\n";
    cout << "3. Поиск элемента в АВЛ дереве\n";
    cin >> choice;
    switch (choice) {
    case 1:
        int value;
        cout << "\nВведите значение нового элемента: ";
        cin >> value;
        node = insert(node, value);
        cout << "\nОбновленное дерево:";
        cout << endl;
        break;
    case 2:
        cout << "\nВведите значение удаляемого элемента: ";
        cin >> value;
        node = deleteAvlNode(node, value);
        cout << "\nОбновленное дерево:";
        cout << endl;
        break;
    case 3:
        cout << "Введите значение элемента: ";
        cin >> value;
        cout << endl;
        elem = search<AvlNode>(node, value);
        system("cls");
        (elem) ? cout << "Элемент найден!" : cout << "Такого элемента нет";
        _getch();
        cout << endl;
        break;
    default:
        cout << "error!";
    }

    menu(node);
}

int main() {
    AvlNode* avlTree = nullptr;
    for (int i = 0; i < 20; ++i) {
        avlTree = insert(avlTree, rand() % 49 + 1);
    }
    system("cls");
    Node* binaryTree = readTreeFromFile<Node>(filename);
    cout << "Бинарное дерево" << endl;
    writeTree(binaryTree);
    //////////////////////
    cout << "АВЛ дерево" << endl;
    cout << "симметричный обход: ";
    InOrderTraversal(avlTree);
    cout << "\nобратный обход: ";
    PostOrderTraversal(avlTree);
    cout << "\nпрямой обход: ";
    PreOrderTraversal(avlTree);
    cout << "\n\n";
    menu(avlTree);
}
