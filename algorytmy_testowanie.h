#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include "algorytmy_sortowania.h"
#include "olcPixelGameEngine.h"

void generateRandomTriangles(std::vector<triangle>& triangles, int count) {
    for (int i = 0; i < count; ++i) {
        triangle t;
        for (int j = 0; j < 3; ++j) {
            t.p[j].x = static_cast<float>(rand()) / RAND_MAX;
            t.p[j].y = static_cast<float>(rand()) / RAND_MAX;
            t.p[j].z = static_cast<float>(rand()) / RAND_MAX;
        }
        triangles.push_back(t);
    }
}

