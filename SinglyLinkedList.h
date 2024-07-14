#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

#include "Circle.h"

struct Node {
    Circle data;
    Node* next;
    Node(const Circle& value) : data(value), next(nullptr) {} 
};

class SinglyLinkedList {
private:
    Node* head;

public:
    SinglyLinkedList();
    ~SinglyLinkedList();
    
    void insert(const Circle& value);
    void remove(Node* prevNode, Node* currentNode);

    void display() const;
    Node* getHead() const;
};

#endif // SINGLYLINKEDLIST_H
