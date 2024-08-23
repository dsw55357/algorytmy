#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <iostream>
#include <chrono>

#include "Array.h"

#include "SinglyLinkedList.h"

#include "DoublyLinkedList.h"

#include "ArrayLinkedList.h"

#include "ArrayDoublyLinkedList.h"

#include "Queue.h"
#include "SinglyLinkedListQueue.h"

#include "ArrayStack.h"
#include "SinglyLinkedListStack.h"



#include "MaxHeap.h"

// Użycie mapy znaków PL
// std::wstring ProcessInput(const olc::PixelGameEngine& pge) {
//     std::wstring input = L"";
    
//     for (const auto& [key, value] : vKeyboardMapPL) {
//         if (pge.GetKey(key).bPressed) {
//             input += value;
//         }
//     }
    
//     return input;
// }

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
    ARRAY,
    ARRAY_LINKED_LIST,
    ARRAY_DOUBLY_LINKED_LIST,
    MAX_HEAP,
    ARRAY_QUEUE,
    LIST_QUEUE,
    STACK,
    STACK_LIST
};

/*

Klasa CircleSimulation zarządza wizualizacją kółek, umożliwiając użytkownikowi wybór między listą jednokierunkową, dwukierunkową i tablicą dynamiczną.

*/

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
    ArrayLinkedList arrayLinkedList;
    ArrayDoublyLinkedList arrayDoublyLinkedList;

    CircleQueue arrayQueue;
    SinglyLinkedListQueue<Circle> listQueue;

    ArrayStack arrayStack;
    SinglyLinkedListStack listStack;

    MaxHeap maxHeap;
    std::vector<Circle> originalCircles;
    std::vector<Circle> sortedCircles;
    
    Mode mode = Mode::MENU;
    std::string performanceMessage {".."};

    std::string errorMessage; // Dodana zmienna do przechowywania komunikatu o błędzie


public:
    bool OnUserCreate() override {
        srand(time(0));
        return true;
    }

    void initCircles() {
        std::cout << "initCircles() " << std::endl;
        for (int i = 0; i < 30; ++i) {
            float x = rand() % ScreenWidth()/2;
            float y = rand() % ScreenHeight();
            float vx = (rand() % 200 - 20) / 1.0f; // Większa prędkość
            float vy = (rand() % 200 - 20) / 1.0f; // Większa prędkość
            float radius = (rand() % 20) + 5; // Losowa średnica od 5 do 25
            olc::Pixel color = olc::Pixel(rand() % 256, rand() % 256, rand() % 256); // Losowy kolor
            Circle circle(x, y, vx, vy, radius, color);
            if (mode == Mode::SINGLY_LINKED_LIST) {
                singlyLinkedList.insert(circle);
            } else if (mode == Mode::DOUBLY_LINKED_LIST) {
                doublyLinkedList.insert(circle);
            } else if (mode == Mode::ARRAY) {
               circleArray.add(circle);
            } else if (mode == Mode::ARRAY_LINKED_LIST) {
                arrayLinkedList.insert(circle);
            } else if (mode == Mode::ARRAY_DOUBLY_LINKED_LIST) {
                arrayDoublyLinkedList.insert(circle);
            } else if (mode == Mode::MAX_HEAP) {
                maxHeap.insert(circle);
                originalCircles.push_back(circle);
            } // ARRAY_QUEUE 
              else if (mode == Mode::ARRAY_QUEUE) {
                arrayQueue.enqueue(circle);
                // std::cout << "initCircles() " << i << std::endl;
            }  else if (mode == Mode::LIST_QUEUE) {
                listQueue.enqueue(circle);
            }  else if (mode == Mode::STACK) {
                arrayStack.push(circle);
            } else if (mode == Mode::STACK_LIST) {
                listStack.push(circle);
            }
        }
    }

    bool OnUserUpdate(float fElapsedTime) override {
        
        Clear(olc::BLACK);
        bool quit =  false;

        // std::wstring input = ProcessInput(*this);
        
        // if (!input.empty()) {
        //     DrawString(15, ScreenHeight() - 10, input, olc::WHITE);
        // }

        if (mode == Mode::MENU) {
            int i = 15;
            // Struktury danych
            DrawString(15, i, "Struktury danych:", olc::GREEN, 2);
            DrawString(15, 10+i*2, "A >> Tablica", olc::GREEN);
            DrawString(15, 10+i*3, "B >> Lista jednokierunkowa", olc::GREEN);
            DrawString(15, 10+i*4, "B + CTRL >> Lista dwukierunkowa", olc::GREEN);
            DrawString(15, 10+i*5, "C >> Lista jednokierunkowa (realizacja za pomoca tablicy)", olc::GREEN);
            DrawString(15, 10+i*6, "C + CTRL >> Lista dwukierunkowa (realizacja za pomoca tablicy)", olc::GREEN);
            DrawString(15, 10+i*8, "D >> Kolejka (realizacja za pomoc tablicy)", olc::GREEN);
            DrawString(15, 10+i*9, "D + CTRL >> Kolejka (realizacja za pomoca listy)", olc::GREEN);
            DrawString(15, 10+i*10, "E >> Stos (realizacja za pomoca tablicy)", olc::GREEN);
            DrawString(15, 10+i*11, "E + CTRL >> Stos (realizacja za pomoca listy)", olc::GREEN);
            DrawString(15, 10+i*7, "F >> Kopiec", olc::GREEN);
            DrawString(15, 10+i*12, "Q >> exit", olc::GREEN);
            
            if (GetKey(olc::Key::A).bPressed) {
                mode = Mode::ARRAY;
                initCircles();
            } else if (GetKey(olc::Key::CTRL).bHeld && GetKey(olc::Key::B).bPressed) {
                mode = Mode::DOUBLY_LINKED_LIST;
                initCircles();
            } else if (GetKey(olc::Key::B).bPressed) {
                mode = Mode::SINGLY_LINKED_LIST;
                initCircles();
            } else if (GetKey(olc::Key::CTRL).bHeld && GetKey(olc::Key::C).bPressed) {
                mode = Mode::ARRAY_DOUBLY_LINKED_LIST;
                initCircles();
            } else if (GetKey(olc::Key::C).bPressed) {
                mode = Mode::ARRAY_LINKED_LIST;
                initCircles();
            } else if (GetKey(olc::Key::F).bPressed) {
                mode = Mode::MAX_HEAP;
                maxHeap = MaxHeap();
                originalCircles.clear();
                sortedCircles.clear();
                initCircles();
                // Wyodrębnij wszystkie elementy z kopca, aby je posortować
                while (!maxHeap.isEmpty()) {
                    sortedCircles.push_back(maxHeap.extractMax());
                }     
            } else if (GetKey(olc::Key::CTRL).bHeld &&  GetKey(olc::Key::D).bPressed) {
                mode = Mode::LIST_QUEUE;
                initCircles();             
            } else if (GetKey(olc::Key::D).bPressed) {
                mode = Mode::ARRAY_QUEUE;
                //arrayQueue = CircleQueue(100); // Resetowanie kolejki
                initCircles();             
            } else if (GetKey(olc::Key::CTRL).bHeld && GetKey(olc::Key::E).bPressed) {
                mode = Mode::STACK_LIST;
                listStack.clear(); // Resetowanie stosu
                initCircles();      
            } else if (GetKey(olc::Key::E).bPressed) {
                mode = Mode::STACK;
                arrayStack.clear(); // Resetowanie stosu
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
                arrayLinkedList = ArrayLinkedList();
                arrayDoublyLinkedList = ArrayDoublyLinkedList();
                //arrayQueue = CircleQueue(100); // 
                maxHeap = MaxHeap();
                originalCircles.clear();
                sortedCircles.clear();
            }
        }

        DrawString(ScreenWidth()-300, ScreenHeight()-15, "Powered by olcPixelGameEngine, 2024(7)", olc::CYAN );

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
        using namespace std::chrono;

        if (mode == Mode::SINGLY_LINKED_LIST) { // Lista jednokierunkowa

            auto start = high_resolution_clock::now();

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
            
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end - start).count();
            performanceMessage = "Lista jednokierunkowa [" + std::to_string(duration) + "] us";
            // std::cout << duration << std::endl;
            DrawString(15, 45, performanceMessage, olc::YELLOW);
            // Wyświetlenie liczby aktualnie wyświetlanych obiektów
            DrawString(10, ScreenWidth()-50, "Number of Circles: " + std::to_string(singlyLinkedList.getSize()), olc::CYAN);


        } else if (mode == Mode::DOUBLY_LINKED_LIST) { // Lista dwukierunkowa
            using namespace std::chrono; 
            const auto start{std::chrono::steady_clock::now()};

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

            const auto end{std::chrono::steady_clock::now()};
            //const std::chrono::duration<double> elapsed_seconds{end - start};
            auto duration = duration_cast<microseconds>(end - start).count();
            performanceMessage = "Lista dwukierunkowa [" + std::to_string(duration) + "] us";
            DrawString(15, 45, performanceMessage, olc::YELLOW);
            // Wyświetlenie liczby aktualnie wyświetlanych obiektów
            DrawString(10, ScreenWidth()-50, "Number of Circles: " + std::to_string(doublyLinkedList.getSize()), olc::CYAN);


        } else if (mode == Mode::ARRAY) { // Tablica

            using namespace std::chrono;
            const auto start{std::chrono::steady_clock::now()};

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

            const auto end{std::chrono::steady_clock::now()};
            //const std::chrono::duration<double> elapsed_seconds{end - start};
            auto duration = duration_cast<microseconds>(end - start).count();
            performanceMessage = "Tablica [" + std::to_string(duration) + "] us";
            DrawString(15, 45, performanceMessage, olc::YELLOW);
            // Wyświetlenie liczby aktualnie wyświetlanych obiektów
            DrawString(10, ScreenWidth()-50, "Number of Circles: " + std::to_string(circleArray.getSize()), olc::CYAN);



    } else if (mode == Mode::ARRAY_LINKED_LIST) { // Lista jednokierunkowa (realizacja za pomoca tablicy)

            using namespace std::chrono;
            const auto start{std::chrono::steady_clock::now()};    

            for (int i = 0; i < arrayLinkedList.getSize(); ++i) {
                if (arrayLinkedList.get(i).active) {
                    arrayLinkedList.get(i).update(fElapsedTime);
                }
            }

            for (int i = 0; i < arrayLinkedList.getSize(); ++i) {
                for (int j = i + 1; j < arrayLinkedList.getSize(); ++j) {
                    if (arrayLinkedList.get(i).active && arrayLinkedList.get(j).active && arrayLinkedList.get(i).isColliding(arrayLinkedList.get(j))) {
                        arrayLinkedList.get(i).active = false;
                        arrayLinkedList.get(j).active = false;
                    }
                }
            }

            for (int i = 0; i < arrayLinkedList.getSize();) {
                if (!arrayLinkedList.get(i).active) {
                    arrayLinkedList.remove(i);
                } else {
                    ++i;
                }
            }
            const auto end{std::chrono::steady_clock::now()};
            auto duration = duration_cast<microseconds>(end - start).count();
            performanceMessage = "Lista jednokierunkowa (realizacja za pomoca tablicy) [" + std::to_string(duration) + "] us";
            DrawString(15, 45, performanceMessage, olc::YELLOW);
            // Wyświetlenie liczby aktualnie wyświetlanych obiektów
            DrawString(10, ScreenWidth()-50, "Number of Circles: " + std::to_string(arrayLinkedList.getSize()), olc::CYAN);


    } else if (mode == Mode::ARRAY_DOUBLY_LINKED_LIST) { // Lista dwukierunkowa (realizacja za pomoca tablicy)

            using namespace std::chrono;
            const auto start{std::chrono::steady_clock::now()};    

            for (int i = 0; i < arrayDoublyLinkedList.getSize(); ++i) {
                    if (arrayDoublyLinkedList.get(i).active) {
                        arrayDoublyLinkedList.get(i).update(fElapsedTime);
                    }
                }

            for (int i = 0; i < arrayDoublyLinkedList.getSize(); ++i) {
                for (int j = i + 1; j < arrayDoublyLinkedList.getSize(); ++j) {
                    if (arrayDoublyLinkedList.get(i).active && arrayDoublyLinkedList.get(j).active && arrayDoublyLinkedList.get(i).isColliding(arrayDoublyLinkedList.get(j))) {
                        arrayDoublyLinkedList.get(i).active = false;
                        arrayDoublyLinkedList.get(j).active = false;
                    }
                }
            }

            for (int i = 0; i < arrayDoublyLinkedList.getSize();) {
                if (!arrayDoublyLinkedList.get(i).active) {
                    arrayDoublyLinkedList.remove(i);
                } else {
                    ++i;
                }
            }

            const auto end{std::chrono::steady_clock::now()};
            auto duration = duration_cast<microseconds>(end - start).count();
            performanceMessage = "Lista dwukierunkowa (realizacja za pomoca tablicy) [" + std::to_string(duration) + "] us";
            DrawString(15, 45, performanceMessage, olc::YELLOW);

            // Wyświetlenie liczby aktualnie wyświetlanych obiektów
            DrawString(10, ScreenWidth()-50, "Number of Circles: " + std::to_string(arrayDoublyLinkedList.getSize()), olc::CYAN);

        } else if (mode == Mode::ARRAY_QUEUE) { // Kolejka - realizacja za pomocą tablicy
            using namespace std::chrono;
            const auto start{std::chrono::steady_clock::now()};             

            // Aktualizacja kółek w kolejce
            arrayQueue.forEach([fElapsedTime](Circle& circle) {
                if (circle.active) {                  
                    circle.update(fElapsedTime);
                }
            });

            arrayQueue.forEach([this](Circle& circle1) {
                arrayQueue.forEach([&circle1](Circle& circle2) {
                    if (&circle1 != &circle2 && circle1.active && circle2.active && circle1.isColliding(circle2)) {
                        circle1.active = false;
                        circle2.active = false;
                    }
                });
            });

            arrayQueue.removeIf([](const Circle& circle) {
                return !circle.active;
            });    

            const auto end {std::chrono::steady_clock::now()};
            auto duration = duration_cast<microseconds>(end - start).count();
            performanceMessage = "Kolejka (realizacja za pomoca tablicy) [" + std::to_string(duration) + "] us";
            DrawString(15, 45, performanceMessage, olc::YELLOW);

        } else if (mode == Mode::LIST_QUEUE) { // Kolejka (realizacja za pomoca listy)
            
            using namespace std::chrono;
            const auto start{std::chrono::steady_clock::now()};   

            // Aktualizacja kółek w kolejce
            listQueue.forEach([fElapsedTime](Circle& circle) {
                if (circle.active) {
                    circle.update(fElapsedTime);
                }
            });

            // Sprawdzanie kolizji i dezaktywacja kółek
            listQueue.forEach([this](Circle& circle1) {
                listQueue.forEach([&circle1](Circle& circle2) {
                    if (&circle1 != &circle2 && circle1.active && circle2.active && circle1.isColliding(circle2)) {
                        circle1.active = false;
                        circle2.active = false;
                    }
                });
            });

            // Usuwanie nieaktywnych kółek
            listQueue.removeIf([](const Circle& circle) {
                return !circle.active;
            });

            const auto end {std::chrono::steady_clock::now()};
            auto duration = duration_cast<microseconds>(end - start).count();
            performanceMessage = "Kolejka (realizacja za pomoca listy) [" + std::to_string(duration) + "] us";
            DrawString(15, 45, performanceMessage, olc::YELLOW);


        } else if (mode == Mode::STACK) { // Stos (realizacja za pomoca tablicy)
            try {
                // Obsługa klawisza R do dodania losowych punktów
                if (GetKey(olc::A).bPressed) {
                    // Dodaj 1 punkt
                    arrayStack.push(addRandomPoint(1));
                }
                // Obsługa klawisza D do usunięcie elementu
                if (GetKey(olc::D).bPressed) {
                    // popPoint(arrayStack, 1); // Usuń punkt
                    arrayStack.pop();
                }
                DrawString(15, ScreenHeight()-15, "A - dodaj / D - zdejmij ze stosu", olc::GREY, 1);

                using namespace std::chrono;
                const auto start{std::chrono::steady_clock::now()};   

                // Aktualizacja kółek w stosie
                arrayStack.forEach([fElapsedTime](Circle& circle) {
                    if (circle.active) {
                        circle.update(fElapsedTime);
                    }
                });

                const auto end {std::chrono::steady_clock::now()};
                auto duration = duration_cast<microseconds>(end - start).count();
                performanceMessage = "Stos (realizacja za pomoca tablicy) [" + std::to_string(duration) + "] us";
                DrawString(15, 45, performanceMessage, olc::YELLOW);

            } catch (const std::runtime_error& e) {
                errorMessage = e.what(); // Przechwycenie wyjątku i zapisanie komunikatu
            }

            // Wyświetlenie komunikatu o błędzie, jeśli istnieje
            if (!errorMessage.empty()) {
                if (arrayStack.size() > 0) {
                    errorMessage.clear();
                } else {
                    // DrawString(ScreenWidth()/2, ScreenHeight()/2, errorMessage, olc::RED, 2);
                    uint32_t scale = 2;
                    // Get the size of the text
                    olc::vi2d textSize = GetTextSize(errorMessage);

                    // Calculate the position to draw the text so that it is centered
                    int textX = ScreenWidth()/2 - (textSize.x * scale) / 2;
                    int textY = ScreenHeight()/2 - (textSize.y * scale) / 2;

                    // Draw the text
                    DrawString(textX, textY, errorMessage, olc::RED, scale);
                }
            }

        } else if (mode == Mode::STACK_LIST) { // Stos (realizacja za pomoca listy)
            try {
                // Obsługa klawisza R do dodania losowych punktów
                if (GetKey(olc::A).bPressed) {
                    // Dodaj 1 punkt
                    listStack.push(addRandomPoint(1));
                }
                // Obsługa klawisza D do usunięcie elementu
                if (GetKey(olc::D).bPressed) {
                    // popPoint(arrayStack, 1); 
                    // Usuń punkt
                    listStack.pop();
                }

                // Stos (realizacja za pomoca listy)

                DrawString(15, ScreenHeight()-15, "A - dodaj / D - zdejmij ze stosu", olc::GREY, 1);

                using namespace std::chrono;
                const auto start{std::chrono::steady_clock::now()};  

                // Aktualizacja kółek w stosie
                listStack.forEach([fElapsedTime](Circle& circle) {
                    if (circle.active) {
                        circle.update(fElapsedTime);
                    }
                });

                const auto end {std::chrono::steady_clock::now()};
                auto duration = duration_cast<microseconds>(end - start).count();
                performanceMessage = "Stos (realizacja za pomoca listy) [" + std::to_string(duration) + "] us";
                DrawString(15, 45, performanceMessage, olc::YELLOW);

            } catch (const std::runtime_error& e) {
                errorMessage = e.what(); // Przechwycenie wyjątku i zapisanie komunikatu
            }

            // Wyświetlenie komunikatu o błędzie, jeśli istnieje
            if (!errorMessage.empty()) {
                if (listStack.getSize() > 0) {
                    errorMessage.clear();
                } else {
                    uint32_t scale = 2;
                    // Get the size of the text
                    olc::vi2d textSize = GetTextSize(errorMessage);

                    // Calculate the position to draw the text so that it is centered
                    int textX = ScreenWidth()/2 - (textSize.x * scale) / 2;
                    int textY = ScreenHeight()/2 - (textSize.y * scale) / 2;

                    // Draw the text
                    DrawString(textX, textY, errorMessage, olc::RED, scale);
                    //DrawString(ScreenWidth()/2, ScreenHeight()/2, errorMessage, olc::RED, 2);
                }
            }



        } else if (mode == Mode::MAX_HEAP) {
            DrawLine(ScreenWidth() / 2, 0, ScreenWidth() / 2, ScreenHeight(), olc::GREEN);
        }

    }


    // Circle addRandomPoint(ArrayStack &arrayStack, int n=1) {
    Circle addRandomPoint( int n=1) {
        // for (int i = 0; i < n; ++i) {
            int x = rand() % ScreenWidth();
            int y = rand() % ScreenHeight();
            float vx = (rand() % 200 - 20) / 1.0f; // Większa prędkość
            float vy = (rand() % 200 - 20) / 1.0f; // Większa prędkość
            float radius = (rand() % 20) + 5; // Losowa średnica od 5 do 25
            olc::Pixel color = olc::Pixel(rand() % 256, rand() % 256, rand() % 256); // Losowy kolor
            Circle circle(x, y, vx, vy, radius, color);           
        // }
        return circle;
    }

    // void popPoint(ArrayStack &arrayStack, int n=1) {
    //     arrayStack.pop();
    // }


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
        } else if (mode == Mode::ARRAY_LINKED_LIST) {
            for (int i = 0; i < arrayLinkedList.getSize(); ++i) {
                if (arrayLinkedList.get(i).active) {
                    FillCircle(arrayLinkedList.get(i).x, arrayLinkedList.get(i).y, arrayLinkedList.get(i).radius, arrayLinkedList.get(i).color);
                }
            }
        } else if (mode == Mode::ARRAY_DOUBLY_LINKED_LIST) {
            for (int i = 0; i < arrayDoublyLinkedList.getSize(); ++i) {
                if (arrayDoublyLinkedList.get(i).active) {
                    FillCircle(arrayDoublyLinkedList.get(i).x, arrayDoublyLinkedList.get(i).y, arrayDoublyLinkedList.get(i).radius, arrayDoublyLinkedList.get(i).color);
                }
            }
        } else if (mode == Mode::ARRAY_QUEUE) {
            // Wyświetlenie kółek na ekranie
            arrayQueue.forEach([this](const Circle& circle) {
                FillCircle(circle.x, circle.y, circle.radius, circle.color);
            });
        } else if (mode == Mode::LIST_QUEUE) {
            // Wyświetlenie kółek na ekranie
            listQueue.forEach([this](const Circle& circle) {
                FillCircle(circle.x, circle.y, circle.radius, circle.color);
            });
        } else if (mode == Mode::STACK) {
            // Wyświetlenie kółek na ekranie
            arrayStack.forEach([this](const Circle& circle) {
                FillCircle(circle.x, circle.y, circle.radius, circle.color);
            });

            DrawString(ScreenWidth() - 200, 15, "Zajetosc stosu: " + std::to_string(arrayStack.size()), olc::YELLOW); 

        } else if (mode == Mode::STACK_LIST) {
        // listStack
            listStack.forEach([this](const Circle& circle) {
                FillCircle(circle.x, circle.y, circle.radius, circle.color);
            });

            DrawString(ScreenWidth() - 200, 15, "Zajetosc stosu: " + std::to_string(listStack.getSize()), olc::YELLOW); 
        
        } else if (mode == Mode::MAX_HEAP) {
            int x = 50;
            int y = 70; // Startowa współrzędna Y
            // Wyświetlenie wszystkich kółek w oryginalnych pozycjach, po lewej stronie
            for (const Circle& circle : originalCircles) {
                // FillCircle(circle.x, circle.y, circle.radius, circle.color);
                FillCircle(x, y, circle.radius, circle.color);
                y += (circle.radius * 2) + 10; // Zwiększanie Y, aby umieścić kolejne kółka poniżej, dodając odstęp 10
                if (y >= 500) {
                    x += (circle.radius * 2) + 10;
                    y = 70;
                }
            }
            
            // Wyświetlenie kółek posortowanych według promienia
            int offsetX = ScreenWidth() / 2;
            x = 50;
            y = 70; // Startowa współrzędna Y
            DrawString(offsetX + x, 15, "Pokopcowane..", olc::CYAN);
            for (const Circle& circle : sortedCircles) {
                FillCircle(offsetX + x, y, circle.radius, circle.color); // 50 to współrzędna X
                y += (circle.radius * 2) + 10; // Zwiększanie Y, aby umieścić kolejne kółka poniżej, dodając odstęp 10
                if (y >= 500) {
                    x += (circle.radius * 2) + 10;
                    y = 70;
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


###
zad.2 ad.d
- Kolejkę (realizacja za pomocą tablicy):
Kolejka to struktura danych, w której elementy są dodawane na końcu (enqueue) i usuwane z początku (dequeue). Kolejkę zaimplementujemy w oparciu o tablicę.
Kod zarządza aktualizacją, wykrywaniem kolizji oraz usuwaniem nieaktywnych elementów

- Kolejkę (realizacja za pomocą listy):
realizacja kolejki za pomocą listy jednokierunkowej, musimy najpierw zaimplementować klasę listy jednokierunkowej. Następnie użyjemy tej klasy do zarządzania kółkami w symulacji. 


###
zad.2 ad.e
- Stos (realizacja za pomocą tablicy)
- Stos (realizacja za pomocą listy)

###
zad.2 ad.f
realizacja kopca, który porządkuje elementy klasy Circle na podstawie promienia okręgu w kolejności od najwyższej do najniższej. 
Pionowa zielona linia dzieli ekran na elementy nieposortowane (lewa strona ekranu) od posortowanych (prawa strona ekranu).




### Kompilacja i uruchomienie

Aby skompilować program, użyj poniższego polecenia:

g++ -o start zad2.cpp Array.cpp SinglyLinkedList.cpp DoublyLinkedList.cpp ArrayLinkedList.cpp ArrayDoublyLinkedList.cpp MaxHeap.cpp Queue.cpp SinglyLinkedListQueue.cpp ArrayStack.h SinglyLinkedListStack.h  -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++20

*/