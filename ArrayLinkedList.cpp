#include "ArrayLinkedList.h"

void ArrayLinkedList::resize(int new_capacity) {
    Circle* new_data = new Circle[new_capacity];
    int* new_next = new int[new_capacity];
    for (int i = 0; i < capacity; ++i) {
        new_data[i] = data[i];
        new_next[i] = next[i];
    }
    delete[] data;
    delete[] next;
    data = new_data;
    next = new_next;
    for (int i = capacity; i < new_capacity; ++i) {
        next[i] = i + 1;
    }
    next[new_capacity - 1] = -1;
    freeIndex = capacity;
    capacity = new_capacity;
}

ArrayLinkedList::ArrayLinkedList(int initial_capacity) {
    size = 0;
    capacity = initial_capacity;
    data = new Circle[capacity];
    next = new int[capacity];
    for (int i = 0; i < capacity - 1; ++i) {
        next[i] = i + 1;
    }
    next[capacity - 1] = -1;
    head = -1;
    freeIndex = 0;
}

ArrayLinkedList::~ArrayLinkedList() {
    delete[] data;
    delete[] next;
}

void ArrayLinkedList::insert(const Circle& value) {
    if (freeIndex == -1) {
        resize(capacity * 2);
    }
    int newNode = freeIndex;
    freeIndex = next[freeIndex];
    data[newNode] = value;
    next[newNode] = head;
    head = newNode;
    ++size;
}

void ArrayLinkedList::remove(int index) {
    if (index < 0 || index >= capacity) {
        throw std::out_of_range("Index out of range");
    }
    if (head == -1) return;

    if (head == index) {
        int temp = head;
        head = next[head];
        next[temp] = freeIndex;
        freeIndex = temp;
    } else {
        int current = head;
        while (next[current] != -1 && next[current] != index) {
            current = next[current];
        }
        if (next[current] == index) {
            int temp = next[current];
            next[current] = next[next[current]];
            next[temp] = freeIndex;
            freeIndex = temp;
        }
    }
    --size;
}

Circle& ArrayLinkedList::get(int index) const {
    if (index < 0 || index >= capacity) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

int ArrayLinkedList::getSize() const {
    return size;
}

int ArrayLinkedList::getHead() const {
    return head;
}
