#ifndef TETRIMINO_H
#define TETRIMINO_H

#include <vector>
#include <utility>      // std::pair
#include <ctime>
#include <iostream>

#include "common.h"

using namespace std;

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

void PlaceTetrimino(int nMapWidth, int nMapHeight, sNode *nodes, int startX, int startY, Tetrimino& tetrimino)
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

bool CanPlaceTetrimino(int nMapWidth, int nMapHeight, sNode *nodes, int startX, int startY, Tetrimino& tetrimino)
{
    for (auto& coord : tetrimino.shape)
    {
        int x = startX + coord.first;
        int y = startY + coord.second;

        // Sprawdzenie, czy węzeł jest w granicach mapy
        if (x < 0 || x >= nMapWidth || y < 0 || y >= nMapHeight)
            return false;

        // Sprawdzenie, czy węzeł nie jest już przeszkodą
        if (nodes[y * nMapWidth + x].bObstacle)
            return false;
    }

    return true;
}

// GenerateRandomTetrisObstacles: Ta funkcja generuje losowo przeszkody w kształcie tetrimino. Przed umieszczeniem kształtu próbuje znaleźć odpowiednie miejsce bez kolizji, podejmując maksymalnie 10 prób.

void GenerateRandomTetrisObstacles(int nMapWidth, int nMapHeight, sNode *nodes, float fObstacleDensity=0.05f)
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
                //PlaceTetrimino(x, y, tetrimino);

               	// Spróbuj znaleźć odpowiednie miejsce do umieszczenia tetrimino
                for (int attempt = 0; attempt < 10; ++attempt) // Próbujemy maksymalnie 10 razy znaleźć odpowiednie miejsce
                {
                    int randomX = rand() % nMapWidth;
                    int randomY = rand() % nMapHeight;

					// CanPlaceTetrimino: Ta funkcja sprawdza, czy tetrimino może zostać umieszczone na danej pozycji (startX, startY) bez kolizji z istniejącymi przeszkodami i czy mieści się w granicach mapy.
                    if (CanPlaceTetrimino(nMapWidth, nMapHeight, nodes, randomX, randomY, tetrimino))
                    {
						// PlaceTetrimino: Funkcja ta faktycznie umieszcza tetrimino na mapie, ustawiając odpowiednie węzły jako przeszkody, tylko jeśli CanPlaceTetrimino zwróci true.
                        PlaceTetrimino(nMapWidth, nMapHeight, nodes, randomX, randomY, tetrimino);
                        break;
                    }
                }
            }
        }
    }
}

#endif // TETRIMINO_H