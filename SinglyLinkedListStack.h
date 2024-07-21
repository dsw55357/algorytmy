#ifndef SINGLYLINKEDLISTSTACK_H
#define SINGLYLINKEDLISTSTACK_H

#include "Circle.h"
#include <iostream>
#include <functional>
#include <stdexcept>

class SinglyLinkedListStack {
private:
    struct Node {
        Circle data;
        Node* next;
        Node(const Circle& data) : data(data), next(nullptr) {}
    };

    Node* head;
    int size;

public:
    SinglyLinkedListStack() : head(nullptr), size(0) {}

    ~SinglyLinkedListStack() {
        clear();
    }

    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }

    void push(const Circle& item) {
        Node* newNode = new Node(item);
        newNode->next = head;
        head = newNode;
        ++size;
    }

    Circle pop() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        Node* temp = head;
        Circle data = head->data;
        head = head->next;
        delete temp;
        --size;
        return data;
    }

    Circle peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return head->data;
    }

    bool isEmpty() const {
        return size == 0;
    }

    int getSize() const {
        return size;
    }

    void forEach(std::function<void(Circle&)> func) {
        Node* current = head;
        while (current) {
            func(current->data);
            current = current->next;
        }
    }
};

#endif // SINGLYLINKEDLISTSTACK_H
