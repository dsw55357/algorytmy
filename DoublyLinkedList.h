#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include "Circle.h"

struct DNode {
    Circle data;
    DNode* next;
    DNode* prev;
    DNode(const Circle& value) : data(value), next(nullptr), prev(nullptr) {}

};

class DoublyLinkedList {
private:
    DNode* head;

public:
    DoublyLinkedList();
    ~DoublyLinkedList();
    
    void insert(const Circle& value);
    void remove(DNode* prevNode, DNode* currentNode);
    void display() const;
    DNode* getHead() const;

};

#endif // DOUBLYLINKEDLIST_H
