#pragma once
#include <iostream>

// Пространство имен Containers предназначено для хранения различных контейнеров.
namespace Containers 
{
    // Vector - шаблонный класс, являющийся оберткой над динамическим массивом.
    template<typename T>
    class Vector 
    {
    private:
        /* === Данные для хранения элементов и управления размером вектора: === */
        T*     objects;               // Указатель на массив элементов.
        size_t currSize     = 0;      // Текущий размер вектора (количество элементов).
        size_t currCapacity = 10;     // Текущая ёмкость вектора (максимальное количество элементов без перевыделения памяти).


        /* === Вспомогательный метод для выделения памяти: === */
        void allocateMemory();        // Выделение памяти необходимого размера для хранения элементов.


    public:
        /* === Описание структуры итератора: === */
        class Iterator;


        /* === Методы для получения итераторов на начало и конец вектора: === */
        Iterator begin() const;
        Iterator end()   const;


        /* === Конструкторы и деструктор: === */
        Vector();                                       // Конструктор по умолчанию.
        Vector(int inputCapacity);                      // Конструктор с заданной ёмкостью.
        Vector(const std::initializer_list<T>& values); // Конструктор из списка инициализации.

        Vector(Vector&& other) noexcept;                // Конструктор перемещения.
        Vector(const Vector& other);                    // Конструктор копирования.

        ~Vector();                                      // Деструктор.


        /* === Перегруженные операторы: === */
        Vector  operator+ (const Vector& other) const;  // Оператор сложения двух векторов.
        Vector& operator+=(const Vector& other);        // Оператор составного присваивания.
        Vector& operator= (const Vector& other);        // Оператор присваивания копированием.
        Vector& operator= (Vector&& other) noexcept;    // Оператор присваивания перемещением.
        bool    operator<<(const T& value) const;       // Оператор проверки наличия элемента в векторе.


        /* === Методы для добавления и удаления элементов: === */
        void pushBack(T value);                         // Добавление элемента в конец вектора.
        void pushBack(const std::initializer_list<T>&); // Добавление диапазона элементов в конец вектора.
        T    popBack();                                 // Удаление последнего элемента из вектора.


        /* === Методы доступа к элементам вектора: === */
        T& at(size_t index);                            // Обращение к элементу с проверкой границ.
        T& operator[](size_t index);                    // Обращение к элементу без проверки границ.
        T  front() const;                               // Получение первого элемента вектора.
        T  back()  const;                               // Получение последнего элемента вектора.


        /* === Методы для получения информации о векторе: === */
        bool   isEmpty()  const;                        // Проверка на пустоту вектора.
        size_t capacity() const;                        // Получение текущей ёмкости вектора.
        size_t size()     const;                        // Получение текущего размера вектора.
    };

} // namespace Containers.



/* ... ОПРЕДЕЛЕНИЕ ФУНКЦИОНАЛЬНОСТИ КЛАССА ... */
namespace Containers 
{

    /* === Вспомогательный защищенный метод для выделения памяти: === */
    template<typename T> 
    void Vector<T>::allocateMemory() {
        objects = new T[currCapacity];
    }


    /* === Описание структуры итератора: === */
    template<typename T> 
    class Vector<T>::Iterator 
    {
    private:
        // Указатель на текущий элемент, на который ссылается итератор.
        T* currPointer;

    public:
        // Информация об итераторе для совместимости со стандартными алгоритмами:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        // Пользовательский конструктор:
        Iterator(T* inputPointer) : currPointer(inputPointer) {
            /* Если пользователь передал значение nullptr, выбрасываю исключение,
               так как разыменование nullptr приведет к неопределенному поведению. */
            if (inputPointer == nullptr) {
                throw std::runtime_error("Error! Iterator cannot be nullptr.");
            }
        }

        // Операторы разыменования:
        reference operator*()  const { return *currPointer; }
        pointer   operator->() const { return currPointer;  }

        // Операторы инкрементирования (префиксный и постфиксный):
        Iterator& operator++()    { ++currPointer; return *this; }
        Iterator  operator++(int) { Iterator temp = *this; ++currPointer; return temp; }

        // Операторы декрементирования (префиксный и постфиксный):
        Iterator& operator--()    { --currPointer; return *this; }
        Iterator  operator--(int) { Iterator temp = *this; --currPointer; return temp; }

        // Арифметические операторы:
        Iterator  operator+(difference_type n) const { return Iterator(currPointer + n); }
        Iterator  operator-(difference_type n) const { return Iterator(currPointer - n); }

        Iterator& operator+=(difference_type n) { currPointer += n; return *this; }
        Iterator& operator-=(difference_type n) { currPointer -= n; return *this; }

        difference_type operator-(const Iterator& other) const { return currPointer - other.currPointer; }

        // Операторы сравнения:
        bool operator==(const Iterator& other) const { return currPointer == other.currPointer; }
        bool operator!=(const Iterator& other) const { return currPointer != other.currPointer; }

        bool operator<=(const Iterator& other) const { return currPointer <= other.currPointer; }
        bool operator>=(const Iterator& other) const { return currPointer >= other.currPointer; }

        bool operator< (const Iterator& other) const { return currPointer < other.currPointer;  }
        bool operator> (const Iterator& other) const { return currPointer > other.currPointer;  }

        // Оператор индексирования:
        reference operator[](difference_type n) const { return *(currPointer + n); }
    };


    /* === Публичные методы для получения итераторов на начало и конец вектора: === */
    template<typename T> typename Vector<T>::Iterator Vector<T>::begin() const { return Iterator(objects); }
    template<typename T> typename Vector<T>::Iterator Vector<T>::end()   const { return Iterator(objects + currSize); }


    /* === Конструкторы и деструктор: === */
    template<typename T>
    Vector<T>::Vector() { this->allocateMemory(); }

    template<typename T>
    Vector<T>::Vector(int inputCapacity) : currCapacity(inputCapacity) { this->allocateMemory(); }

    template<typename T>
    Vector<T>::Vector(const std::initializer_list<T>& values) : Vector(values.size() * 2)
    {
        // ^^^ Увеличиваю емкость вектора, чтобы избежать частых перераспределений памяти ^^^ .
        for (const T& currValue: values) { this->pushBack(currValue); }
    }

    template<typename T>
    Vector<T>::Vector(Vector&& other) noexcept(true)
    {
        // Переношу ресурсы из другого объекта в текущий.
        this->objects = other.objects;
        this->currSize = other.currSize; 
        this->currCapacity = other.currCapacity;

        // Очищаю данные другого объекта, чтобы избежать двойного освобождения памяти.
        other.objects = nullptr;
        other.currSize = 0; other.currCapacity = 0;
    }

    template<typename T>
    Vector<T>::Vector(const Vector& other) : Vector(other.currCapacity) {
        for (const T& currValue : other) { this->pushBack(currValue); }
    }

    template<typename T>
    Vector<T>::~Vector() { delete[] objects; }


    /* === Перегруженные операторы === */
    template<typename T>
    Vector<T> Vector<T>::operator+(const Vector& other) const
    {
        // Создаю временный вектор с суммарной емкостью.
        Vector tempObject(this->currSize + other.currSize);

        // Копирую элементы из обоих векторов в новый.
        for (const T& value: *this) { tempObject.pushBack(value); }
        for (const T& value: other) { tempObject.pushBack(value); }

        return tempObject;
    }

    template<typename T>
    Vector<T>& Vector<T>::operator+=(const Vector& other) {
        for (const T& value: other) { this->pushBack(value); }
        return *this;
    }

    template<typename T>
    Vector<T>& Vector<T>::operator=(const Vector& other)
    {
        // Осуществляю проверку на самоприсваивание, чтобы избежать возможных ошибок.
        if (this != &other) 
        {
            // Освобождаю память, занятую текущими элементами.
            delete[] objects;

            // Сбрасываю размер вектора и устанавливаю новую емкость из другого вектора.
            this->currSize = 0;
            this->currCapacity = other.currCapacity;

            // Выделяю новую память под элементы с учетом обновленной емкости.
            allocateMemory();

            // Копирую элементы из другого вектора в текущий с помощью метода pushBack.
            for (const T& value : other) { 
                this->pushBack(value); 
            }
        }

        return *this;
    }

    template<typename T>
    Vector<T>& Vector<T>::operator=(Vector&& other) noexcept(true)
    {
        // Осуществляю проверку на самоприсваивание, чтобы избежать ненужных операций и возможных ошибок.
        if (this != &other)
        {
            // Освобождаю память, занятую текущими элементами.
            delete[] objects;

            // Переношу ресурсы из другого объекта в текущий.
            this->objects = other.objects;
            this->currSize = other.currSize; 
            this->currCapacity = other.currCapacity;

            // Очищаю данные другого объекта, чтобы избежать двойного освобождения памяти.
            other.objects = nullptr;
            other.currSize = 0; other.currCapacity = 0;
        }

        return *this;
    }

    template<typename T>
    bool Vector<T>::operator<<(const T& value) const
    {
        for (const T& currValue: *this) {
            if (currValue == value) return true;
        }

        return false;
    }


    /* === Публичные методы для добавления и удаления элементов из вектора: === */
    template<typename T>
    void Vector<T>::pushBack(T value)
    {
        // Если я начинаю превышать емкость:
        if (currSize >= currCapacity)
        {
            // Увеличиваю емкость в 2 раза и выделяю новую память.
            currCapacity *= 2;
            T* newMemory = new T[currCapacity];

            // Копирую существующие элементы в новый массив.
            for (size_t i = 0; i < currSize; ++i) {
                newMemory[i] = objects[i];
            }

            // Освобождаю старую память и переназначаю указатель.
            delete[] objects;
            objects = newMemory;
        }

        // Добавляю новый элемент в конец и увеличиваю размер вектора.
        objects[currSize] = value;
        ++currSize;
    }

    template<typename T>
    void Vector<T>::pushBack(const std::initializer_list<T>& values)
    {
        for (const T& currValue: values) {
            this->pushBack(currValue);
        }
    }

    template<typename T>
    T Vector<T>::popBack()
    {
        // Если вектор пустой - выбрасываю исключение.
        if (this->isEmpty()) {
            throw std::runtime_error("Error! You cannot delete an element from an empty vector.");
        }

        // Уменьшаю размер вектора и сохраняю удаляемый элемент.
        --currSize;
        T temp = objects[currSize];

        // Вызываю деструктор для удаленного элемента (необязательно, если тип T - примитивный).
        objects[currSize].~T();

        return temp;
    }


    /* === Публичные методы доступа к элементам вектора: === */
    template<typename T>
    T& Vector<T>::at(size_t index)
    {
        if (index >= currSize) {
            throw std::out_of_range("Error! Index is out of range.");
        }

        return objects[index];
    }

    template<typename T>
    T& Vector<T>::operator[](size_t index) 
    {
        // Обращение к элементу без проверки границ может привести к ошибкам при некорректном индексе!
        return objects[index];
    }

    template<typename T>
    T Vector<T>::front() const
    {
        if (this->isEmpty()) {
            throw std::runtime_error("Error! You cannot access the first element in an empty vector.");
        }

        return objects[0];
    }

    template<typename T>
    T Vector<T>::back() const
    {
        if (this->isEmpty()) {
            throw std::runtime_error("Error! You cannot access the last element in an empty vector.");
        }

        return objects[currSize - 1];
    }


    /* === Методы для получения информации о векторе: === */
    template<typename T> bool   Vector<T>::isEmpty()  const { return currSize == 0; }
    template<typename T> size_t Vector<T>::capacity() const { return currCapacity;  }
    template<typename T> size_t Vector<T>::size()     const { return currSize;      }

} // namespace Containers.