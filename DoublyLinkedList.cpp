#include "DoublyLinkedList.h"
#include <iostream>

DoublyLinkedList::DoublyLinkedList() : head(nullptr) {}

DoublyLinkedList::~DoublyLinkedList() {
    DNode* current = head;
    DNode* next = nullptr;
    while (current != nullptr) {
        next = current->next;
        delete current;
        current = next;
    }
}

void DoublyLinkedList::insert(const Circle& value) {
    DNode* newNode = new DNode(value);
    newNode->next = head;
    if (head != nullptr) {
        head->prev = newNode;
    }
    head = newNode;
}

void DoublyLinkedList::remove(DNode* prevNode, DNode* currentNode) {
    if (currentNode == nullptr) return;

    if (currentNode == head) {
        head = currentNode->next;
        if (head != nullptr) {
            head->prev = nullptr;
        }
        delete currentNode;
    } else {
        if (currentNode->next != nullptr) {
            currentNode->next->prev = currentNode->prev;
        }
        if (currentNode->prev != nullptr) {
            currentNode->prev->next = currentNode->next;
        }
        delete currentNode;
    }
}

void DoublyLinkedList::display() const {
    DNode* current = head;
    while (current != nullptr) {
        std::cout << "(" << current->data.x << ", " << current->data.y << ") <-> ";
        current = current->next;
    }
    std::cout << "nullptr" << std::endl;
}

DNode* DoublyLinkedList::getHead() const {
    return head;
}