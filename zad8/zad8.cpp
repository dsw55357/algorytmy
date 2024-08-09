/*

Code is based  on work javidx9
https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/SmallerProjects/OneLoneCoder_PathFinding_AStar.cpp#L90

*/

#define OLC_PGE_APPLICATION
#include "../olcPixelGameEngine.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

using namespace std;

bool test {false};

struct sNode
{
    bool bObstacle = false;			// Is the node an obstruction?
    bool bVisited = false;			// Have we searched this node before?
    float fGlobalGoal;				// Distance to goal so far
    float fLocalGoal;				// Distance to goal if we took the alternative route
    int x;							// Nodes position in 2D space
    int y;
    vector<sNode*> vecNeighbours;	// Connections to neighbours
    sNode* parent;					// Node connecting to this node that offers shortest parent
};

class olcEngine : public olc::PixelGameEngine
{
public:
	olcEngine()
	{
		sAppName = "Algorytm Dijsktry";
	}

private:
		int nNodeSize = 18;
		int nNodeBorder = 2;

        sNode *nodes = nullptr;
        int nMapWidth = 16;
        int nMapHeight = 16;

        sNode *nodeStart = nullptr;
        sNode *nodeEnd = nullptr;

public:
	bool OnUserCreate() override
	{
        // Calculate map dimensions based on screen size
        nMapWidth = ScreenWidth() / nNodeSize;
        nMapHeight = ScreenHeight() / nNodeSize;  

	std::cout << "nMapWidth: " << nMapWidth << ", nMapHeight: " << nMapHeight << std::endl;

    // Create a 2D array of nodes - this is for convenience of rendering and construction
    // and is not required for the algorithm to work - the nodes could be placed anywhere
    // in any space, in multiple dimensions...

		nodes = new sNode[nMapWidth * nMapHeight];

		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				nodes[y * nMapWidth + x].x = x; // ...because we give each node its own coordinates
				nodes[y * nMapWidth + x].y = y;
				nodes[y * nMapWidth + x].bObstacle = false;
				nodes[y * nMapWidth + x].parent = nullptr;
				nodes[y * nMapWidth + x].bVisited = false;
			}

		// Create connections - in this case nodes are on a regular grid
		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				if(y>0)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 0)]);
				if(y<nMapHeight-1)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 0)]);
				if (x>0)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x - 1)]);
				if(x<nMapWidth-1)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x + 1)]);

				// We can also connect diagonally
				/*if (y>0 && x>0)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x - 1)]);
				if (y<nMapHeight-1 && x>0)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x - 1)]);
				if (y>0 && x<nMapWidth-1)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 1)]);
				if (y<nMapHeight - 1 && x<nMapWidth-1)
					nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 1)]);
				*/
			}


		// Generowanie przeszkód z gęstością 30% (możesz dostosować tę wartość)
		// fObstacleDensity:
		// Jest to parametr funkcji, który określa procentową gęstość przeszkód na mapie. 
		// Na przykład, jeśli fObstacleDensity wynosi 0.3f, to około 30% węzłów będzie przeszkodami.		
		// Inicjalizacja generatora liczb losowych:
		// że za każdym razem, gdy uruchomisz program, przeszkody będą rozmieszczane w sposób losowy.
    	//GenerateRandomObstacles(0.35f);

		// Manually positio the start and end markers so they are not nullptr
		nodeStart = &nodes[(nMapHeight / 2) * nMapWidth + 1];
		nodeEnd = &nodes[(nMapHeight / 2) * nMapWidth + nMapWidth-2];

		// Ustawienie losowych punktów startu i końca
	    //RandomizeStartAndEnd(); 

		return true;
	}

	void RandomizeStartAndEnd()
	{
		srand(time(nullptr)); // Ustawienie ziarenka dla generatora liczb losowych

		// Definiujemy minimalną odległość, np. 5 jednostek
		const float minimalnaOdległość = 5.0f;

		// Losujemy punkt startowy, upewniając się, że nie jest to przeszkoda
		do {
			nodeStart = &nodes[(rand() % nMapHeight) * nMapWidth + (rand() % nMapWidth)];
		} while (nodeStart->bObstacle);

		// Losujemy punkt końcowy, upewniając się, że nie jest to przeszkoda ani punkt startowy
		do {
			nodeEnd = &nodes[(rand() % nMapHeight) * nMapWidth + (rand() % nMapWidth)];
		} while (nodeEnd->bObstacle || nodeEnd == nodeStart);

		// Sprawdzamy, czy odległość między punktem startowym a końcowym jest odpowiednia
		while (sqrtf((nodeStart->x - nodeEnd->x) * (nodeStart->x - nodeEnd->x) + 
					(nodeStart->y - nodeEnd->y) * (nodeStart->y - nodeEnd->y)) < minimalnaOdległość)
		{
			nodeEnd = &nodes[(rand() % nMapHeight) * nMapWidth + (rand() % nMapWidth)];
		}
	}

	// GenerateRandomObstacles(float fObstacleDensity):
	// Funkcja iteruje przez wszystkie węzły na mapie.
	// Dla każdego węzła losuje liczbę z przedziału od 0.0 do 1.0.
	// Jeśli ta liczba jest mniejsza niż fObstacleDensity, węzeł zostaje oznaczony jako przeszkoda (bObstacle = true).
	// W przeciwnym razie węzeł pozostaje wolny od przeszkód (bObstacle = false).
	void GenerateRandomObstacles2(float fObstacleDensity)
	{
		// Ustawienie ziarenka dla generatora liczb 
		// Inicjalizacja generatora liczb losowych:
		// Użycie srand(time(nullptr)) zapewnia, że za każdym razem, gdy uruchomisz program, przeszkody będą rozmieszczane w sposób losowy.
		srand(time(nullptr));

		// Iterujemy po wszystkich węzłach na mapie
		for (int x = 0; x < nMapWidth; x++)
		{
			for (int y = 0; y < nMapHeight; y++)
			{
				// Losujemy liczbę z przedziału 0.0 - 1.0 i sprawdzamy, czy jest mniejsza niż podany wskaźnik przeszkód
				if ((float)rand() / RAND_MAX < fObstacleDensity)
				{
					// Ustawiamy dany węzeł jako przeszkodę
					nodes[y * nMapWidth + x].bObstacle = true;
				} else
				{
					// Jeśli nie, to upewniamy się, że węzeł nie jest przeszkodą
					nodes[y * nMapWidth + x].bObstacle = false;
				}
			}
		}
	}

	void GenerateRandomObstacles(float fObstacleDensity)
	{
		// Ustawienie ziarenka dla generatora liczb 
		srand(time(nullptr));

		// Iterujemy po wszystkich węzłach na mapie
		for (int x = 0; x < nMapWidth; x++)
		{
			for (int y = 0; y < nMapHeight; y++)
			{
				// Losujemy liczbę z przedziału 0.0 - 1.0 i sprawdzamy, czy jest mniejsza niż podany wskaźnik przeszkód
				if ((float)rand() / RAND_MAX < fObstacleDensity)
				{
					// Ustawiamy dany węzeł jako przeszkodę
					nodes[y * nMapWidth + x].bObstacle = true;
				} 
				else
				{
					// Jeśli nie, to upewniamy się, że węzeł nie jest przeszkodą
					nodes[y * nMapWidth + x].bObstacle = false;
				}

				// Dla każdego węzła aktualizujemy listę sąsiadów
				nodes[y * nMapWidth + x].vecNeighbours.clear();

				// Jeżeli dany węzeł nie jest przeszkodą, dodajemy sąsiadów
				if (!nodes[y * nMapWidth + x].bObstacle)
				{
					// Dodajemy sąsiadów tylko wtedy, gdy nie są przeszkodami
					if (y > 0 && !nodes[(y - 1) * nMapWidth + x].bObstacle)
						nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + x]);
					if (y < nMapHeight - 1 && !nodes[(y + 1) * nMapWidth + x].bObstacle)
						nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + x]);
					if (x > 0 && !nodes[y * nMapWidth + (x - 1)].bObstacle)
						nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[y * nMapWidth + (x - 1)]);
					if (x < nMapWidth - 1 && !nodes[y * nMapWidth + (x + 1)].bObstacle)
						nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[y * nMapWidth + (x + 1)]);
				}
			}
		}
	}

	bool Solve_AStar()
	{
		// Reset Navigation Graph - default all node states
		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				nodes[y*nMapWidth + x].bVisited = false;
				nodes[y*nMapWidth + x].fGlobalGoal = INFINITY;
				nodes[y*nMapWidth + x].fLocalGoal = INFINITY;
				nodes[y*nMapWidth + x].parent = nullptr;	// No parents
			}

		auto distance = [](sNode* a, sNode* b) // For convenience
		{
			return sqrtf((a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y));
		};

		auto heuristic = [distance](sNode* a, sNode* b) // So we can experiment with heuristic
		{
			return distance(a, b);
		};

		// Setup starting conditions
		sNode *nodeCurrent = nodeStart;
		nodeStart->fLocalGoal = 0.0f;
		nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

		// Add start node to not tested list - this will ensure it gets tested.
		// As the algorithm progresses, newly discovered nodes get added to this
		// list, and will themselves be tested later
		list<sNode*> listNotTestedNodes;
		listNotTestedNodes.push_back(nodeStart);

		// if the not tested list contains nodes, there may be better paths
		// which have not yet been explored. However, we will also stop 
		// searching when we reach the target - there may well be better
		// paths but this one will do - it wont be the longest.
		while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
		{
			// Sort Untested nodes by global goal, so lowest is first
			listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs){ return lhs->fGlobalGoal < rhs->fGlobalGoal; } );
			
			// Front of listNotTestedNodes is potentially the lowest distance node. Our
			// list may also contain nodes that have been visited, so ditch these...
			while(!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
				listNotTestedNodes.pop_front();

			// ...or abort because there are no valid nodes left to test
			if (listNotTestedNodes.empty())
				break;

			nodeCurrent = listNotTestedNodes.front();
			nodeCurrent->bVisited = true; // We only explore a node once
			
					
			// Check each of this node's neighbours...
			for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
			{
				// ... and only if the neighbour is not visited and is 
				// not an obstacle, add it to NotTested List
				if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
					listNotTestedNodes.push_back(nodeNeighbour);

				// Calculate the neighbours potential lowest parent distance
				float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

				// If choosing to path through this node is a lower distance than what 
				// the neighbour currently has set, update the neighbour to use this node
				// as the path source, and set its distance scores as necessary
				if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
				{
					nodeNeighbour->parent = nodeCurrent;
					nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

					// The best path length to the neighbour being tested has changed, so
					// update the neighbour's score. The heuristic is used to globally bias
					// the path algorithm, so it knows if its getting better or worse. At some
					// point the algo will realise this path is worse and abandon it, and then go
					// and search along the next best path.
					nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
				}
			}	
		}

		return true;
	}


bool Solve_Dijkstra()
{
    // Reset Navigation Graph - default all node states
    for (int x = 0; x < nMapWidth; x++)
        for (int y = 0; y < nMapHeight; y++)
        {
            nodes[y*nMapWidth + x].bVisited = false;
            nodes[y*nMapWidth + x].fGlobalGoal = INFINITY;
            nodes[y*nMapWidth + x].fLocalGoal = INFINITY;
            nodes[y*nMapWidth + x].parent = nullptr;    // No parents
        }

    auto distance = [](sNode* a, sNode* b) // For convenience
    {
        return sqrtf((a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y));
    };

    // Setup starting conditions
    sNode *nodeCurrent = nodeStart;
    nodeStart->fLocalGoal = 0.0f;
    nodeStart->fGlobalGoal = nodeStart->fLocalGoal; // In Dijkstra, fGlobalGoal is the same as fLocalGoal

    // Add start node to not tested list - this will ensure it gets tested.
    list<sNode*> listNotTestedNodes;
    listNotTestedNodes.push_back(nodeStart);

    // if the not tested list contains nodes, there may be better paths
    // which have not yet been explored. However, we will also stop 
    // searching when we reach the target - there may well be better
    // paths but this one will do - it wont be the longest.
    while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd) // Find absolutely shortest path
    {
        // Sort Untested nodes by global goal, so lowest is first
        listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs){ return lhs->fGlobalGoal < rhs->fGlobalGoal; } );
        
        // Front of listNotTestedNodes is potentially the lowest distance node. Our
        // list may also contain nodes that have been visited, so ditch these...
        while(!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
            listNotTestedNodes.pop_front();

        // ...or abort because there are no valid nodes left to test
        if (listNotTestedNodes.empty())
            break;

        nodeCurrent = listNotTestedNodes.front();
        nodeCurrent->bVisited = true; // We only explore a node once
                
        // Check each of this node's neighbours...
        for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
        {
            // ... and only if the neighbour is not visited and is 
            // not an obstacle, add it to NotTested List
            if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
                listNotTestedNodes.push_back(nodeNeighbour);

            // Calculate the neighbours potential lowest parent distance
            float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

            // If choosing to path through this node is a lower distance than what 
            // the neighbour currently has set, update the neighbour to use this node
            // as the path source, and set its distance scores as necessary
            if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
            {
                nodeNeighbour->parent = nodeCurrent;
                nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

                // In Dijkstra's algorithm, there is no heuristic component, so global goal is just the local goal
                nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal;
            }
        }    
    }

    return true;
}



// Definjujemy przeszkody które mają kształty przypominające element te z gry Tetris
struct Tetrimino
{
    vector<pair<int, int>> shape; // Zawiera pary przesunięć (dx, dy) od punktu startowego

    Tetrimino(initializer_list<pair<int, int>> coords) : shape(coords) {}
};

vector<Tetrimino> tetriminos = {
    // Definicje tetrimino
    {{0, 0}, {1, 0}, {2, 0}, {3, 0}}, // I-kształt
    {{0, 0}, {1, 0}, {2, 0}, {1, 1}}, // T-kształt
    {{0, 0}, {1, 0}, {0, 1}, {1, 1}}, // O-kształt
    {{0, 0}, {0, 1}, {1, 1}, {2, 1}}, // L-kształt
    {{0, 0}, {1, 0}, {1, 1}, {2, 1}}, // Z-kształt
};

void PlaceTetrimino(int startX, int startY, Tetrimino& tetrimino)
{
    for (auto& coord : tetrimino.shape)
    {
        int x = startX + coord.first;
        int y = startY + coord.second;

        if (x >= 0 && x < nMapWidth && y >= 0 && y < nMapHeight)
        {
            nodes[y * nMapWidth + x].bObstacle = true;
        }
    }
}

void GenerateRandomTetrisObstacles(float fObstacleDensity)
{
    srand(time(nullptr)); // Inicjalizacja generatora liczb losowych


	for (int x = 0; x < nMapWidth; x++)
		for (int y = 0; y < nMapHeight; y++)
		{
			nodes[y * nMapWidth + x].x = x; // ...because we give each node its own coordinates
			nodes[y * nMapWidth + x].y = y;
			nodes[y * nMapWidth + x].bObstacle = false;
			nodes[y * nMapWidth + x].parent = nullptr;
			nodes[y * nMapWidth + x].bVisited = false;
		}


    for (int x = 0; x < nMapWidth; x++)
    {
        for (int y = 0; y < nMapHeight; y++)
        {
            if ((float)rand() / RAND_MAX < fObstacleDensity)
            {
                Tetrimino& tetrimino = tetriminos[rand() % tetriminos.size()];
				// Funkcja ta umieszcza tetrimino na mapie, ustawiając odpowiednie węzły jako przeszkody.
                PlaceTetrimino(x, y, tetrimino);
            }
        }
    }
}



	bool OnUserUpdate(float fElapsedTime) override
	{
		// Clear Screen
		Clear(olc::BLACK);
        bool quit =  false;

		// Use integer division to nicely get cursor position in node space
		int nSelectedNodeX = GetMouseX() / nNodeSize;
		int nSelectedNodeY = GetMouseY() / nNodeSize;


        if (GetKey(olc::Key::ESCAPE).bPressed) {
			quit = true;
        } 

		if (GetKey(olc::Key::SPACE).bReleased) {
			test = true;
			// GenerateRandomObstacles(0.35f);
			// losowo umieszcza kształty tetrimino na mapie na podstawie wskaźnika gęstości przeszkód.
			GenerateRandomTetrisObstacles(0.05f);
			// 
			RandomizeStartAndEnd();
			Solve_Dijkstra();
			//Solve_AStar();
		}

		if (GetMouse(0).bReleased)
		{
			if(nSelectedNodeX >=0 && nSelectedNodeX < nMapWidth)
				if (nSelectedNodeY >= 0 && nSelectedNodeY < nMapHeight)
				{
					if (GetKey(olc::Key::SHIFT).bHeld)
						// std::cout << "shift" << std::endl;
						nodeStart = &nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX];
					else if (GetKey(olc::Key::CTRL).bHeld)
						nodeEnd = &nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX];
					else
						nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX].bObstacle = !nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX].bObstacle;
					
					//Solve_AStar(); // Solve in "real-time" gives a nice 
					Solve_Dijkstra();
				}			
		}

		// Draw Connections First - lines from this nodes position to its
		// connected neighbour node positions
		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				for (auto n : nodes[y*nMapWidth + x].vecNeighbours)
				{
					DrawLine(x*nNodeSize + nNodeSize / 2, y*nNodeSize + nNodeSize / 2,
						n->x*nNodeSize + nNodeSize / 2, n->y*nNodeSize + nNodeSize / 2, olc::DARK_BLUE);
				}
			}

		// Draw Nodes on top
		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
	            // Fills a rectangle at (x,y) to (x+w,y+h)
				FillRect(x*nNodeSize + nNodeBorder, y*nNodeSize + nNodeBorder, 
					//(x + 1)*nNodeSize - nNodeBorder, (y + 1)*nNodeSize - nNodeBorder, 
                    nNodeSize-nNodeBorder*2, nNodeSize-nNodeBorder*2,
					nodes[y * nMapWidth + x].bObstacle ? olc::GREY : olc::BLUE);

                // DrawLine(x*nNodeSize + nNodeSize / 2, y*nNodeSize + nNodeSize / 2, x*nNodeSize + nNodeSize, y*nNodeSize + nNodeSize,
                	// olc::YELLOW);

				// FillCircle(x*nNodeSize + nNodeSize / 2, y*nNodeSize + nNodeSize / 2, 2, olc::GREEN);
            }     

		// nodeStart = &nodes[(nMapHeight / 2) * nMapWidth + 1];
		// nodeEnd = &nodes[(nMapHeight / 2) * nMapWidth + nMapWidth-2];
		FillCircle(nodeStart->x*nNodeSize + nNodeSize / 2, nodeStart->y*nNodeSize + nNodeSize / 2, 2, olc::RED);
		FillCircle(nodeEnd->x*nNodeSize + nNodeSize / 2, nodeEnd->y*nNodeSize + nNodeSize / 2, 2, olc::GREEN);

		//std::cout << p->x*nNodeSize << ", " << p->y*nNodeSize << std::endl;
		// Draw Path by starting ath the end, and following the parent node trail
		// back to the start - the start node will not have a parent path to follow
		if (nodeEnd != nullptr)
		{
			//std::cout << "if (nodeEnd != nullptr)" << std::endl;

			sNode *p = nodeEnd;
			while (p->parent != nullptr)
			{
				// std::cout << "while (p->parent != nullptr)" << std::endl;
				FillCircle(p->x*nNodeSize + nNodeSize / 2, p->y*nNodeSize + nNodeSize / 2, 2, olc::GREEN);

				DrawLine(p->x*nNodeSize + nNodeSize / 2, p->y*nNodeSize + nNodeSize / 2,
					p->parent->x*nNodeSize + nNodeSize / 2, p->parent->y*nNodeSize + nNodeSize / 2, olc::YELLOW);
				
				// Set next node to this node's parent
				p = p->parent;
			}

        }

		DrawString(15, 15, std::to_string(GetMouseX()) + ", " + std::to_string(GetMouseY()) 
			+ " [" + std::to_string(nSelectedNodeX) + ", " + std::to_string(nSelectedNodeY) + "]", olc::CYAN );
        DrawString(ScreenWidth()-300, ScreenHeight()-15, "Powered by olcPixelGameEngine, 2024(8)", olc::CYAN );

        return !quit;
    }

};

int main() {

	olcEngine app;
	if (app.Construct(512, 480, 2, 2))
		app.Start();


    return 0;
}
