#include "ArrayLinkedList.h"
#include <stdexcept>

ArrayLinkedList::ArrayLinkedList() {}

ArrayLinkedList::~ArrayLinkedList() {}

void ArrayLinkedList::insert(const Circle& value) {
    data.push_back(value);
}

void ArrayLinkedList::remove(int index) {
    if (index < 0 || index >= data.size()) {
        throw std::out_of_range("Index out of range");
    }
    data.erase(data.begin() + index);
}

Circle& ArrayLinkedList::get(int index) {
    if (index < 0 || index >= data.size()) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

int ArrayLinkedList::getSize() const {
    return data.size();
}
