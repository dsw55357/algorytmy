#define OLC_PGE_APPLICATION
#include "../olcPixelGameEngine.h"

#include <vector>
#include <queue>
#include <chrono>
#include <thread>


struct Node
{
    int x, y; // Pozycja węzła
    std::vector<Node*> neighbors; // Lista sąsiadów
    bool visited = false;
    bool inQueue = false; // Czy węzeł jest w kolejce?
};

class BFS_Visualization : public olc::PixelGameEngine
{
public:
    BFS_Visualization()
    {
        sAppName = "BFS Visualization";
    }

private:
    std::vector<Node> nodes;
    std::queue<Node*> nodeQueue;
    Node* startNode = nullptr;

    // Parametry animacji
    int delayMilliseconds = 250;
public:
    bool OnUserCreate() override
    {
        // Ustawienia dla siatki węzłów
        // liczba węzłów : 20
        int gridWidth = 5;
        int gridHeight = 4;
        // Węzły są rozmieszczone w siatce, z odstępami (spacing) wynoszącymi 100 jednostek.
        int spacing = 100; // Odległość między węzłami

        // Inicjalizacja węzłów
        nodes.resize(gridWidth * gridHeight);
        for (int y = 0; y < gridHeight; y++)
        {
            for (int x = 0; x < gridWidth; x++)
            {
                int index = y * gridWidth + x;
                nodes[index].x = x * spacing + 100;
                nodes[index].y = y * spacing + 100;
            }
        }

        // Ustalanie sąsiadów (połączeń)
        // każdy węzeł ma sąsiadów w czterech kierunkach (góra, dół, lewo, prawo), co zapewnia, że BFS będzie przeszukiwać wszystkie możliwe ścieżki.
        for (int y = 0; y < gridHeight; y++)
        {
            for (int x = 0; x < gridWidth; x++)
            {
                int index = y * gridWidth + x;
                if (x > 0) // Połączenie z lewym sąsiadem
                    nodes[index].neighbors.push_back(&nodes[y * gridWidth + (x - 1)]);
                if (x < gridWidth - 1) // Połączenie z prawym sąsiadem
                    nodes[index].neighbors.push_back(&nodes[y * gridWidth + (x + 1)]);
                if (y > 0) // Połączenie z górnym sąsiadem
                    nodes[index].neighbors.push_back(&nodes[(y - 1) * gridWidth + x]);
                if (y < gridHeight - 1) // Połączenie z dolnym sąsiadem
                    nodes[index].neighbors.push_back(&nodes[(y + 1) * gridWidth + x]);
            }
        }
        // Ustawienie węzła początkowego
        // BFS zaczyna od lewego górnego węzła (indeks 0).
        startNode = &nodes[0];
        nodeQueue.push(startNode);
        startNode->inQueue = true;

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);

        if (!nodeQueue.empty())
        {
            Node* currentNode = nodeQueue.front();
            nodeQueue.pop();

            if (!currentNode->visited)
            {
                currentNode->visited = true;

                // Opóźnienie
                // Zmienna delayMilliseconds, która określa czas opóźnienia w milisekundach między odwiedzeniem kolejnych węzłów. 
                // Dzięki temu węzły będą odwiedzane sekwencyjnie, a użytkownik może obserwować proces w czasie rzeczywistym.
                std::this_thread::sleep_for(std::chrono::milliseconds(delayMilliseconds));
                
                for (auto neighbor : currentNode->neighbors)
                {
                    if (!neighbor->visited && !neighbor->inQueue)
                    {
                        nodeQueue.push(neighbor);
                        neighbor->inQueue = true;
                    }
                }
            }
        }

        // Rysowanie węzłów
        for (auto& node : nodes)
        {
            olc::Pixel color = olc::BLUE;
            if (node.visited)
                color = olc::GREEN;
            else if (node.inQueue)
                color = olc::YELLOW;

            FillCircle(node.x, node.y, 20, color);
        }

        // Rysowanie krawędzi
        for (auto& node : nodes)
        {
            for (auto neighbor : node.neighbors)
            {
                DrawLine(node.x, node.y, neighbor->x, neighbor->y, olc::WHITE);
            }
        }

        return true;
    }
};

int main()
{
    BFS_Visualization demo;
    if (demo.Construct(600, 600, 1, 1))
        demo.Start();

    return 0;
}
