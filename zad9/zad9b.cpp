
#define OLC_PGE_APPLICATION
#include "../olcPixelGameEngine.h"

#include <iostream>
#include <limits>
#include <queue>
#include <vector>
#include <tuple>
#include <cstring>

/* 

problem dotyczący eksportu kawy z Meksyku można modelować jako problem przepływu w sieci, gdzie poszczególne porty i magazyny są węzłami, a przepływy między nimi to krawędzie z określoną pojemnością. Aby stworzyć graf, rozważmy następujące kroki:

1. Zdefiniowanie węzłów:
Magazyny: Veracruz, Tampico, Tuxpan, Campeche.
Porty docelowe (importerzy): Bordeaux, Saint-Nazaire, Hawar, Dunkierka.
2. Zdefiniowanie krawędzi:
Krawędzie między magazynami a portami docelowymi reprezentują możliwości transportu kawy, z odpowiednią pojemnością (maksymalną ilością kawy, którą można przetransportować).
3. Wprowadzenie źródła i ujścia:
Źródło (source): Jeden węzeł reprezentujący całą dostępność kawy.
Ujście (sink): Jeden węzeł reprezentujący całość popytu na kawę w portach docelowych.
4. Krawędzie:
Od źródła do magazynów z odpowiednimi pojemnościami (ilość kawy w magazynach).
Od magazynów do portów docelowych z pojemnościami odpowiadającymi zamówieniom lub maksymalnym możliwościom transportu.
5. Priorytet Campeche:
Aby zapewnić pełne wykorzystanie zasobów z magazynu w Campeche, możemy dodać warunek, że krawędź od Campeche do portów docelowych ma najwyższy priorytet lub zwiększoną pojemność, by najpierw wykorzystać zasoby z tego magazynu.

Reprezentacja w formie grafu:
Węzły:

𝑆
S (źródło)
𝑉
V (Veracruz)
𝑇
T (Tampico)
𝑋
X (Tuxpan)
𝐶
C (Campeche)
𝐵
B (Bordeaux)
𝑁
N (Saint-Nazaire)
𝐻
H (Hawar)
𝐷
D (Dunkierka)
𝑇
T (ujście)
Krawędzie:

𝑆
→
𝑉
S→V, 
𝑆
→
𝑇
S→T, 
𝑆
→
𝑋
S→X, 
𝑆
→
𝐶
S→C (dostępność kawy w magazynach)
𝑉
→
𝐵
V→B, 
𝑉
→
𝑁
V→N, 
𝑉
→
𝐻
V→H, 
𝑉
→
𝐷
V→D (pojemności transportowe)
𝑇
→
𝐵
T→B, 
𝑇
→
𝑁
T→N, 
𝑇
→
𝐻
T→H, 
𝑇
→
𝐷
T→D
𝑋
→
𝐵
X→B, 
𝑋
→
𝑁
X→N, 
𝑋
→
𝐻
X→H, 
𝑋
→
𝐷
X→D
𝐶
→
𝐵
C→B, 
𝐶
→
𝑁
C→N, 
𝐶
→
𝐻
C→H, 
𝐶
→
𝐷
C→D
𝐵
→
𝑇
B→T, 
𝑁
→
𝑇
N→T, 
𝐻
→
𝑇
H→T, 
𝐷
→
𝑇
D→T (popyt w portach)

*/


class CoffeeFlowVisualization : public olc::PixelGameEngine {
public:
    CoffeeFlowVisualization() {
        sAppName = "Coffee Export Flow with Ford-Fulkerson";
    }

private:
    struct Node {
        int x, y;
        std::string label;
    };

    std::vector<Node> nodes;
    std::vector<std::tuple<int, int, int>> edges; // From, To, Capacity
    int nNodes, source, sink;
    std::vector<std::vector<int>> capacity, residualCapacity;
    std::vector<std::vector<int>> adjList;

public:
    bool OnUserCreate() override {

        DrawString(ScreenWidth() / 2 - 40, ScreenHeight() / 2, "Loading...", olc::WHITE);

        nNodes = 10;
        source = 0;
        sink = 9;

        capacity.resize(nNodes, std::vector<int>(nNodes, 0));
        residualCapacity.resize(nNodes, std::vector<int>(nNodes, 0));
        adjList.resize(nNodes);

        // Definiowanie węzłów
        nodes.push_back({50, 50, "Source"});
        nodes.push_back({150, 150, "Veracruz"});
        nodes.push_back({150, 250, "Tampico"});
        nodes.push_back({150, 350, "Tuxpan"});
        nodes.push_back({150, 450, "Campeche"});
        nodes.push_back({450, 150, "Bordeaux"});
        nodes.push_back({450, 250, "Saint-Nazaire"});
        nodes.push_back({450, 350, "Hawar"});
        nodes.push_back({450, 450, "Dunkirk"});
        nodes.push_back({550, 250, "Sink"});

        // Definiowanie krawędzi i pojemności
        AddEdge(0, 1, 100); // Source -> Veracruz
        AddEdge(0, 2, 150); // Source -> Tampico
        AddEdge(0, 3, 100); // Source -> Tuxpan
        AddEdge(0, 4, 200); // Source -> Campeche

        AddEdge(1, 5, 50);  // Veracruz -> Bordeaux
        AddEdge(1, 6, 50);  // Veracruz -> Saint-Nazaire
        AddEdge(1, 7, 50);  // Veracruz -> Hawar
        AddEdge(1, 8, 50);  // Veracruz -> Dunkirk

        AddEdge(2, 5, 50);  // Tampico -> Bordeaux
        AddEdge(2, 6, 50);  // Tampico -> Saint-Nazaire
        AddEdge(2, 7, 50);  // Tampico -> Hawar
        AddEdge(2, 8, 50);  // Tampico -> Dunkirk

        AddEdge(3, 5, 50);  // Tuxpan -> Bordeaux
        AddEdge(3, 6, 50);  // Tuxpan -> Saint-Nazaire
        AddEdge(3, 7, 50);  // Tuxpan -> Hawar
        AddEdge(3, 8, 50);  // Tuxpan -> Dunkirk

        AddEdge(4, 5, 100); // Campeche -> Bordeaux
        AddEdge(4, 6, 100); // Campeche -> Saint-Nazaire
        AddEdge(4, 7, 100); // Campeche -> Hawar
        AddEdge(4, 8, 100); // Campeche -> Dunkirk

        AddEdge(5, 9, 150); // Bordeaux -> Sink
        AddEdge(6, 9, 150); // Saint-Nazaire -> Sink
        AddEdge(7, 9, 150); // Hawar -> Sink
        AddEdge(8, 9, 150); // Dunkirk -> Sink

        return true;
    }

    void AddEdge(int u, int v, int cap) {
        capacity[u][v] = cap;
        residualCapacity[u][v] = cap;
        adjList[u].push_back(v);
        adjList[v].push_back(u);
        edges.push_back({u, v, cap});
    }

    void DrawGraph() {
        // Rysowanie krawędzi
        for (auto &edge : edges) {
            int from = std::get<0>(edge);
            int to = std::get<1>(edge);
            int capacity = std::get<2>(edge);

            DrawLine(nodes[from].x, nodes[from].y, nodes[to].x, nodes[to].y, olc::WHITE);
            DrawString((nodes[from].x + nodes[to].x) / 2, (nodes[from].y + nodes[to].y) / 2, std::to_string(capacity), olc::RED);
        }

        // Rysowanie węzłów
        for (auto &node : nodes) {
            FillCircle(node.x, node.y, 10, olc::BLUE);
            DrawString(node.x - 15, node.y - 15, node.label, olc::YELLOW);
        }
    }


void HighlightNode(int node, olc::Pixel color) {
    FillCircle(nodes[node].x, nodes[node].y, 10, color);
}

void HighlightEdge(int from, int to, olc::Pixel color) {
    DrawLine(nodes[from].x, nodes[from].y, nodes[to].x, nodes[to].y, color);
}

    bool BFS(std::vector<int>& parent) {
        std::fill(parent.begin(), parent.end(), -1);
        parent[source] = source;
        std::queue<std::pair<int, int>> q;
        q.push({source, std::numeric_limits<int>::max()});

        while (!q.empty()) {
            int cur = q.front().first;
            int flow = q.front().second;
            q.pop();

            for (int next : adjList[cur]) {
                if (parent[next] == -1 && residualCapacity[cur][next] > 0) {
                    parent[next] = cur;

                    int new_flow = std::min(flow, residualCapacity[cur][next]);
                    if (next == sink) {
                        return true;
                    }
                    q.push({next, new_flow});

                    // Clear(olc::BLACK);
                    // Animowanie procesu przeszukiwania
                    //DrawGraph();
                    HighlightEdge(cur, next, olc::RED);
                    HighlightNode(next, olc::YELLOW);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));

                }
            }
        }

        return false;
    }

void DrawFlow() {
    for (int i = 0; i < nNodes; i++) {
        for (int j = 0; j < nNodes; j++) {
            if (capacity[i][j] > 0) {
                int flow = capacity[i][j] - residualCapacity[i][j];
                if (flow > 0) {
                    DrawLine(nodes[i].x, nodes[i].y, nodes[j].x, nodes[j].y, olc::GREEN); // Zmiana koloru na zielony
                    DrawString((nodes[i].x + nodes[j].x) / 2, (nodes[i].y + nodes[j].y) / 2 + 10, std::to_string(flow) + "/" + std::to_string(capacity[i][j]), olc::GREEN);
                }
            }
        }
    }
}

    int FordFulkerson() {
        int max_flow = 0;
        std::vector<int> parent(nNodes);

        while (BFS(parent)) {
            int flow = std::numeric_limits<int>::max();
            int cur = sink;

            while (cur != source) {
                int prev = parent[cur];
                flow = std::min(flow, residualCapacity[prev][cur]);
                cur = prev;
            }

            max_flow += flow;
            cur = sink;

            while (cur != source) {
                int prev = parent[cur];
                residualCapacity[prev][cur] -= flow;
                residualCapacity[cur][prev] += flow;
                cur = prev;
            }
            // Rysowanie zmieniającego się przepływu
            // Clear(olc::BLACK);
            //DrawGraph();
            // DrawFlow();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));


        }

        return max_flow;
    }

    bool OnUserUpdate(float fElapsedTime) override {

        //Clear(olc::BLACK);

        // // Rysowanie krawędzi
        // for (auto &edge : edges) {
        //     int from = std::get<0>(edge);
        //     int to = std::get<1>(edge);
        //     int capacity = std::get<2>(edge);

        //     DrawLine(nodes[from].x, nodes[from].y, nodes[to].x, nodes[to].y, olc::WHITE);
        //     DrawString((nodes[from].x + nodes[to].x) / 2, (nodes[from].y + nodes[to].y) / 2, std::to_string(capacity), olc::RED);
        // }

        // Rysowanie węzłów
        // for (auto &node : nodes) {
        //     FillCircle(node.x, node.y, 10, olc::BLUE);
        //     DrawString(node.x - 15, node.y - 15, node.label, olc::YELLOW);
        // }

        // Rysowanie grafu
        // DrawGraph();

        // Rozwiązanie problemu za pomocą Forda-Fulkersona
        int maxFlow = FordFulkerson();
        // DrawString(10, 10, "Max Flow: " + std::to_string(maxFlow), olc::GREEN);

        return true;
    }
};

int main() {
    CoffeeFlowVisualization demo;
    if (demo.Construct(600, 600, 2, 2))
        demo.Start();

    return 0;
}

/*

Wyjaśnienie:
Struktura Grafu: Węzły i krawędzie reprezentują magazyny, porty oraz możliwe połączenia między nimi.
Ford-Fulkerson: Wykorzystuje BFS do znajdowania ścieżek powiększających w sieci residualnej.

Wizualizacja: Rysujemy węzły i krawędzie oraz wyświetlamy maksymalny przepływ na ekranie.
Dalsze Rozszerzenia:
Animacja: Można animować proces znajdowania ścieżek powiększających w sieci residualnej.
Interaktywność: Dodanie interaktywnego dodawania lub usuwania krawędzi oraz zmienianie ich pojemności.
Wizualizacja Przepływu: Wyświetlanie rzeczywistego przepływu na krawędziach w miarę postępu algorytmu.


Rozwijając temat animacji procesu znajdowania ścieżek powiększających w sieci residualnej oraz wizualizacji przepływu, można stworzyć bardziej dynamiczną i edukacyjną prezentację działania algorytmu Forda-Fulkersona z wykorzystaniem olcPixelGameEngine. Poniżej opisuję, jak można zaimplementować te funkcje w bardziej szczegółowy sposób:

1. Animacja procesu znajdowania ścieżek powiększających:
W procesie algorytmu Forda-Fulkersona BFS (Breadth-First Search) jest używany do znajdowania ścieżek powiększających w sieci residualnej. Animacja może pokazać krok po kroku, jak BFS przeszukuje sieć, zaznaczając aktualnie analizowane krawędzie i węzły.

Implementacja:
Podświetlanie węzłów: Możemy podświetlać węzły, które są aktualnie odwiedzane przez BFS.
Podświetlanie krawędzi: Krawędzie, które są analizowane przez BFS, mogą być rysowane w innym kolorze, aby pokazać, jak algorytm przeszukuje graf.
Opóźnienia: Wprowadzenie opóźnień między kolejnymi krokami BFS pozwoli na obserwowanie każdego kroku algorytmu.
2. Wizualizacja rzeczywistego przepływu:
Po zakończeniu każdego cyklu algorytmu (czyli po znalezieniu ścieżki powiększającej), można wizualizować aktualny stan sieci - pokazując, ile jednostek przepływu przepłynęło przez każdą krawędź.

Implementacja:
Rysowanie przepływu na krawędziach: Każda krawędź może być narysowana z etykietą wskazującą aktualny przepływ oraz maksymalną pojemność (np. "5/10" oznacza, że przez krawędź przepłynęło 5 jednostek z maksymalnie 10 możliwych).
Zmiana koloru krawędzi: Krawędzie z większym przepływem mogą być narysowane grubszą linią lub w innym kolorze, aby łatwo zobaczyć, którędy przepływa najwięcej jednostek.
Sieć residualna: Można też narysować sieć residualną, która pokazuje pozostałą pojemność na każdej krawędzi oraz przepływy wsteczne.


iteracja 3

Zmiana koloru w zależności od przepływu:
Możemy zmienić grubość lub kolor linii w zależności od wielkości przepływu, co pozwoli łatwo zauważyć, które krawędzie są intensywnie używane.

3. Sieć residualna:
Możemy również wizualizować sieć residualną, aby pokazać, jakie przepływy są możliwe w kolejnych iteracjach algorytmu. Sieć residualna może być rysowana w innym kolorze (np. niebieskim), a etykiety mogą wskazywać pozostałą pojemność.

Końcowy efekt:
Dzięki tym rozszerzeniom, użytkownik będzie mógł na bieżąco obserwować, jak BFS przeszukuje graf, jak przepływ jest aktualizowany na krawędziach oraz jak działa sieć residualna. Takie wizualne podejście znacznie ułatwia zrozumienie działania algorytmu Forda-Fulkersona.


*/



// #include <vector>
// #include <queue>
// #include <limits.h>
// #include <algorithm>

// using namespace std;

// #define V 6  // Liczba węzłów w grafie

// class FlowVisualization : public olc::PixelGameEngine {
// public:
//     FlowVisualization() {
//         sAppName = "Ford-Fulkerson Visualization";
//     }

// private:
//     int graph[V][V];
//     int residualGraph[V][V];
//     int maxFlow = 0;
//     vector<pair<int, int>> edges;
//     vector<int> nodes;

//     int source = 0;
//     int sink = 5;
//     int parent[V];

// public:
//     bool OnUserCreate() override {
//         // Inicjalizacja grafu
//         int tempGraph[V][V] = {
//             {0, 16, 13, 0, 0, 0},
//             {0, 0, 10, 12, 0, 0},
//             {0, 4, 0, 0, 14, 0},
//             {0, 0, 9, 0, 0, 20},
//             {0, 0, 0, 7, 0, 4},
//             {0, 0, 0, 0, 0, 0}
//         };
//         memcpy(graph, tempGraph, sizeof(tempGraph));
//         memcpy(residualGraph, tempGraph, sizeof(tempGraph));

//         // Pozycje węzłów
//         nodes = {50, 150, 250, 350, 450, 550};

//         // Krawędzie grafu
//         edges = {{0, 1}, {0, 2}, {1, 2}, {1, 3}, {2, 4}, {3, 2}, {3, 5}, {4, 3}, {4, 5}};

//         return true;
//     }

//     bool bfs(int parent[]) {
//         bool visited[V];
//         fill(visited, visited + V, false);

//         queue<int> q;
//         q.push(source);
//         visited[source] = true;
//         parent[source] = -1;

//         while (!q.empty()) {
//             int u = q.front();
//             q.pop();

//             for (int v = 0; v < V; v++) {
//                 if (!visited[v] && residualGraph[u][v] > 0) {
//                     if (v == sink) {
//                         parent[v] = u;
//                         return true;
//                     }
//                     q.push(v);
//                     parent[v] = u;
//                     visited[v] = true;
//                 }
//             }
//         }

//         return false;
//     }

//     void FordFulkerson() {
//         while (bfs(parent)) {
//             int pathFlow = INT_MAX;

//             for (int v = sink; v != source; v = parent[v]) {
//                 int u = parent[v];
//                 pathFlow = min(pathFlow, residualGraph[u][v]);
//             }

//             for (int v = sink; v != source; v = parent[v]) {
//                 int u = parent[v];
//                 residualGraph[u][v] -= pathFlow;
//                 residualGraph[v][u] += pathFlow;
//             }

//             maxFlow += pathFlow;
//         }
//     }

//     void DrawGraph() {
//         Clear(olc::BLACK);

//         // Rysowanie krawędzi
//         for (auto &edge : edges) {
//             int u = edge.first;
//             int v = edge.second;
//             DrawLine(nodes[u], 100, nodes[v], 100, olc::WHITE);
//             // Rysowanie przepływu
//             DrawString((nodes[u] + nodes[v]) / 2, 90, to_string(residualGraph[v][u]) + "/" + to_string(graph[u][v]), olc::RED);
//         }

//         // Rysowanie węzłów
//         for (int i = 0; i < V; i++) {
//             FillCircle(nodes[i], 100, 10, olc::BLUE);
//             DrawString(nodes[i] - 5, 95, to_string(i), olc::YELLOW);
//         }
//     }

//     bool OnUserUpdate(float fElapsedTime) override {
//         DrawGraph();
        
//         if (GetKey(olc::SPACE).bPressed) {
//             FordFulkerson();
//         }

//         return true;
//     }
// };

// int main() {
//     FlowVisualization app;
//     if (app.Construct(600, 200, 2, 2))
//         app.Start();

//     return 0;
// }



/*

Pomysł na wizualizację:
Wizualizacja Grafu:

Węzły mogą być przedstawione jako punkty na ekranie.
Krawędzie między węzłami mogą być rysowane jako linie, z grubością linii reprezentującą pojemność przepływu.
Pojemność aktualnego przepływu i pojemność krawędzi mogą być wyświetlane obok krawędzi.
Przepływ:

Animacja pokazująca przepływ przez krawędzie, np. przez zmieniające się kolory krawędzi lub animowane strzałki.
Podczas każdej iteracji można pokazać aktualny przepływ na danej krawędzi oraz jego zmiany w czasie rzeczywistym.
Proces BFS:

Wizualizacja procesu BFS, jak węzły są odwiedzane w poszukiwaniu ścieżki powiększającej.
Kolorowanie węzłów i krawędzi, które są częścią bieżącej ścieżki powiększającej.
Interaktywność:

Możliwość interaktywnego dodawania lub usuwania węzłów i krawędzi.
Dynamiczne zmienianie pojemności przepływów przez użytkownika.
Uruchomienie algorytmu na żądanie i zatrzymanie lub pauza w celu prześledzenia szczegółów działania.

Wyjaśnienie:
Klasa FlowVisualization dziedziczy po olc::PixelGameEngine i obsługuje wizualizację grafu.
Węzły i krawędzie są rysowane na ekranie, a przepływy są oznaczone na krawędziach.
BFS i Ford-Fulkerson są zaimplementowane jako część klasy, z możliwością uruchomienia algorytmu po naciśnięciu spacji.
Funkcja DrawGraph() rysuje aktualny stan grafu, w tym przepływy i pojemności na krawędziach.


Algorytm Forda-Fulkersona jest klasycznym algorytmem w teorii grafów, używanym do znajdowania maksymalnego przepływu w sieciach przepływowych. Algorytm ten opiera się na idei przepływu w sieci residualnej, która reprezentuje możliwości przepływu w grafie po uwzględnieniu aktualnie istniejących przepływów.

Podstawowe pojęcia:
Sieć przepływowa:

- Węzły (Nodes): Elementy sieci, które mogą być źródłem, ujściem lub pośrednimi punktami przepływu.
- Krawędzie (Edges): Połączenia między węzłami, które mają określoną pojemność, czyli maksymalny możliwy przepływ przez tę krawędź.
- Przepływ (Flow): Ilość zasobu (np. woda, dane, energia) przepływającego przez krawędź.
- Źródło (Source): Węzeł początkowy, z którego przepływ jest inicjowany.
- Ujście (Sink): Węzeł końcowy, do którego przepływ zmierza.

Sieć residualna (Residual Network):

Sieć residualna reprezentuje możliwości dalszego przepływu w sieci przepływowej. Dla każdej krawędzi w sieci istnieje krawędź residualna, która ma pojemność równą różnicy między pojemnością krawędzi a aktualnym przepływem.

*/