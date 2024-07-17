#ifndef ARRAYLINKEDLIST_H
#define ARRAYLINKEDLIST_H

#include "Circle.h"
#include <vector>

class ArrayLinkedList {
private:
    std::vector<Circle> data;

public:
    ArrayLinkedList();
    ~ArrayLinkedList();

    void insert(const Circle& value);
    void remove(int index);
    Circle& get(int index);
    int getSize() const;
};

#endif // ARRAYLINKEDLIST_H
