#include "Array.h"
#include <iostream>
#include <stdexcept>

void Array::resize(int new_capacity) {
    int* new_data = new int[new_capacity];
    for (int i = 0; i < size; ++i) {
        new_data[i] = data[i];
    }
    delete[] data;
    data = new_data;
    capacity = new_capacity;
}

// Konstruktor
Array::Array(int initial_capacity) {
    size = 0;
    capacity = initial_capacity;
    data = new int[capacity];
}

// Destruktor
Array::~Array() {
    delete[] data;
}

// Metoda do dodawania elementu
void Array::add(int value) {
    if (size == capacity) {
        resize(capacity * 2);
    }
    data[size++] = value;
}

// Metoda do usuwania elementu na danym indeksie
void Array::remove(int index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    for (int i = index; i < size - 1; ++i) {
        data[i] = data[i + 1];
    }
    --size;
}

// Metoda do ustawiania wartości
void Array::set(int index, int value) {
    if (index >= 0 && index < size) {
        data[index] = value;
    } else {
        throw std::out_of_range("Index out of range");
    }
}

// Metoda do pobierania wartości
int Array::get(int index) const {
    if (index >= 0 && index < size) {
        return data[index];
    } else {
        throw std::out_of_range("Index out of range");
    }
}

// Metoda do wyświetlania tablicy
void Array::display() const {
    for (int i = 0; i < size; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

// Metoda do zwracania rozmiaru tablicy
int Array::getSize() const {
    return size;
}
