#pragma once

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <stack>

// Containers - пространство имен, предназначенное для хранения различных контейнеров.
namespace Containers
{
    // BinarySearchTree - шаблонный класс, описывающий структуру двоичного дерева поиска. 
    template<typename T>
    class BinarySearchTree
    {
    private:
        /*  >>> Вложенные структуры данных. <<<  */
        struct TreeNode;                                            // Структура узла дерева.


        /*  >>> Члены данных. <<<  */
        TreeNode* root;                                             // Указатель на корень (начальный узел) дерева.
        size_t sizeOfTree;                                          // Текущее количество узлов в дереве.


        /*  >>> Вспомогательные защищенные методы для изменения дерева. <<<  */
        void push(TreeNode*& currNode, const T& value);             // Добавление элемента в дерево.
        void clear(TreeNode* currNode);                             // Полная очистка дерева / поддерева.


        /*  >>> Вспомогательные защищенные методы для получения информации о дереве. <<<  */
        size_t height(TreeNode* currNode) const;


        /*  >>> Вспомогательные защищенные методы для осуществления поиска в дереве. <<<  */
        bool contains(TreeNode* currNode, const T& value) const;    // Проверка наличия элемента в дереве.


        /*  >>> Вспомогательные защищенные методы для копирования дерева. <<<  */
        TreeNode* copyTree(TreeNode* currNode);                     // Глубокое копирование дерева / поддерева.


    public:
        /*  >>> Описание структуры итератора. <<<  */
        class Iterator;


        /*  >>> Публичные методы для получения итераторов на начальный и пост-конечный элементы дерева. <<<  */
        Iterator begin() const;
        Iterator end() const;


        /*  >>> Конструкторы и деструктор. <<<  */
        BinarySearchTree();                                         // Конструктор по умолчанию.
        BinarySearchTree(const std::initializer_list<T>& someList); // Пользовательский конструктор.
        BinarySearchTree(const BinarySearchTree& other);            // Конструктор глубокого копирования.
        BinarySearchTree(BinarySearchTree&& other);                 // Конструктор перемещения.
        ~BinarySearchTree();                                        // Деструктор.

        
        /*  >>> Перегруженные операторы. <<<  */
        BinarySearchTree& operator=(const BinarySearchTree& other); // Оператор присваивания копированием.
        BinarySearchTree& operator=(BinarySearchTree&& other);      // Оператор присваивания перемещением.


        /*  >>> Публичные методы для изменения дерева. <<<  */
        void push(const T& value);                                  // Добавление элемента в дерево.
        void reconstruct();                                         // Реконструкция дерева (в случае его неверной структуры / плохой сбалансированности). 
        void clear();                                               // Полная очистка дерева.


        /*  >>> Публичные методы для получения информации о дереве. <<<  */
        bool isEmpty() const;                                       // Проверка, пустое ли дерево.
        size_t height() const;                                      // Возвращает текущую высоту дерева.
        size_t size() const;                                        // Возвращает количество узлов в дереве.


        /*  >>> Публичные методы для осуществления поиска в дереве. <<<  */
        bool contains(const T& value) const;                        // Проверка наличия элемента в дереве.
        T min() const;                                              // Нахождение минимального элемента в дереве.
        T max() const;                                              // Нахождение максимального элемента в дереве.


        /*  >>> Публичные методы для обхода дерева. <<<  */
        void print() const;                                         // Вывод элементов дерева в консоль.
        std::vector<T> toVector() const;                            // Преобразование дерева к вектору.


        /*  >>> Публичные методы для необходимых вычислений. <<<  */
        T sum() const;                                              // Суммирование значений всех узлов.
    };
}


/*  ... ОПРЕДЕЛЕНИЕ ФУНКЦИОНАЛА КЛАССА ...  */
namespace Containers
{
    /*  >>> Вложенные структуры данных. <<<  */
    template<typename T>
    struct BinarySearchTree<T>::TreeNode
    {
        T value;                                    // Значение, хранящееся в узле.

        TreeNode* left{ nullptr };                  // Указатель на левого потомка.
        TreeNode* right{ nullptr };                 // Указатель на правого потомка.

        TreeNode(const T& value) : value(value) {}  // Пользовательский конструктор для создания узла.
    };


    
    /*  >>> Вспомогательные защищенные методы для изменения дерева. <<<  */
    template<typename T>
    void BinarySearchTree<T>::push(TreeNode*& currNode, const T& value)
    {
        /* 1.   Если указатель на текущий узел равен nullptr,
                значит - я достиг места вставки нового элемента.   */
        if (currNode == nullptr)
        {
            /* 1.1  Создаю новый узел и присваиваю указатель на него переменной currNode,
                    тем самым связывая новый узел с родительским (посредством указателя currNode).  */
            currNode = new TreeNode(value);

            ++sizeOfTree;
            return;
        }

        /* 2.   В противном случае - в зависимости от значения value
                рекурсивно направляюсь в левого либо правого потомка.   */
        if (value < currNode->value) {
            push(currNode->left, value);
        }
        else {
            push(currNode->right, value);
        }
    }

    template<typename T>
    void BinarySearchTree<T>::clear(TreeNode* currNode)
    {
        // 1. Если текущий узел пустой - завершаю функцию (базовый случай рекурсии).
        if (currNode == nullptr) { return; }

        // 2. Рекурсивно очищаю левое и правое поддеревья.
        clear(currNode->left);
        clear(currNode->right);

        // 3. Удаляю текущий узел.
        delete currNode;
    }



    /*  >>> Вспомогательные защищенные методы для получения информации о дереве. <<<  */
    template<typename T>
    size_t BinarySearchTree<T>::height(TreeNode* currNode) const
    {
        // 1. Базовый случай: если узел пустой - высота равна 0.
        if (currNode == nullptr) { return 0; }

        // 2. Рекурсивный случай: высота текущего узла равна 1 + максимальная высота его поддеревьев.
        return 1 + std::max(height(currNode->left), height(currNode->right));
    }



    /*  >>> Вспомогательные защищенные методы для осуществления поиска в дереве. <<<  */
    template<typename T>
    bool BinarySearchTree<T>::contains(TreeNode* currNode, const T& value) const
    {
        // 1. Если узел, в котором я оказался, пустой - я ничего не нашел. 
        if (currNode == nullptr) {
            return false;
        }

        /* 2.   В противном случае - в зависимости от значения value либо возвращаю true,
                либо рекурсивно направляюсь в одного из потомков для продолжения поиска.   */
        if (value == currNode->value) {
            return true;
        }
        else if (value < currNode->value) {
            return contains(currNode->left, value);
        }
        else {
            return contains(currNode->right, value);
        }
    }



    /*  >>> Вспомогательные защищенные методы для копирования дерева. <<<  */
    template<typename T>
    typename BinarySearchTree<T>::TreeNode* BinarySearchTree<T>::copyTree(TreeNode* currNode)
    {
        // 1. Если текущий узел пустой - возвращаю nullptr (базовый случай рекурсии).
        if (currNode == nullptr) { return nullptr; }
    
        // 2. Создаю новый узел с таким же значением, как у текущего узла.
        TreeNode* newNode = new TreeNode(currNode->value);
    
        // 3. Рекурсивно копирую поддеревья и присваиваю результат соответствующему потомку нового узла.
        newNode->left = copyTree(currNode->left);
        newNode->right = copyTree(currNode->right);
    
        // 4. Возвращаю указатель на новый скопированный узел.
        return newNode;
    }



    /*  >>> Структура итератора. <<<  */
    template<typename T>
    class BinarySearchTree<T>::Iterator
    {
    private:
        /*  Для эффективного обхода дерева я использую стек, 
            в котором храню узлы, еще ожидающие посещения.  */
        std::stack<TreeNode*> waitingNodes;


        /*  Вспомогательный защищенный метод, 
            добавляющий в стек всех левых узлов-потомков текущего узла.  */
        void pushLeftNodes(TreeNode* currNode) 
        {
            while (currNode)
            {
                waitingNodes.push(currNode);
                currNode = currNode->left;
            }
        }


    public:
        // Информация об итераторе для библиотеки <algorithm>:
        using iterator_category = std::forward_iterator_tag;    // Тип итератора.
        using value_type = T;                                   // Тип элемента.
        using difference_type = std::ptrdiff_t;                 // Разница между итераторами.
        using pointer = T*;                                     // Указатель на элемент.
        using reference = T&;                                   // Ссылка на элемент.


        Iterator() = default;                                   // Конструктор по умолчанию.


        Iterator(TreeNode* rootNode)                            // Пользовательский конструктор.
        {
            /*  При создании итератора begin() необходимо поместить в стек
                все левые узлы (начиная с корня) для достижения наименьшего элемента.  */
            pushLeftNodes(rootNode);
        }


        /*  Перегрузка оператора разыменования '*'.
            Возвращает ссылку на значение текущего узла, на который смотрит итератор.  
            
            Осторожно! Неосторожное изменение элемента узла через 
            итератор может привести к неверной структуре бинарного дерева.  */
        reference operator*() const {
            return waitingNodes.top()->value;
        }


        /*  Перегрузка префиксного оператора инкрементирования '++'.
            Позволяет перейти к следующему элементу дерева (в порядке возрастания).  */
        Iterator& operator++()
        {
            TreeNode* currNode = waitingNodes.top();   // Получаю текущий узел из вершины стека.
            waitingNodes.pop();                        // Извлекаю текущий узел из стека (так как он уже обработан).

            // Если у текущего узла есть *правый потомок*:
            if (currNode->right) {
                pushLeftNodes(currNode->right); // Добавляю в стек всех левых узлов-потомков *правого потомка*.
            }

            return *this;
        }


        /*  Перегрузка оператора равенства '=='.
            Определяет, указывают ли два итератора на один и тот же элемент дерева.  */
        bool operator==(const Iterator& other) const 
        {
            // 1. Если оба стека пусты - итераторы равны (оба указывают на end()).
            if (waitingNodes.empty() && other.waitingNodes.empty()) { return true; }

            /* 2.   Если оба стека не пусты, cравниваю указатели на текущие узлы:
                    если указатели равны - значит итераторы указывают на один и тот же объект в памяти.   */
            else if (!waitingNodes.empty() && !other.waitingNodes.empty()) {
                return waitingNodes.top() == other.waitingNodes.top();
            }

            // 3. В остальных случаях (один стек пустой, другой нет) - итераторы не равны.
            else { return false; }
        }


        /*  Перегрузка оператора неравенства '!='.
            Возвращает значение, обратное оператору '=='.  */
        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };



    /*  >>> Публичные методы для получения итераторов на начальный и пост-конечный элементы дерева. <<<  */
    template<typename T>
    typename BinarySearchTree<T>::Iterator BinarySearchTree<T>::begin() const {
        return Iterator(root);
    }

    template<typename T>
    typename BinarySearchTree<T>::Iterator BinarySearchTree<T>::end() const {
        return Iterator();
    }



    /*  >>> Конструкторы и деструктор. <<<  */
    template <typename T>
    BinarySearchTree<T>::BinarySearchTree() : root(nullptr), sizeOfTree(0) {}

    template <typename T>
    BinarySearchTree<T>::BinarySearchTree(const std::initializer_list<T> &someList) : BinarySearchTree() 
    {
        for (const auto& value: someList) {
            push(value);
        }
    }

    template<typename T>
    BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree& other) : sizeOfTree(other.sizeOfTree) {
        root = copyTree(other.root);
    }

    template<typename T>
    BinarySearchTree<T>::BinarySearchTree(BinarySearchTree&& other) : root(other.root), sizeOfTree(other.sizeOfTree)
    {
        other.root = nullptr;
        other.sizeOfTree = 0;
    }

    template<typename T>
    BinarySearchTree<T>::~BinarySearchTree() 
    {
        if (!isEmpty()) {
            clear(root);
        }
    }



    /*  >>> Перегруженные операторы. <<<  */
    template<typename T>
    BinarySearchTree<T>& BinarySearchTree<T>::operator=(const BinarySearchTree& other)
    {
        // 1. Обрабатываю самоприсваивание.
        if (this == &other) {
            return *this;
        }

        // 2. Полностью очищаю дерево.
        clear();
        
        // 3. Копирую необходимые значения.
        root = copyTree(other.root);
        sizeOfTree = other.sizeOfTree;

        return *this;
    }

    template<typename T>
    BinarySearchTree<T>& BinarySearchTree<T>::operator=(BinarySearchTree&& other)
    {
        // 1. Обрабатываю самоприсваивание.
        if (this == &other) {
            return *this;
        }

        // 2. Полностью очищаю дерево.
        clear();

        // 3. *Забираю ресурсы*.
        root = other.root;
        sizeOfTree = other.sizeOfTree;

        // 4. *Обнуляю* указатель на корень r-value объекта и его размер.
        other.root = nullptr;
        other.sizeOfTree = 0;

        return *this;
    } 



    /*  >>> Публичные методы для изменения дерева. <<<  */
    template<typename T>
    void BinarySearchTree<T>::push(const T& value) {
        push(root, value);
    }

    template<typename T>
    void BinarySearchTree<T>::reconstruct()
    {
        // 1. Получаю все элементы дерева в отсортированном порядке.
        std::vector<T> elements{ toVector() };

        // 2. Создаю генератор случайных чисел, инициализирую его зерном.
        std::random_device source;
        std::mt19937 generator(source());

        /* 3.   Перемешиваю элементы вектора случайным образом, чтобы
                избежать сильной несбалансированности бинарного дерева поиска.   */
        std::shuffle(elements.begin(), elements.end(), generator);

        // 4. Полностью очищаю текущее дерево.
        clear();

        // 5. Вставляю каждый элемент из перемешанного вектора обратно в дерево.
        for (const auto& value: elements) {
            push(value);
        }
    }

    template<typename T>
    void BinarySearchTree<T>::clear()
    {
        // 1. Очищаю дерево.
        clear(root);

        // 2. *Обнуляю* указатель на корень и размер дерева.
        root = nullptr;
        sizeOfTree = 0;
    }



    /*  >>> Публичные методы для получения информации о дереве. <<<  */
    template<typename T>
    bool BinarySearchTree<T>::isEmpty() const {
        return root == nullptr && sizeOfTree == 0;
    }

    template<typename T>
    size_t BinarySearchTree<T>::height() const {
        return height(root);
    }

    template<typename T>
    size_t BinarySearchTree<T>::size() const {
        return sizeOfTree;
    }



    /*  >>> Публичные методы для осуществления поиска в дереве. <<<  */
    template<typename T>
    bool BinarySearchTree<T>::contains(const T& value) const {
        return contains(root, value);
    }

    template<typename T>
    T BinarySearchTree<T>::min() const
    {
        // 1. Если дерево пустое - выбрасываю исключение с соответствующим сообщением.
        if (isEmpty()) {
            throw std::runtime_error("Error! It is impossible to access the minimum element in an empty tree.");
        }

        /* 2.   В противном случае - передвигаюсь по дереву исключительно по левой стороне,
                пока не окажусь в узле, у которого отсутствует левый потомок. Значение узла,
                в котором я остановился - и есть самое минимальное в текущем двоичном дереве.   */
        TreeNode* tempNode = root;

        while (tempNode->left) {
            tempNode = tempNode->left;
        }

        return tempNode->value;
    }

    template<typename T>
    T BinarySearchTree<T>::max() const
    {
        // 1. Если дерево пустое - выбрасываю исключение с соответствующим сообщением.
        if (isEmpty()) {
            throw std::runtime_error("Error! It is impossible to access the maximum element in an empty tree.");
        }

        // 2. Схема действий аналогична действиям из метода min(). 
        TreeNode* tempNode = root;

        while (tempNode->right) {
            tempNode = tempNode->right;
        }

        return tempNode->value;
    }



    /*  >>> Публичные методы для обхода дерева. <<<  */
    template<typename T>
    void BinarySearchTree<T>::print() const
    {
        for (const auto& element: *this) {
            std::cout << element << ' ';
        }

        std::cout << std::endl;
    }

    template<typename T>
    std::vector<T> BinarySearchTree<T>::toVector() const
    {
        std::vector<T> elements;

        for (const auto& value: *this) {
            elements.push_back(value);
        }

        return elements;
    }



    /*  >>> Публичные методы для необходимых вычислений. <<<  */
    template<typename T>
    T BinarySearchTree<T>::sum() const 
    {
        T resultSum = T();

        for(const auto& value: *this) {
            resultSum += value;
        }

        return resultSum;
    }
}
