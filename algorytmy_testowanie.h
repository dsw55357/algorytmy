#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
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

// próba zoptymalizacji kodu testującego, używając wektora par (std::vector<std::pair>)

auto testSortingAlgorithm(std::function<void(std::vector<triangle>&)> sortFunc, std::vector<triangle> triangles) {
    using namespace std::chrono;

    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(triangles);
    auto end = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double> duration = end - start;
    auto duration = duration_cast<microseconds>(end - start).count();

    return duration;
}


void testSortowanieBabelkowe(std::vector<triangle>& triangles) {

    SortowanieBabelkowe(triangles);
}


void testSortowaniePrzezWstawianie(std::vector<triangle>& triangles) {

    SortowaniePrzezWstawianie(triangles);
}


void testHeapSort(std::vector<triangle>& triangles) {

    HeapSort(triangles);
}


void testQuickSort(std::vector<triangle>& triangles) {

    QuickSort(triangles, 0, triangles.size() - 1);
}


void testMergeSort(std::vector<triangle>& triangles) {

    MergeSort(triangles, 0, triangles.size() - 1);
}

void testCountingSort(std::vector<triangle>& triangles) {

    CountingSort(triangles);
}

void testStdSort(std::vector<triangle>& triangles) {

    // Sort triangles from back to front
    sort(triangles.begin(), triangles.end(), [](triangle &t1, triangle &t2)
    {
        float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
        float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
        return z1 > z2;
    });
}


