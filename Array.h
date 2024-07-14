#ifndef ARRAY_H
#define ARRAY_H

#include "Circle.h"

class Array {
private:
    Circle* data;
    int size;
    int capacity;

    // Metoda pomocnicza do zmiany rozmiaru tablicy
    void resize(int new_capacity);

public:
    // Konstruktor
    Array(int initial_capacity = 10);

    // Destruktor
    ~Array();

    // Metoda do dodawania elementu
    // void add(int value);
    void add(const Circle& value);

    // Metoda do usuwania elementu na danym indeksie
    void remove(int index);

    // Metoda do ustawiania wartości
    void set(int index, int value);

    // Metoda do pobierania wartości
    // int get(int index) const;
    Circle& get(int index) const;

    // Metoda do wyświetlania tablicy
    void display() const;

    // Metoda do zwracania rozmiaru tablicy
    int getSize() const;
};

#endif // ARRAY_H
