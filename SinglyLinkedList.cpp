#include "SinglyLinkedList.h"
#include <iostream>

SinglyLinkedList::SinglyLinkedList() : head(nullptr) {}

SinglyLinkedList::~SinglyLinkedList() {
    Node* current = head;
    Node* next = nullptr;
    while (current != nullptr) {
        next = current->next;
        delete current;
        current = next;
    }
}

int SinglyLinkedList::getSize() const {
    Node* current = head;
    int c {0};
    while (current != nullptr) {
        c++;
        current = current->next;    
    }
    
    return c;
}

//void SinglyLinkedList::insert(int value) {
void SinglyLinkedList::insert(const Circle& value) {
    Node* newNode = new Node(value);
    newNode->next = head;
    head = newNode;
}

// void SinglyLinkedList::remove(int value) {
//     if (head == nullptr) return;

//     if (head->data == value) {
//         Node* temp = head;
//         head = head->next;
//         delete temp;
//         return;
//     }

//     Node* current = head;
//     while (current->next != nullptr && current->next->data != value) {
//         current = current->next;
//     }

//     if (current->next != nullptr) {
//         Node* temp = current->next;
//         current->next = current->next->next;
//         delete temp;
//     }
// }

void SinglyLinkedList::remove(Node* prevNode, Node* currentNode) {
    if (currentNode == nullptr) return;

    if (currentNode == head) {
        head = currentNode->next;
        delete currentNode;
    } else {
        prevNode->next = currentNode->next;
        delete currentNode;
    }
}

void SinglyLinkedList::display() const {
    Node* current = head;
    while (current != nullptr) {
        std::cout << "(" << current->data.x << ", " << current->data.y << ") -> ";
        current = current->next;
    }
    std::cout << "nullptr" << std::endl;
}

Node* SinglyLinkedList::getHead() const {
    return head;
}
