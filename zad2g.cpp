#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>

// g++ -o start tablica_haszująca.cpp  -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++20

/*

Przykład użycia tablicy haszującej do przechowywania i optymalizacji dróg w grafach może być przydatny w kontekście gier, gdzie postacie muszą poruszać się po złożonym terenie lub mapie. Rozważmy prosty przykład gry, w której postać musi znaleźć najkrótszą drogę między różnymi punktami na mapie.

Opis problemu:
Załóżmy, że mamy graf, gdzie węzły reprezentują różne lokacje na mapie (np. miasta, skrzyżowania, itp.), a krawędzie reprezentują drogi między tymi lokacjami. Chcemy znaleźć najkrótszą drogę między dwoma węzłami, ale jednocześnie chcemy uniknąć ponownego przeliczania tej drogi, jeśli już wcześniej ją znaleźliśmy.

Zastosowanie tablicy haszującej:
Możemy użyć tablicy haszującej, aby zapamiętać wcześniej obliczone najkrótsze ścieżki między parami węzłów. Kluczem w tablicy haszującej byłaby para węzłów (początkowy i końcowy), a wartością - wcześniej obliczona najkrótsza ścieżka.

Przykład implementacji:
Załóżmy, że mamy graf i chcemy przechowywać najkrótsze ścieżki między węzłami:


Jak działa ten przykład?

Graf:
Reprezentowany jako lista sąsiedztwa (adjList), gdzie addEdge dodaje krawędzie między węzłami z określonymi wagami.


Funkcja shortestPath:
Wykorzystuje algorytm Dijkstry do znalezienia najkrótszej ścieżki między dwoma węzłami.

Tablica haszująca (PathCache):
Przechowuje wcześniej obliczone ścieżki między parami węzłów. Jeśli zapytanie o najkrótszą ścieżkę między dwoma węzłami zostało już wcześniej wykonane, wynik jest zwracany bez ponownego przeliczania.
Kluczem jest para węzłów (start, end), a wartością - najkrótsza ścieżka (wektor węzłów).

Zalety:
Dzięki cache’owaniu wyników (przechowywaniu ich w tablicy haszującej) unikasz wielokrotnego przeliczania tych samych ścieżek, co jest szczególnie przydatne w dynamicznych grach, gdzie wiele zapytań o najkrótsze ścieżki może dotyczyć tych samych węzłów.

Zastosowanie w rzeczywistości:
Tego typu podejście może być stosowane w grach, gdzie postacie muszą często przemieszczać się między różnymi lokacjami, a wiele zapytań może dotyczyć tych samych punktów. Przechowywanie wcześniej obliczonych ścieżek pozwala znacznie zwiększyć wydajność, szczególnie gdy graf jest złożony, a algorytmy takie jak Dijkstra mogą być czasochłonne.



*/


// Struktura reprezentująca krawędź w grafie
struct Edge {
    int to;
    int weight;
};

// Hash funkcji do pary węzłów
struct NodePairHash {
    /*
    Wyrażenie std::hash<int>()(p.first) ^ std::hash<int>()(p.second); jest używane do generowania wartości haszującej dla pary wartości, w tym przypadku dla pary typu std::pair<int, int>. To wyrażenie łączy wartości haszujące dwóch liczb całkowitych (p.first i p.second) w jedną wartość haszującą przy użyciu operacji XOR (^).
    */
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ std::hash<int>()(p.second);
    }
    /*
    Składniki tego wyrażenia:
    std::hash<int>():

    std::hash<int> to specjalizacja szablonu std::hash dla typu int. std::hash jest funkcją haszującą dostarczoną przez standardową bibliotekę C++, która generuje wartość haszującą dla określonego typu (w tym przypadku int).
    std::hash<int>() tworzy obiekt funkcji haszującej, który można zastosować do liczby całkowitej.
    p.first i p.second:

    p.first i p.second to odpowiednio pierwszy i drugi element pary std::pair<int, int>.
    Na przykład, jeśli mamy std::pair<int, int> p = {3, 7};, to p.first będzie równe 3, a p.second będzie równe 7.
    std::hash<int>()(p.first):

    To wyrażenie stosuje funkcję haszującą do wartości p.first. Generuje wartość haszującą dla p.first.
    std::hash<int>()(p.second):

    To wyrażenie stosuje funkcję haszującą do wartości p.second. Generuje wartość haszującą dla p.second.
    Operator XOR (^):

    XOR (^) to bitowa operacja logiczna, która porównuje dwie liczby binarne i zwraca 1 na każdej pozycji bitowej, gdzie bity tych liczb różnią się (jeden jest 1, a drugi 0). Jeśli bity są identyczne, wynik jest 0.
    W kontekście wartości haszujących, użycie XOR służy do połączenia dwóch wartości haszujących w jedną, co daje bardziej zróżnicowany wynik.


    Generuje wartość haszującą dla pary std::pair<int, int>. Najpierw generowane są wartości haszujące dla p.first i p.second, a następnie są one łączone przy użyciu operacji XOR. Celem tego procesu jest uzyskanie unikalnej wartości haszującej dla pary dwóch liczb całkowitych, która może być użyta jako klucz w tablicy haszującej 

    Dlaczego używa się XOR?
    Operacja XOR jest często stosowana w takich sytuacjach, ponieważ:

    Jest szybka do obliczenia.
    Pomaga uniknąć prostych kolizji, które mogłyby wystąpić, gdybyśmy po prostu dodali lub przemnożyli wartości haszujące.
    Jednakże warto zauważyć, że taki sposób generowania hasza nie jest idealny dla wszystkich przypadków, ale w większości zastosowań, gdzie używane są proste typy danych, działa wystarczająco dobrze. W bardziej złożonych systemach można by zastosować bardziej zaawansowane funkcje haszujące.

    */
};

// Graf reprezentowany jako lista sąsiedztwa
class Graph {
public:
    std::vector<std::vector<Edge>> adjList;
    std::vector<olc::vf2d> positions;

    // Graph(int numNodes) : adjList(numNodes), positions(numNodes) {}
    Graph() {}

    void addEdge(int from, int to, int weight=1) {
        int w = calculateDistance(from, to);
        adjList[from].push_back({to, w});
        adjList[to].push_back({from, w});  // zakładając, że graf jest nieskierowany
    }

    void setNodePosition(int node, float x, float y) {
        positions[node] = {x, y};
    }

    void addNode(float x, float y) {
        positions.push_back({x, y});
        adjList.push_back(std::vector<Edge>()); // Dodajemy nową pustą listę krawędzi dla nowego węzła
    }

    // // Funkcja do obliczania odległości euklidesowej między dwoma węzłami
    // float calculateDistance(int node1, int node2) {
    //     float dx = positions[node2].x - positions[node1].x;
    //     float dy = positions[node2].y - positions[node1].y;
    //     return std::sqrt(dx * dx + dy * dy);
    // }

    // Funkcja do obliczania odległości euklidesowej między dwoma węzłami
    float calculateDistance(int node1, int node2) {
        return (positions[node2] - positions[node1]).mag();  // Użycie metody mag() z olc::vf2d
    }

    // Przesuń węzeł do nowej pozycji i zaktualizuj odległości krawędzi
    void moveNode(int node, const olc::vf2d& newPosition) {
        positions[node] = newPosition;
        updateEdges(node);
    }

    // Aktualizuj odległości krawędzi związanych z danym węzłem
    void updateEdges(int node) {
        for (auto& edge : adjList[node]) {
            edge.weight = calculateDistance(node, edge.to);
            // Musimy też zaktualizować krawędź w sąsiednim węźle
            for (auto& reverseEdge : adjList[edge.to]) {
                if (reverseEdge.to == node) {
                    reverseEdge.weight = edge.weight;
                    break;
                }
            }
        }
    }


    std::vector<int> shortestPath(int start, int end) {
        std::vector<int> dist(adjList.size(), std::numeric_limits<int>::max());
        std::vector<int> prev(adjList.size(), -1);
        dist[start] = 0;

        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            int current = pq.top().second;
            pq.pop();

            for (const auto& edge : adjList[current]) {
                int next = edge.to;
                int newDist = dist[current] + edge.weight;

                if (newDist < dist[next]) {
                    dist[next] = newDist;
                    prev[next] = current;
                    pq.push({newDist, next});
                }
            }
        }

        // Odtworzenie ścieżki
        std::vector<int> path;
        for (int at = end; at != -1; at = prev[at]) {
            path.push_back(at);
        }
        std::reverse(path.begin(), path.end());

        // Sprawdzamy, czy istnieje droga
        if (path[0] != start) path.clear();

        return path;
    }
};

// Przechowywanie i optymalizacja dróg za pomocą tablicy haszującej
class PathCache {
public:
    std::unordered_map<std::pair<int, int>, std::vector<int>, NodePairHash> cache;

    std::vector<int> getShortestPath(Graph& graph, int start, int end) {
        std::pair<int, int> key = {start, end};

        // Sprawdzenie, czy ścieżka jest już w cache
        if (cache.find(key) != cache.end()) {
            return cache[key];
        }

        // Obliczenie najkrótszej ścieżki
        std::vector<int> path = graph.shortestPath(start, end);

        // Zapisanie ścieżki do cache
        cache[key] = path;

        return path;
    }

    // Metoda do wyczyszczenia cache
    void clearCache() {
        cache.clear();
    }
};

class Example : public olc::PixelGameEngine
{
public:
    Example() {
        sAppName = "Struktury danych: Tablica haszujaca";
    }

private:
    // Graph* graph;
    Graph graph;
    PathCache cache;
    // punkty startowy i końcowy są określone przez zmienne startNode i endNode:
    int startNode = 0;
    int endNode = 4;
    std::vector<int> path;

    int newNodeIndex {0};
    int selectedNode = -1;
    int selectedStartNode = -1;
    int selectedEndNode = -1;
    bool bMenu {false};
public:

    void connectToClosestNodes(Graph& graph, int newNodeIndex, int maxConnections = 3) {
    
        std::vector<std::pair<float, int>> distances;

        for (int i = 0; i < newNodeIndex; i++) {
            float distance = (graph.positions[newNodeIndex] - graph.positions[i]).mag();
            distances.push_back({distance, i});
        }

        // Sortuj węzły według odległości od nowego węzła
        std::sort(distances.begin(), distances.end());
        // Połącz z najbliższymi węzłami
        for (int i = 0; i < std::min(maxConnections, (int)distances.size()); i++) {
            int nearestNodeIndex = distances[i].second;
            std::cout << nearestNodeIndex << std::endl;
            int weight = distances[i].first;  // Możesz ustalić wagę jako odległość, lub inny parametr
            graph.addEdge(newNodeIndex, nearestNodeIndex, weight);
        }
    }
    
	bool Menu()
	{
		// wybieramy najdluższy łańcuch
		olc::vi2d textSize = GetTextSize("CTRL + Right Mouse - new target position");
		FillRect(10, 10, textSize.x+10, 90, olc::GREEN);

		DrawString(15, 15, "Menu", olc::RED);
        DrawString(15, 30, "Left Mouse - add new node", olc::RED);
        DrawString(15, 45, "SHIFT + Left Mouse - set start node", olc::RED);
		DrawString(15, 60, "CTRL + Left Mouse - set end node", olc::RED);
		DrawString(15, 75, "SHIFT + Right Mouse - new node position", olc::RED);

		return true;
	}

    bool OnUserCreate() override {
        // Tworzymy graf
        // graph = new Graph(6);

        // Inicjalizacja grafu w konstruktorze lub w metodzie OnUserCreate
        // Ustawiamy pozycje węzłów na ekranie
        graph.addNode(100, 100);
        graph.addNode(200, 50);
        graph.addNode(200, 150);
        graph.addNode(300, 100);
        graph.addNode(400, 100);
        graph.addNode(150, 200);


        // Dodajemy krawędzie do grafu
        graph.addEdge(0, 1, 7);
        graph.addEdge(0, 2, 9);
        graph.addEdge(0, 5, 14);
        graph.addEdge(1, 2, 10);
        graph.addEdge(1, 3, 15);
        graph.addEdge(2, 3, 11);
        graph.addEdge(2, 5, 2);
        graph.addEdge(3, 4, 6);
        graph.addEdge(4, 5, 9);

        // Obliczenie najkrótszej ścieżki
        path = cache.getShortestPath(graph, startNode, endNode);

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLACK);
        bool quit =  false;
        
        if (GetKey(olc::Key::ESCAPE).bPressed) {
				quit = true;
        } 

        // Sprawdzanie, czy lewy przycisk myszy i SHIFT są wciśnięte
        if (GetKey(olc::SHIFT).bHeld && GetMouse(0).bPressed) {
            for (int i = 0; i < graph.positions.size(); ++i) {
                if ((graph.positions[i] - olc::vf2d(GetMouseX(), GetMouseY())).mag() < 10.0f) {
                    selectedStartNode = i;
                    break;
                }
            }
        } else if (GetKey(olc::CTRL).bHeld && GetMouse(0).bPressed) {
            for (int i = 0; i < graph.positions.size(); ++i) {
                if ((graph.positions[i] - olc::vf2d(GetMouseX(), GetMouseY())).mag() < 10.0f) {
                    selectedEndNode = i;
                    break;
                }
            }
        }
        // Obsługa dodawania nowego węzła po kliknięciu LMB
        else if (GetMouse(0).bPressed) {  // Jeśli lewy przycisk myszy został kliknięty
            olc::vf2d mousePos = { float(GetMouseX()), float(GetMouseY()) };
            graph.addNode(mousePos.x, mousePos.y);  // Dodaje nowy węzeł

            newNodeIndex = graph.positions.size()-1;
            connectToClosestNodes(graph, newNodeIndex);

            // Obliczenie najkrótszej ścieżki
            cache.clearCache();
            path = cache.getShortestPath(graph, startNode, endNode);
        }       

        // Rysowanie krawędzi
        for (int from = 0; from < graph.adjList.size(); from++) {
            for (const auto& edge : graph.adjList[from]) {
                int to = edge.to;
                DrawLine(graph.positions[from], graph.positions[to], olc::WHITE);
                std::string weightStr = std::to_string(edge.weight);
                olc::vf2d mid = (graph.positions[from] + graph.positions[to]) * 0.5f;
                DrawString(mid.x, mid.y, weightStr, olc::YELLOW);
            }
        }

        // Rysowanie węzłów
        for (int i = 0; i < graph.positions.size(); i++) {

            FillCircle(graph.positions[i], 10, olc::RED);

            if (i == startNode) {
                FillCircle(graph.positions[i], 10, olc::CYAN);    
                FillCircle(15, 15, 10, olc::CYAN);    
                DrawString(15-5, 15-5, std::to_string(i), olc::WHITE);
                DrawString(10+25, 15-5, "- start node", olc::WHITE);
            }
            if (i == endNode) {
                FillCircle(graph.positions[i], 10, olc::BLUE);    
                FillCircle(15, 45, 10, olc::BLUE);    
                DrawString(15-5, 45-5, std::to_string(i), olc::WHITE);
                DrawString(10+25, 45-5, "- end node", olc::WHITE);
            }

            DrawString(graph.positions[i].x - 5, graph.positions[i].y - 5, std::to_string(i), olc::WHITE);
        }

        // Sprawdzanie, czy lewy przycisk myszy i SHIFT są wciśnięte
        if (GetKey(olc::SHIFT).bHeld && GetMouse(1).bPressed) {
            for (int i = 0; i < graph.positions.size(); ++i) {
                if ((graph.positions[i] - olc::vf2d(GetMouseX(), GetMouseY())).mag() < 10.0f) {
                    selectedNode = i;
                    break;
                }
            }
        }

        // Przesuwanie węzła, jeśli jest wybrany
        if (selectedNode != -1 && GetMouse(1).bHeld) {
            graph.moveNode(selectedNode, olc::vf2d(GetMouseX(), GetMouseY()));
            // Obliczenie najkrótszej ścieżki
            cache.clearCache(); // Czyści cache, aby zapobiec korzystaniu z nieaktualnych danych po modyfikacji grafu.
            path = cache.getShortestPath(graph, startNode, endNode);  
        }

        if (selectedStartNode != -1 && GetMouse(0).bHeld) {
            DrawCircle(graph.positions[selectedStartNode].x, graph.positions[selectedStartNode].y, 10.0f, olc::CYAN);
            startNode = selectedStartNode;
            // Obliczenie najkrótszej ścieżki
            cache.clearCache();
            path = cache.getShortestPath(graph, startNode, endNode);
        }
        if (selectedEndNode != -1 && GetMouse(0).bHeld) {
            DrawCircle(graph.positions[selectedEndNode].x, graph.positions[selectedEndNode].y, 10.0f, olc::BLUE);
            endNode = selectedEndNode;
            // Obliczenie najkrótszej ścieżki
            cache.clearCache();
            path = cache.getShortestPath(graph, startNode, endNode);
        }


        if (GetMouse(0).bReleased) {
            selectedNode = -1;
            selectedStartNode = -1;
            selectedEndNode = -1;
            bMenu = false;
        }

        // Resetowanie zaznaczenia węzła po puszczeniu przycisku myszy
        if (GetMouse(1).bReleased) {
            selectedNode = -1;
            bMenu = false;
        }

        // Rysowanie najkrótszej ścieżki
        if (!path.empty()) {
            for (size_t i = 0; i < path.size() - 1; i++) {
                DrawLine(graph.positions[path[i]], graph.positions[path[i + 1]], olc::GREEN);
            }
        }

        // if (GetKey(olc::Key::SPACE).bPressed) {
		    // DrawString(8, ScreenHeight()-30, ":: " + std::to_string(path.size()), olc::GREEN);		
        // } 

		if (GetKey(olc::Key::F1).bReleased) {
			bMenu = !bMenu;
		}

        if(bMenu) {
			Menu();
		}

        DrawString(8, ScreenHeight()-15, "ESC - exit / F1 - menu", olc::GREEN);

        olc::vi2d textSize = GetTextSize("Powered by olcPixelGameEngine, 2024(8)");
        DrawString(ScreenWidth()-textSize.x, ScreenHeight()-15, "Powered by olcPixelGameEngine, 2024(8)", olc::CYAN );

		return !quit;
    }
};

int main() {
    Example app;
    if (app.Construct(512, 512, 2, 2))
        app.Start();
    return 0;
}

/*

Ten kod w C++ jest przykładem implementacji aplikacji do zarządzania grafami i ścieżkami w kontekście gier komputerowych, przy użyciu biblioteki **olcPixelGameEngine**. Oto podsumowanie, co robi i jak działa ten kod:

### Podstawowe elementy:

1. **olcPixelGameEngine**:
   - Jest to biblioteka używana do tworzenia prostych gier i symulacji 2D. Umożliwia ona łatwe rysowanie grafiki, obsługę wejść od użytkownika, i zarządzanie czasem.

2. **Graf (Graph)**:
   - Graf jest reprezentowany jako lista sąsiedztwa (`adjList`), gdzie każdy węzeł ma listę krawędzi (obiektów `Edge`) prowadzących do innych węzłów.
   - Węzły w grafie mają przypisane pozycje na płaszczyźnie 2D (`positions`), co umożliwia ich wizualne przedstawienie.

3. **Tablica haszująca (PathCache)**:
   - Tablica haszująca jest używana do przechowywania wcześniej obliczonych najkrótszych ścieżek między parami węzłów. Kluczem jest para węzłów (start, end), a wartością jest wektor węzłów reprezentujący najkrótszą ścieżkę.
   - Dzięki temu, jeśli użytkownik zapyta ponownie o ścieżkę między tymi samymi węzłami, wynik zostanie pobrany z cache, bez potrzeby ponownego obliczania.

4. **Algorytm Dijkstry (shortestPath)**:
   - Używany jest do znajdowania najkrótszej ścieżki w grafie między dwoma węzłami. Wynik jest przechowywany w tablicy haszującej, aby zoptymalizować przyszłe zapytania o tę samą ścieżkę.

5. **Interakcja użytkownika**:
   - Użytkownik może dodawać nowe węzły (punkty) na ekranie, łączyć je z istniejącymi węzłami oraz przesuwać istniejące węzły. Wszystko to jest wizualizowane na ekranie.
   - Można również ustawić węzły startowe i końcowe, a aplikacja automatycznie znajdzie i wyświetli najkrótszą ścieżkę między nimi.

6. **Wyświetlanie grafiki i ścieżek**:
   - Węzły i krawędzie są rysowane na ekranie, a także wyświetlana jest najkrótsza ścieżka jako zielona linia.
   - Użytkownik może otworzyć menu pomocy (`Menu`) z instrukcjami dotyczącymi sterowania.

### Jak to działa?

- **Tworzenie grafu**: Na początku aplikacja tworzy graf z kilkoma węzłami i krawędziami.
- **Interakcja**: Użytkownik może dodawać nowe węzły, przesuwać istniejące węzły i ustawiać węzły startowe i końcowe. Przy każdej modyfikacji grafu, najkrótsza ścieżka jest obliczana na nowo i wyświetlana.
- **Cache'owanie ścieżek**: Jeśli najkrótsza ścieżka między dwoma węzłami była już wcześniej obliczona, jest ona pobierana z cache, co oszczędza czas.
- **Rysowanie**: Aplikacja rysuje węzły, krawędzie, i ścieżki na ekranie, a także reaguje na wejścia użytkownika.

### Zastosowanie w rzeczywistości:
Tego typu implementacja może być przydatna w grach komputerowych, gdzie postacie muszą poruszać się po złożonych mapach, a optymalizacja trasy jest kluczowa. Dzięki zastosowaniu tablicy haszującej, aplikacja może szybciej reagować na zapytania o najkrótsze ścieżki, co jest istotne w dynamicznych środowiskach gier.

*/