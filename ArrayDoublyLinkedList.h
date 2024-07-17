#ifndef ARRAYDOUBLYLINKEDLIST_H
#define ARRAYDOUBLYLINKEDLIST_H

#include "Circle.h"
#include <vector>

class ArrayDoublyLinkedList {
private:
    struct Node {
        Circle data;
        int prev;
        int next;
        Node(const Circle& value) : data(value), prev(-1), next(-1) {}
    };

    std::vector<Node> data;
    int head;
    int tail;

public:
    ArrayDoublyLinkedList();
    ~ArrayDoublyLinkedList();

    void insert(const Circle& value);
    void remove(int index);
    Circle& get(int index);
    int getSize() const;
};

#endif // ARRAYDOUBLYLINKEDLIST_H
