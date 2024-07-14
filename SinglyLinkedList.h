#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

struct Node {
    int data;
    Node* next;
    Node(int value) : data(value), next(nullptr) {}
};

class SinglyLinkedList {
private:
    Node* head;

public:
    SinglyLinkedList();
    ~SinglyLinkedList();
    
    void insert(int value);
    void remove(int value);
    void display() const;
};

#endif // SINGLYLINKEDLIST_H
