#ifndef ARRAYSTACK_H
#define ARRAYSTACK_H

#include "Circle.h"

#include <iostream>
#include <stdexcept>
#include <functional>

class ArrayStack {
private:
    Circle* data;
    int top;
    int capacity;

public:
    ArrayStack(int size = 100) {
        data = new Circle[size];
        capacity = size;
        top = -1;
    }

    ~ArrayStack() {
        delete[] data;
    }

    void push(const Circle& item) {
        if (isFull()) {
            throw std::runtime_error("Stack is full");
        }
        data[++top] = item;
    }

    Circle pop() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return data[top--];
    }

    Circle peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return data[top];
    }

    bool isEmpty() const {
        return top == -1;
    }

    bool isFull() const {
        return top == capacity - 1;
    }

    int size() const {
        return top + 1;
    }

    void clear() {
        top = -1;
    }

    void forEach(std::function<void(Circle&)> func) {
        for (int i = 0; i <= top; ++i) {
            func(data[i]);
        }
    }
};

#endif // ARRAYSTACK_H
