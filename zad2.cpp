#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <iostream>
#include "Array.h"

#include "SinglyLinkedList.h"

#include "DoublyLinkedList.h"

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

enum class Mode {
    MENU,
    SINGLY_LINKED_LIST,
    DOUBLY_LINKED_LIST,
    ARRAY
};

class CircleSimulation : public olc::PixelGameEngine {
public:
    CircleSimulation() {
        sAppName = "Circle Simulation";
    }

private:
    SinglyLinkedList circles;
    SinglyLinkedList singlyLinkedList;
    DoublyLinkedList doublyLinkedList;
    Array circleArray;
    Mode mode = Mode::MENU;

public:
    bool OnUserCreate() override {
        srand(time(0));
        // for (int i = 0; i < 20; ++i) {
        //     float x = rand() % ScreenWidth();
        //     float y = rand() % ScreenHeight();
        //     float vx = (rand() % 100 - 25) / 1.0f;
        //     float vy = (rand() % 100 - 25) / 1.0f;
        //     float radius = 10.0f;
        //     olc::Pixel color = olc::Pixel(rand() % 256, rand() % 256, rand() % 256); // Losowy kolor
        //     circles.insert(Circle(x, y, vx, vy, radius, color));
        // }
        return true;
    }

    void initCircles() {
        for (int i = 0; i < 20; ++i) {
            float x = rand() % ScreenWidth();
            float y = rand() % ScreenHeight();
            float vx = (rand() % 200 - 20) / 1.0f; // Większa prędkość
            float vy = (rand() % 200 - 20) / 1.0f; // Większa prędkość
            float radius = 10.0f;
            olc::Pixel color = olc::Pixel(rand() % 256, rand() % 256, rand() % 256); // Losowy kolor
            Circle circle(x, y, vx, vy, radius, color);
            if (mode == Mode::SINGLY_LINKED_LIST) {
                singlyLinkedList.insert(circle);
            } else if (mode == Mode::DOUBLY_LINKED_LIST) {
                doublyLinkedList.insert(circle);
            } else if (mode == Mode::ARRAY) {
               circleArray.add(circle);
            }
        }
    }

    bool OnUserUpdate(float fElapsedTime) override {
        
        Clear(olc::BLACK);
        bool quit =  false;

        if (mode == Mode::MENU) {
            DrawString(15, 15, "1. Array", olc::GREEN);
            DrawString(15, 30, "2. Singly Linked List", olc::GREEN);
            DrawString(15, 45, "3. Doubly Linked List", olc::GREEN);

            DrawString(15, 60, "q - exit", olc::GREEN);
            
            if (GetKey(olc::Key::K1).bPressed) {
                mode = Mode::ARRAY;
                initCircles();
            } else if (GetKey(olc::Key::K2).bPressed) {
                mode = Mode::SINGLY_LINKED_LIST;
                initCircles();
            } else if (GetKey(olc::Key::K3).bPressed) {
                mode = Mode::DOUBLY_LINKED_LIST;
                initCircles();
            } else if (GetKey(olc::Key::Q).bReleased) {
			    quit = true;
            }
        } else {
            DrawString(15, 15, "ESC - back to main menu", olc::GREEN);

            updateCircles(fElapsedTime);
            renderCircles();

            // Powrót do menu po naciśnięciu klawisza ESC
            if (GetKey(olc::Key::ESCAPE).bPressed) {
                mode = Mode::MENU;
                singlyLinkedList = SinglyLinkedList();
                doublyLinkedList = DoublyLinkedList();
                circleArray = Array();
            }
        }



        // // Aktualizacja pozycji kółek
        // Node* current = circles.getHead();
        // while (current != nullptr) {
        //     if (current->data.active) {
        //         current->data.update(fElapsedTime);
        //     }
        //     current = current->next;
        // }

        // // Sprawdzanie kolizji i dezaktywacja kółek
        // Node* node1 = circles.getHead();
        // while (node1 != nullptr) {
        //     Node* node2 = node1->next;
        //     while (node2 != nullptr) {
        //         if (node1->data.active && node2->data.active && node1->data.isColliding(node2->data)) {
        //             node1->data.active = false;
        //             node2->data.active = false;
        //         }
        //         node2 = node2->next;
        //     }
        //     node1 = node1->next;
        // }
 
        // // Usuwanie dezaktywowanych kółek
        // Node* prev = nullptr;
        // current = circles.getHead();
        // while (current != nullptr) {
        //     Node* next = current->next;
        //     if (!current->data.active) {
        //         circles.remove(prev, current);
        //     } else {
        //         prev = current;
        //     }
        //     current = next;
        // }

        // Rysowanie kółek
        // current = circles.getHead();
        // while (current != nullptr) {
        //     if (current->data.active) {
        //         FillCircle(current->data.x, current->data.y, current->data.radius, current->data.color);
        //     }
        //     current = current->next;
        // }

        return !quit;
    }

    void updateCircles(float fElapsedTime) {
        if (mode == Mode::SINGLY_LINKED_LIST) {
            Node* current = singlyLinkedList.getHead();
            while (current != nullptr) {
                if (current->data.active) {
                    current->data.update(fElapsedTime);
                }
                current = current->next;
            }

            Node* node1 = singlyLinkedList.getHead();
            while (node1 != nullptr) {
                Node* node2 = node1->next;
                while (node2 != nullptr) {
                    if (node1->data.active && node2->data.active && node1->data.isColliding(node2->data)) {
                        node1->data.active = false;
                        node2->data.active = false;
                    }
                    node2 = node2->next;
                }
                node1 = node1->next;
            }

            Node* prev = nullptr;
            current = singlyLinkedList.getHead();
            while (current != nullptr) {
                Node* next = current->next;
                if (!current->data.active) {
                    singlyLinkedList.remove(prev, current);
                } else {
                    prev = current;
                }
                current = next;
            }
        } else if (mode == Mode::DOUBLY_LINKED_LIST) {
            DNode* current = doublyLinkedList.getHead();
            while (current != nullptr) {
                if (current->data.active) {
                    current->data.update(fElapsedTime);
                }
                current = current->next;
            }

            DNode* node1 = doublyLinkedList.getHead();
            while (node1 != nullptr) {
                DNode* node2 = node1->next;
                while (node2 != nullptr) {
                    if (node1->data.active && node2->data.active && node1->data.isColliding(node2->data)) {
                        node1->data.active = false;
                        node2->data.active = false;
                    }
                    node2 = node2->next;
                }
                node1 = node1->next;
            }

            DNode* prev = nullptr;
            current = doublyLinkedList.getHead();
            while (current != nullptr) {
                DNode* next = current->next;
                if (!current->data.active) {
                    doublyLinkedList.remove(prev, current);
                } else {
                    prev = current;
                }
                current = next;
            }
        } else if (mode == Mode::ARRAY) {
            for (int i = 0; i < circleArray.getSize(); ++i) {
                if (circleArray.get(i).active) {
                    circleArray.get(i).update(fElapsedTime);
                }
            }

            for (int i = 0; i < circleArray.getSize(); ++i) {
                for (int j = i + 1; j < circleArray.getSize(); ++j) {
                    if (circleArray.get(i).active && circleArray.get(j).active && circleArray.get(i).isColliding(circleArray.get(j))) {
                        circleArray.get(i).active = false;
                        circleArray.get(j).active = false;
                    }
                }
            }

            for (int i = 0; i < circleArray.getSize();) {
                if (!circleArray.get(i).active) {
                    circleArray.remove(i);
                } else {
                    ++i;
                }
            }
        }
    }


    void renderCircles() {

        if (mode == Mode::SINGLY_LINKED_LIST) {
            Node* current = nullptr;
            current = singlyLinkedList.getHead();
            while (current != nullptr) {
                if (current->data.active) {
                    FillCircle(current->data.x, current->data.y, current->data.radius, current->data.color);
                }
                current = current->next;
            }
        } else if (mode == Mode::DOUBLY_LINKED_LIST) {
            DNode* current = nullptr;
            current = doublyLinkedList.getHead();
            while (current != nullptr) {
                if (current->data.active) {
                    FillCircle(current->data.x, current->data.y, current->data.radius, current->data.color);
                }
                current = current->next;
            }
        } else if (mode == Mode::ARRAY) {
            for (int i = 0; i < circleArray.getSize(); ++i) {
                if (circleArray.get(i).active) {
                    FillCircle(circleArray.get(i).x, circleArray.get(i).y, circleArray.get(i).radius, circleArray.get(i).color);
                }
            }
        }
    }


};


int main() {


/*

Implementacja tablicy jako klasy w C++. 

Wyjaśnienie:
- Dynamiczna zmiana rozmiaru: Metoda resize zmienia rozmiar wewnętrznej tablicy, kopiując istniejące elementy do nowej tablicy o większej pojemności.
- Dodawanie elementów: Metoda add dodaje element na końcu tablicy. Jeśli tablica jest pełna, jej rozmiar jest podwajany.
- Usuwanie elementów: Metoda remove usuwa element na podanym indeksie i przesuwa pozostałe elementy w lewo.
- Ustawianie wartości: Metoda set ustawia wartość na danym indeksie.
- Pobieranie wartości: Metoda get zwraca wartość z danego indeksu.
- Wyświetlanie tablicy: Metoda display wyświetla zawartość tablicy.
- Rozmiar tablicy: Metoda getSize zwraca aktualny rozmiar tablicy.

*/

    // Array arr;

    // // Dodawanie elementów do tablicy
    // for (int i = 0; i < 15; ++i) {
    //     arr.add(i + 1);
    // }

    // std::cout << "Tablica po dodaniu elementów: ";
    // arr.display();

    // // Usuwanie elementu
    // arr.remove(5);

    // std::cout << "Tablica po usunięciu elementu o indeksie 5: ";
    // arr.display();

    // // Ustawianie wartości
    // arr.set(2, 42);

    // std::cout << "Tablica po ustawieniu wartości 42 na indeksie 2: ";
    // arr.display();

    // // Pobieranie wartości
    // int value = arr.get(2);
    // std::cout << "Wartość na indeksie 2: " << value << std::endl;


    // // Lista jednokierunkowa
    // SinglyLinkedList list;
    // list.insert(3);
    // list.insert(5);
    // list.insert(7);

    // std::cout << "Lista jednokierunkowa: ";
    // list.display();

    // list.remove(5);
    // std::cout << "Po usunięciu 5: ";
    // list.display();


    // // Lista dwukierunkowa
    // DoublyLinkedList list;
    // list.insert(3);
    // list.insert(5);
    // list.insert(7);

    // std::cout << "Lista dwukierunkowa: ";
    // list.display();

    // list.remove(5);
    // std::cout << "Po usunięciu 5: ";
    // list.display();


    CircleSimulation demo;
    if (demo.Construct(600, 600, 1, 1))
        demo.Start();
    return 0;


    return 0;
}

/*
Wyjaśnienie:

Klasa przeniesiona została do oddzielnego pliku, co pomoże w organizacji kodu i ułatwi jego utrzymanie. 

Podzieliono kod na dwa pliki:
Plik nagłówkowy Array.h: Zawiera deklaracje klasy Array oraz metod i zmiennych członkowskich.
Plik źródłowy Array.cpp: Zawiera definicje metod klasy Array.

Plik główny zad2.cpp: Zawiera funkcję main, która testuje funkcjonalność klasy Array.


W celu uatrakcyjnienia pracy z listami jednokierunkowymi i dwukierunkowymi, wykorzystaliśmy bibliotekę olcPixelGameEngine do wizualizacji dynamicznych kółek na ekranie. Poniżej przedstawiam podsumowanie naszych działań:

1. **Klasa `Circle`**:
   - Utworzyliśmy klasę `Circle`, która reprezentuje pojedyncze kółko. Każdy obiekt `Circle` posiada:
     - Pozycję `(x, y)`.
     - Prędkość `(vx, vy)`.
     - Promień `radius`.
     - Kolor `color`.
     - Atrybut `active`, który wskazuje, czy kółko jest aktywne (czy powinno być wyświetlane).
   - Klasa `Circle` zawiera metody:
     - `update(float fElapsedTime)`: Aktualizuje pozycję kółka na podstawie jego prędkości.
     - `isColliding(const Circle& other)`: Sprawdza, czy dwa kółka się zderzają.

2. **Lista jednokierunkowa (`SinglyLinkedList`)**:
   - Utworzyliśmy klasę `SinglyLinkedList`, która zarządza dynamiczną listą jednokierunkową zawierającą obiekty `Circle`.
   - Klasa `SinglyLinkedList` zawiera:
     - Wskaźnik `head` na pierwszy element listy.
     - Metody do zarządzania listą:
       - `insert(const Circle& value)`: Dodaje nowe kółko na początek listy.
       - `remove(Node* prevNode, Node* currentNode)`: Usuwa węzeł z listy.
       - `display() const`: Wyświetla zawartość listy (do debugowania).
       - `getHead() const`: Zwraca wskaźnik na pierwszy węzeł listy.

3. **Klasa `CircleSimulation`**:
   - Utworzyliśmy klasę `CircleSimulation`, która dziedziczy po `olc::PixelGameEngine` i zarządza wizualizacją kółek.
   - Klasa `CircleSimulation` zawiera:
     - Obiekt `SinglyLinkedList` do przechowywania kółek.
     - Metody:
       - `OnUserCreate()`: Inicjalizuje kółka, tworząc je w losowych pozycjach z losowymi prędkościami i kolorami.
       - `OnUserUpdate(float fElapsedTime)`: Aktualizuje pozycje kółek, sprawdza kolizje, usuwa dezaktywowane kółka i rysuje aktywne kółka na ekranie.

### Kroki, które wykonaliśmy:

1. **Stworzenie klasy `Circle`**:
   - Klasa ta reprezentuje pojedyncze kółko, zawierając dane dotyczące pozycji, prędkości, promienia, koloru oraz aktywności.
   - Dodaliśmy metody do aktualizacji pozycji kółka i sprawdzania kolizji.

2. **Stworzenie klasy `SinglyLinkedList`**:
   - Klasa ta zarządza dynamiczną listą jednokierunkową zawierającą obiekty `Circle`.
   - Dodaliśmy metody do dodawania, usuwania i wyświetlania kółek.

3. **Stworzenie klasy `CircleSimulation`**:
   - Klasa ta dziedziczy po `olc::PixelGameEngine` i zarządza wizualizacją kółek.
   - Inicjalizuje kółka w losowych pozycjach z losowymi prędkościami i kolorami.
   - Aktualizuje pozycje kółek, sprawdza kolizje, usuwa dezaktywowane kółka i rysuje aktywne kółka na ekranie.

### Kompilacja i uruchomienie

Aby skompilować program, użyj poniższego polecenia:

g++ -o start zad2.cpp Array.cpp SinglyLinkedList.cpp DoublyLinkedList.cpp  -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

//g++ zad2.cpp Array.cpp SinglyLinkedList.cpp DoublyLinkedList.cpp -o main



*/