#include <iostream>
#include "Array.h"

/*

Implementacja tablicy jako klasy w C++ jest rzeczywiście elegancka i daje dużą elastyczność. Możemy rozszerzyć tę klasę, dodając więcej funkcji i metod, aby uczynić ją bardziej funkcjonalną. Poniżej dodam kilka dodatkowych metod do klasy Array, takich jak dynamiczne dodawanie elementów, usuwanie elementów, i możliwość zmiany rozmiaru tablicy.


Wyjaśnienie:
- Dynamiczna zmiana rozmiaru: Metoda resize zmienia rozmiar wewnętrznej tablicy, kopiując istniejące elementy do nowej tablicy o większej pojemności.
- Dodawanie elementów: Metoda add dodaje element na końcu tablicy. Jeśli tablica jest pełna, jej rozmiar jest podwajany.
- Usuwanie elementów: Metoda remove usuwa element na podanym indeksie i przesuwa pozostałe elementy w lewo.
- Ustawianie wartości: Metoda set ustawia wartość na danym indeksie.
- Pobieranie wartości: Metoda get zwraca wartość z danego indeksu.
- Wyświetlanie tablicy: Metoda display wyświetla zawartość tablicy.
- Rozmiar tablicy: Metoda getSize zwraca aktualny rozmiar tablicy.

*/

/*

class Array {
private:
    int* data;
    int size;
    int capacity;

    // Metoda pomocnicza do zmiany rozmiaru tablicy
    void resize(int new_capacity) {
        int* new_data = new int[new_capacity];
        for (int i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

public:
    // Konstruktor
    Array(int initial_capacity = 10) {
        size = 0;
        capacity = initial_capacity;
        data = new int[capacity];
    }

    // Destruktor
    ~Array() {
        delete[] data;
    }

    // Metoda do dodawania elementu
    void add(int value) {
        if (size == capacity) {
            resize(capacity * 2);
        }
        data[size++] = value;
    }

    // Metoda do usuwania elementu na danym indeksie
    void remove(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        for (int i = index; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }
        --size;
    }

    // Metoda do ustawiania wartości
    void set(int index, int value) {
        if (index >= 0 && index < size) {
            data[index] = value;
        } else {
            throw std::out_of_range("Index out of range");
        }
    }

    // Metoda do pobierania wartości
    int get(int index) const {
        if (index >= 0 && index < size) {
            return data[index];
        } else {
            throw std::out_of_range("Index out of range");
        }
    }

    // Metoda do wyświetlania tablicy
    void display() const {
        for (int i = 0; i < size; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }

    // Metoda do zwracania rozmiaru tablicy
    int getSize() const {
        return size;
    }
};

*/

int main() {
    Array arr;

    // Dodawanie elementów do tablicy
    for (int i = 0; i < 15; ++i) {
        arr.add(i + 1);
    }

    std::cout << "Tablica po dodaniu elementów: ";
    arr.display();

    // Usuwanie elementu
    arr.remove(5);

    std::cout << "Tablica po usunięciu elementu o indeksie 5: ";
    arr.display();

    // Ustawianie wartości
    arr.set(2, 42);

    std::cout << "Tablica po ustawieniu wartości 42 na indeksie 2: ";
    arr.display();

    // Pobieranie wartości
    int value = arr.get(2);
    std::cout << "Wartość na indeksie 2: " << value << std::endl;

    return 0;
}



/*

Kompilacja i uruchomienie
Aby skompilować program, użyj poniższego polecenia:

sh
Skopiuj kod
g++ main.cpp Array.cpp -o main

*/