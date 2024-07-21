#ifndef SINGLYLINKEDLISTQUEUE_H
#define SINGLYLINKEDLISTQUEUE_H

#include <iostream>
#include <functional>

template <typename T>
class SinglyLinkedListQueue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& data) : data(data), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size;

public:
    SinglyLinkedListQueue() : head(nullptr), tail(nullptr), size(0) {}

    ~SinglyLinkedListQueue() {
        clear();
    }

    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        size = 0;
    }

    void enqueue(const T& data) {
        Node* newNode = new Node(data);
        if (tail) {
            tail->next = newNode;
        } else {
            head = newNode;
        }
        tail = newNode;
        ++size;
    }

    T dequeue() {
        if (!head) throw std::runtime_error("Queue is empty");
        Node* temp = head;
        T data = head->data;
        head = head->next;
        if (!head) {
            tail = nullptr;
        }
        delete temp;
        --size;
        return data;
    }

    T peek() const {
        if (!head) throw std::runtime_error("Queue is empty");
        return head->data;
    }

    bool isEmpty() const {
        return size == 0;
    }

    int getSize() const {
        return size;
    }

    void forEach(std::function<void(T&)> func) {
        Node* current = head;
        while (current) {
            func(current->data);
            current = current->next;
        }
    }

    void removeIf(std::function<bool(const T&)> predicate) {
        Node* current = head;
        Node* prev = nullptr;
        while (current) {
            if (predicate(current->data)) {
                Node* temp = current;
                if (prev) {
                    prev->next = current->next;
                } else {
                    head = current->next;
                }
                if (current == tail) {
                    tail = prev;
                }
                current = current->next;
                delete temp;
                --size;
            } else {
                prev = current;
                current = current->next;
            }
        }
    }
};

#endif // SINGLYLINKEDLISTQUEUE_H
