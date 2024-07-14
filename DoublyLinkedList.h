#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

struct DNode {
    int data;
    DNode* next;
    DNode* prev;
    DNode(int value) : data(value), next(nullptr), prev(nullptr) {}
};

class DoublyLinkedList {
private:
    DNode* head;

public:
    DoublyLinkedList();
    ~DoublyLinkedList();
    
    void insert(int value);
    void remove(int value);
    void display() const;
};

#endif // DOUBLYLINKEDLIST_H
