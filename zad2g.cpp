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

// // Graf reprezentowany jako lista sąsiedztwa
// class Graph2 {
// public:
//     std::vector<std::vector<Edge>> adjList;

//     Graph(int numNodes) : adjList(numNodes) {}

//     void addEdge(int from, int to, int weight) {
//         adjList[from].push_back({to, weight});
//         adjList[to].push_back({from, weight});  // zakładając, że graf jest nieskierowany
//     }

//     std::vector<int> shortestPath(int start, int end) {
//         std::vector<int> dist(adjList.size(), std::numeric_limits<int>::max());
//         std::vector<int> prev(adjList.size(), -1);
//         dist[start] = 0;

//         std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
//         pq.push({0, start});

//         while (!pq.empty()) {
//             int current = pq.top().second;
//             pq.pop();

//             for (const auto& edge : adjList[current]) {
//                 int next = edge.to;
//                 int newDist = dist[current] + edge.weight;

//                 if (newDist < dist[next]) {
//                     dist[next] = newDist;
//                     prev[next] = current;
//                     pq.push({newDist, next});
//                 }
//             }
//         }

//         // Odtworzenie ścieżki
//         std::vector<int> path;
//         for (int at = end; at != -1; at = prev[at]) {
//             path.push_back(at);
//         }
//         std::reverse(path.begin(), path.end());

//         // Sprawdzamy, czy istnieje droga
//         if (path[0] != start) path.clear();

//         return path;
//     }
// };



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
		DrawString(15, 60, "SHIFT + Right Mouse - set end node", olc::RED);
		DrawString(15, 75, "CTRL + Right Mouse - new target position", olc::RED);

		return true;
	}

    bool OnUserCreate() override {
        // Tworzymy graf
        // graph = new Graph(6);

        // Inicjalizacja grafu w konstruktorze lub w metodzie OnUserCreate
        graph.addNode(100, 100);
        graph.addNode(200, 50);
        graph.addNode(200, 150);
        graph.addNode(300, 100);
        graph.addNode(400, 100);
        graph.addNode(150, 200);

        // Dodajemy krawędzie do grafu
        // graph->addEdge(0, 1, 7);
        // graph->addEdge(0, 2, 9);
        // graph->addEdge(0, 5, 14);
        // graph->addEdge(1, 2, 10);
        // graph->addEdge(1, 3, 15);
        // graph->addEdge(2, 3, 11);
        // graph->addEdge(2, 5, 2);
        // graph->addEdge(3, 4, 6);
        // graph->addEdge(4, 5, 9);

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


        // // Ustawiamy pozycje węzłów na ekranie
        // graph->setNodePosition(0, 100, 100);
        // graph->setNodePosition(1, 200, 50);
        // graph->setNodePosition(2, 200, 150);
        // graph->setNodePosition(3, 300, 100);
        // graph->setNodePosition(4, 400, 100);
        // graph->setNodePosition(5, 150, 200);

        // Obliczenie najkrótszej ścieżki
        // path = cache.getShortestPath(*graph, startNode, endNode);
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
            //newNodeIndex = graph->positions.size();
            // graph.positions.push_back(mousePos);
            graph.addNode(mousePos.x, mousePos.y);  // Dodaje nowy węzeł
            //graph->setNodePosition(newNodeIndex, mousePos.x, mousePos.y);
            // graph->adjList.push_back(std::vector<Edge>()); // Dodaj nowy węzeł do listy sąsiedztwa

            // Dodaj krawędzie do istniejących węzłów (opcjonalne)
            // Można użyć heurystyki do automatycznego tworzenia krawędzi lub dodać UI do ręcznego tworzenia krawędzi

            newNodeIndex = graph.positions.size()-1;
            // connectToClosestNodes(*graph, newNodeIndex, 1);
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
            // olc::vf2d(GetMouseX(), GetMouseY())
            // graph.moveNode(selectedNode, );
            DrawCircle(graph.positions[selectedStartNode].x, graph.positions[selectedStartNode].y, 10.0f, olc::CYAN);
            startNode = selectedStartNode;
            // Obliczenie najkrótszej ścieżki
            cache.clearCache();
            path = cache.getShortestPath(graph, startNode, endNode);
        }
        if (selectedEndNode != -1 && GetMouse(0).bHeld) {
            // olc::vf2d(GetMouseX(), GetMouseY())
            // graph.moveNode(selectedNode, );
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
            int c = {0};
            for (size_t i = 0; i < path.size() - 1; i++) {
                DrawLine(graph.positions[path[i]], graph.positions[path[i + 1]], olc::GREEN);
                // c = i;
                // DrawString(15, 15+i*15, std::to_string(c), olc::GREEN);
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

// g++ -o start tablica_haszująca.cpp  -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++20



/* 
Połączenie nowo dodanego węzła z istniejącymi węzłami w grafie może być wyzwaniem, szczególnie jeśli chcesz uniknąć "zamieszania" i nadmiernego zagęszczenia krawędzi na ekranie. Oto kilka strategii, które można zastosować, aby w sposób kontrolowany i przejrzysty połączyć nowy węzeł z istniejącymi:

### 1. **Łączenie z najbliższymi węzłami:**
   - **Opis:** Łącz nowy węzeł tylko z jednym lub kilkoma najbliższymi węzłami. Możesz użyć prostej odległości euklidesowej do znalezienia najbliższych sąsiadów.
   - **Zaleta:** Unikasz tworzenia nadmiarowych połączeń i przecięcia wielu krawędzi, co pozwala utrzymać przejrzystość.

   **Implementacja:**
   ```cpp
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
           int weight = distances[i].first;  // Możesz ustalić wagę jako odległość, lub inny parametr
           graph.addEdge(newNodeIndex, nearestNodeIndex, weight);
       }
   }
   ```

   **Użycie po dodaniu nowego węzła:**
   ```cpp
   int newNodeIndex = graph->positions.size() - 1;
   connectToClosestNodes(*graph, newNodeIndex);
   ```

### 2. **Łączenie tylko z węzłami w obrębie określonego promienia:**
   - **Opis:** Połącz nowy węzeł tylko z węzłami, które znajdują się w określonym promieniu od niego. Dzięki temu unikniesz tworzenia zbyt długich i niepotrzebnych krawędzi.
   - **Zaleta:** Zapobiegasz tworzeniu krawędzi, które przebiegają przez cały obszar rysowania, co może zaburzać czytelność grafu.

   **Implementacja:**
   ```cpp
   void connectToNodesWithinRadius(Graph& graph, int newNodeIndex, float radius) {
       for (int i = 0; i < newNodeIndex; i++) {
           float distance = (graph.positions[newNodeIndex] - graph.positions[i]).mag();
           if (distance <= radius) {
               int weight = distance;  // Możesz ustalić wagę jako odległość, lub inny parametr
               graph.addEdge(newNodeIndex, i, weight);
           }
       }
   }
   ```

   **Użycie po dodaniu nowego węzła:**
   ```cpp
   int newNodeIndex = graph->positions.size() - 1;
   float connectionRadius = 100.0f;  // Przykładowy promień
   connectToNodesWithinRadius(*graph, newNodeIndex, connectionRadius);
   ```

### 3. **Ręczne łączenie węzłów:**
   - **Opis:** Umożliw użytkownikowi ręczne wskazanie, z którymi węzłami ma być połączony nowy węzeł. Można to zrobić poprzez kliknięcie na istniejący węzeł po dodaniu nowego.
   - **Zaleta:** Daje pełną kontrolę nad tym, jak graf jest tworzony, co może być przydatne w bardziej skomplikowanych lub specjalistycznych aplikacjach.

   **Implementacja:**
   - Dodaj funkcjonalność pozwalającą użytkownikowi kliknąć na węzeł, aby wybrać, z którym ma zostać połączony nowy węzeł. Po wybraniu dwóch węzłów automatycznie dodaj krawędź między nimi.

### 4. **Połączenie węzłów na podstawie minimalnej liczby krawędzi:**
   - **Opis:** Połącz nowy węzeł z tymi istniejącymi, które mają najmniej połączeń. To pozwala na równomierne rozłożenie połączeń między węzłami.
   - **Zaleta:** Utrzymuje równowagę w grafie, co może być przydatne, gdy chcesz uniknąć sytuacji, gdzie jeden węzeł ma zbyt wiele połączeń, a inne zbyt mało.

   **Implementacja:**
   ```cpp
   void connectToNodesWithFewestEdges(Graph& graph, int newNodeIndex, int maxConnections = 3) {
       std::vector<std::pair<int, int>> edgeCounts;

       for (int i = 0; i < newNodeIndex; i++) {
           edgeCounts.push_back({graph.adjList[i].size(), i});
       }

       // Sortuj węzły według liczby połączeń (rosnąco)
       std::sort(edgeCounts.begin(), edgeCounts.end());

       // Połącz z węzłami mającymi najmniej połączeń
       for (int i = 0; i < std::min(maxConnections, (int)edgeCounts.size()); i++) {
           int nodeIndex = edgeCounts[i].second;
           float distance = (graph.positions[newNodeIndex] - graph.positions[nodeIndex]).mag();
           graph.addEdge(newNodeIndex, nodeIndex, distance);
       }
   }
   ```

   **Użycie po dodaniu nowego węzła:**
   ```cpp
   int newNodeIndex = graph->positions.size() - 1;
   connectToNodesWithFewestEdges(*graph, newNodeIndex);
   ```

### Wnioski:

Każda z tych metod ma swoje zalety i zastosowanie zależy od konkretnego przypadku użycia:

- **Łączenie z najbliższymi węzłami** i **łączenie w obrębie promienia** są najbardziej automatyczne i łatwe do zaimplementowania, ale mogą prowadzić do tworzenia wielu krótkich krawędzi.
- **Ręczne łączenie** daje największą kontrolę użytkownikowi, ale wymaga dodatkowego interfejsu i interakcji użytkownika.
- **Łączenie na podstawie minimalnej liczby krawędzi** pozwala równoważyć połączenia w grafie.

Wybór metody zależy od tego, jak dynamiczny ma być graf i jak dużą kontrolę chcesz dać użytkownikowi nad jego kształtowaniem. Każda z tych metod może być także łączona z innymi, aby uzyskać optymalne rezultaty.

!! 21-08-2024 !!

Dodanie funkcji wprowadzającej nowy węzeł między dwoma istniejącymi węzłami w grafie z możliwością ustawienia wagi nowej krawędzi wymaga kilku kroków. W skrócie, gdy użytkownik zdecyduje się dodać nowy węzeł między dwoma już istniejącymi, musimy:

1. **Wprowadzić nowy węzeł do grafu.**
2. **Podzielić istniejącą krawędź na dwie nowe krawędzie,** które połączą istniejące węzły z nowym węzłem.
3. **Umożliwić ustawienie wagi dla nowych krawędzi** w zakresie od 0 do wartości oryginalnej krawędzi.

### Przykład implementacji

Poniżej przedstawiam zaktualizowany kod w C++, który implementuje tę funkcjonalność:

```cpp
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include "olcPixelGameEngine.h"

// Struktura reprezentująca krawędź w grafie
struct Edge {
    int to;
    float weight;
};

// Klasa reprezentująca graf jako listę sąsiedztwa
class Graph {
public:
    std::vector<std::vector<Edge>> adjList;
    std::vector<olc::vf2d> positions;

    Graph(int numNodes) : adjList(numNodes), positions(numNodes) {}

    // Dodaj węzeł z jego pozycją na mapie
    void addNode(int node, float x, float y) {
        if (node >= positions.size()) {
            positions.resize(node + 1);
            adjList.resize(node + 1);
        }
        positions[node] = {x, y};
    }

    // Dodaj krawędź między węzłami, obliczając jej wagę na podstawie odległości euklidesowej
    void addEdge(int from, int to) {
        float weight = calculateDistance(from, to);
        adjList[from].push_back({to, weight});
        adjList[to].push_back({from, weight});  // Zakładamy, że graf jest nieskierowany
    }

    // Dodaj krawędź między węzłami z określoną wagą
    void addEdge(int from, int to, float weight) {
        adjList[from].push_back({to, weight});
        adjList[to].push_back({from, weight});
    }

    // Funkcja do obliczania odległości euklidesowej między dwoma węzłami
    float calculateDistance(int node1, int node2) {
        return (positions[node2] - positions[node1]).mag();
    }

    // Przesuń węzeł do nowej pozycji i zaktualizuj odległości krawędzi
    void moveNode(int node, const olc::vf2d& newPosition) {
        positions[node] = newPosition;
        updateEdges(node);
    }

    // Dodaj nowy węzeł pomiędzy dwa istniejące z możliwością ustawienia wagi nowej krawędzi
    void addNodeBetween(int from, int to, int newNode, float weightFromToNew, float weightNewToTo) {
        // Dodaj nowy węzeł w połowie drogi między istniejącymi
        olc::vf2d newPosition = (positions[from] + positions[to]) * 0.5f;
        addNode(newNode, newPosition.x, newPosition.y);

        // Usuń starą krawędź między from a to
        removeEdge(from, to);

        // Dodaj nowe krawędzie
        addEdge(from, newNode, weightFromToNew);
        addEdge(newNode, to, weightNewToTo);
    }

    // Usuń krawędź między dwoma węzłami
    void removeEdge(int from, int to) {
        adjList[from].erase(std::remove_if(adjList[from].begin(), adjList[from].end(),
            [to](const Edge& edge) { return edge.to == to; }), adjList[from].end());

        adjList[to].erase(std::remove_if(adjList[to].begin(), adjList[to].end(),
            [from](const Edge& edge) { return edge.to == from; }), adjList[to].end());
    }

    // Aktualizuj odległości krawędzi związanych z danym węzłem
    void updateEdges(int node) {
        for (auto& edge : adjList[node]) {
            edge.weight = calculateDistance(node, edge.to);
            for (auto& reverseEdge : adjList[edge.to]) {
                if (reverseEdge.to == node) {
                    reverseEdge.weight = edge.weight;
                    break;
                }
            }
        }
    }

    // Funkcja do wypisania grafu
    void printGraph() {
        for (int i = 0; i < adjList.size(); ++i) {
            std::cout << "Node " << i << " at position (" 
                      << positions[i].x << ", " 
                      << positions[i].y << "):\n";
            for (const Edge& edge : adjList[i]) {
                std::cout << "  -> " << edge.to << " (weight: " << edge.weight << ")\n";
            }
        }
    }
};

int main() {
    Graph graph(3);
    graph.addNode(0, 0.0f, 0.0f);
    graph.addNode(1, 10.0f, 0.0f);
    graph.addNode(2, 20.0f, 0.0f);

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);

    // Przykład dodania nowego węzła między węzłami 0 i 1
    int newNode = 3;
    float maxWeight = graph.calculateDistance(0, 1);
    graph.addNodeBetween(0, 1, newNode, maxWeight * 0.5f, maxWeight * 0.5f);

    graph.printGraph();

    return 0;
}
```

### Wyjaśnienie kodu:

1. **Dodawanie nowego węzła między istniejącymi węzłami:**
   - Metoda `addNodeBetween(int from, int to, int newNode, float weightFromToNew, float weightNewToTo)` pozwala na dodanie nowego węzła pomiędzy dwoma istniejącymi węzłami.
   - Nowy węzeł jest umieszczany w połowie odległości między dwoma wybranymi węzłami.
   - Stara krawędź jest usuwana, a następnie tworzone są dwie nowe krawędzie z określonymi wagami, podzielonymi według podanych wartości.

2. **Usuwanie krawędzi:**
   - Metoda `removeEdge(int from, int to)` usuwa krawędź między dwoma węzłami.

3. **Waga krawędzi:**
   - Wagi nowych krawędzi mogą być ustawione w przedziale od 0 do wartości oryginalnej krawędzi, podzielone proporcjonalnie na dwie nowe krawędzie. W przykładzie zastosowano podział na pół.

4. **Przykład użycia:**
   - W przykładzie, po dodaniu nowego węzła pomiędzy węzły `0` i `1`, węzeł `3` zostaje dodany w połowie drogi, a krawędź `0-1` jest zastępowana krawędziami `0-3` i `3-1`.

### Korzyści:
Takie podejście pozwala na dynamiczne modyfikowanie grafu w sposób bardziej złożony, co może być przydatne w różnych zastosowaniach, takich jak rozbudowa sieci dróg, dynamiczne aktualizacje tras w czasie rzeczywistym w grach, lub modelowanie zmian w strukturze połączeń między różnymi punktami w aplikacjach symulacyjnych.


*/
