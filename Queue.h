#ifndef CIRCLEQUEUE_H
#define CIRCLEQUEUE_H

#include "Circle.h"
#include <iostream>
#include <stdexcept>

class CircleQueue {
private:
    Circle* data;
    int front;
    int rear;
    int capacity;
    int count;

public:
    CircleQueue(int size = 100) {
        data = new Circle[size];
        capacity = size;
        front = 0;
        rear = -1;
        count = 0;
    }

    CircleQueue(const CircleQueue& other) {
        capacity = other.capacity;
        data = new Circle[capacity];
        front = other.front;
        rear = other.rear;
        count = other.count;
        for (int i = 0; i < capacity; ++i) {
            data[i] = other.data[i];
        }
    }

    ~CircleQueue() {
        delete[] data;
    }

    void enqueue(const Circle& item) {
        if (isFull()) {
            throw std::runtime_error("Queue is full");
        }
        rear = (rear + 1) % capacity;
        data[rear] = item;
        count++;
    }

    Circle dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        Circle item = data[front];
        front = (front + 1) % capacity;
        count--;
        return item;
    }

    Circle peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        return data[front];
    }

    bool isEmpty() const {
        return count == 0;
    }

    bool isFull() const {
        return count == capacity;
    }

    int size() const {
        return count;
    }

    void forEach(std::function<void(Circle&)> func) {
        int index = front;
        for (int i = 0; i < count; ++i) {
            func(data[index]);
            index = (index + 1) % capacity;
        }
    }

    /*
    std::function<void(const Circle&)> func: Parametrem metody jest func, który jest funkcją o sygnaturze void(const Circle&), czyli przyjmuje jako argument stałą referencję do obiektu Circle i nie zwraca żadnej wartości.
    const: Metoda jest oznaczona jako const, co oznacza, że nie modyfikuje stanu obiektu.
    */
    void forEach(std::function<void(const Circle&)> func) const {
        // Inicjalizujemy zmienną index wartością front, co oznacza początek kolejki.
        int index = front;
        for (int i = 0; i < count; ++i) {
            /*
            Wykonujemy przekazaną funkcję func na bieżącym elemencie kolejki, który znajduje się pod indeksem index.
            */
            func(data[index]);
            /*
            Przesunięcie indeksu do następnego elementu:
            Zwiększamy wartość index o 1 i używamy operatora modulo (%), aby zapewnić cykliczność indeksu w przypadku, gdy index osiągnie wartość capacity. Dzięki temu index wraca do 0, gdy osiągnie koniec tablicy, co realizuje cykliczną naturę kolejki.
            */
            index = (index + 1) % capacity;
        }
    }

    void removeIf(std::function<bool(const Circle&)> predicate) {
        int currentSize = count;
        for (int i = 0; i < currentSize; ++i) {
            Circle circle = dequeue();
            if (!predicate(circle)) {
                enqueue(circle);
            }
        }
    }

};

#endif // CIRCLEQUEUE_H
