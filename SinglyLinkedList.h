#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

#include "Circle.h"

struct Node {
    // int data;
    Circle data;
    Node* next;
    //Node(int value) : data(value), next(nullptr) {}
    Node(const Circle& value) : data(value), next(nullptr) {} 
};

class SinglyLinkedList {
private:
    Node* head;

public:
    SinglyLinkedList();
    ~SinglyLinkedList();
    
    //void insert(int value);
    void insert(const Circle& value);
    // void remove(int value);
    void remove(Node* prevNode, Node* currentNode);

    void display() const;
    Node* getHead() const;
};

#endif // SINGLYLINKEDLIST_H
