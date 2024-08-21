#ifndef COMMON_H
#define COMMON_H

#include <vector>

#include "../olcPixelGameEngine.h"

using namespace std;

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
    olc::Pixel color;               // Kolor węzła
};

#endif