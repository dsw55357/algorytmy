#include "ArrayDoublyLinkedList.h"
#include <stdexcept>

ArrayDoublyLinkedList::ArrayDoublyLinkedList() : head(-1), tail(-1) {}

ArrayDoublyLinkedList::~ArrayDoublyLinkedList() {}

void ArrayDoublyLinkedList::insert(const Circle& value) {
    Node newNode(value);
    data.push_back(newNode);
    int index = data.size() - 1;

    if (head == -1) {
        head = tail = index;
    } else {
        data[tail].next = index;
        data[index].prev = tail;
        tail = index;
    }
}

void ArrayDoublyLinkedList::remove(int index) {
    if (index < 0 || index >= data.size()) {
        throw std::out_of_range("Index out of range");
    }

    if (data[index].prev != -1) {
        data[data[index].prev].next = data[index].next;
    } else {
        head = data[index].next;
    }

    if (data[index].next != -1) {
        data[data[index].next].prev = data[index].prev;
    } else {
        tail = data[index].prev;
    }

    data.erase(data.begin() + index);
}

Circle& ArrayDoublyLinkedList::get(int index) {
    if (index < 0 || index >= data.size()) {
        throw std::out_of_range("Index out of range");
    }
    return data[index].data;
}

int ArrayDoublyLinkedList::getSize() const {
    return data.size();
}
