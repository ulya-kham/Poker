#pragma once
#include <stdexcept>
template <typename T>
class CustomStack {//стек на динамич массиве
private:
    T* data;//динамич массив
    int capacity;//вместимость
    int topIndex;//индекс вершины
public:
    CustomStack(int cap = 10);//конструктор
    CustomStack(const CustomStack& other);//конструктор копирования
    ~CustomStack();//деструктор
    CustomStack& operator=(const CustomStack& other);//оператор присваивания
    void push(const T& item);//добавить элемент
    T pop();//удалить и вернуть верхний
    T peek() const;//посмотреть верхний
    bool isEmpty() const;//проверка на пустоту
    bool isFull() const;//проверка на заполненность
    int getSize() const;//получить размер
    void clear();//очистить стек
    T& operator[](int index);//доступ по индексу
    const T& operator[](int index) const;//доступ по индексу
};
template <typename T>
CustomStack<T>::CustomStack(int cap) : capacity(cap), topIndex(-1) {//конструктор
    data = new T[capacity];//выделяем память
}
template <typename T>
CustomStack<T>::CustomStack(const CustomStack& other) : capacity(other.capacity), topIndex(other.topIndex) {//конструктор копирования
    data = new T[capacity];//выделяем память
    for (int i = 0; i <= topIndex; i++) {//копируем элементы
        data[i] = other.data[i];
    }
}
template <typename T>
CustomStack<T>::~CustomStack() {//деструктор
    delete[] data;//освобождаем память
}
template <typename T>
CustomStack<T>& CustomStack<T>::operator=(const CustomStack& other) {//оператор присваивания
    if (this != &other) {//проверка на самоприсваивание
        delete[] data;//освобождаем старую память
        capacity = other.capacity;
        topIndex = other.topIndex;
        data = new T[capacity];//выделяем новую память
        for (int i = 0; i <= topIndex; i++) {//копируем элементы
            data[i] = other.data[i];
        }
    }
    return *this;
}
template <typename T>
void CustomStack<T>::push(const T& item) {//добавление элемента
    if (isFull()) {//если стек полон
        throw std::runtime_error("Переполнение стека: невозможно добавить элемент");
    }
    data[++topIndex] = item;//добавляем
}
template <typename T>
T CustomStack<T>::pop() {//удаление верхнего
    if (isEmpty()) {//если стек пуст
        throw std::runtime_error("Стек пуст: невозможно удалить элемент");
    }
    return data[topIndex--];//возвращаем и уменьшаем индекс
}
template <typename T>
T CustomStack<T>::peek() const {//просмотр верхнего
    if (isEmpty()) {//если стек пуст
        throw std::runtime_error("Стек пуст: невозможно просмотреть");
    }
    return data[topIndex];
}
template <typename T>
bool CustomStack<T>::isEmpty() const {//проверка на пустоту
    return topIndex == -1;
}
template <typename T>
bool CustomStack<T>::isFull() const {//проверка на заполненность
    return topIndex == capacity - 1;
}
template <typename T>
int CustomStack<T>::getSize() const {//получение размера
    return topIndex + 1;
}
template <typename T>
void CustomStack<T>::clear() {//очистка стека
    topIndex = -1;
}
template <typename T>
T& CustomStack<T>::operator[](int index) {//доступ по индексу
    if (index < 0 || index > topIndex) {//проверка границ
        throw std::out_of_range("Индекс вне границ стека");
    }
    return data[index];
}
template <typename T>
const T& CustomStack<T>::operator[](int index) const {//доступ по индексу
    if (index < 0 || index > topIndex) {//проверка границ
        throw std::out_of_range("Индекс вне границ стека");
    }
    return data[index];
}