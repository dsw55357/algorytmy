#ifndef ARRAYBASEDLINKEDLIST_H
#define ARRAYBASEDLINKEDLIST_H

#include "Circle.h"
#include <stdexcept>

class ArrayLinkedList {
private:
    std::vector<Circle> data;

    //Circle* data;
    int* next;
    int head;
    int freeIndex;
    int size;
    int capacity;

    void resize(int new_capacity);

public:
    ArrayLinkedList(int initial_capacity = 10);
    ~ArrayLinkedList();

    void insert(const Circle& value);
    void remove(int index);
    Circle& get(int index) const;
    int getSize() const;
    int getHead() const;
};

#endif // ARRAYBASEDLINKEDLIST_H
