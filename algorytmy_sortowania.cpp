#include "algorytmy_sortowania.h"

void SortowaniePrzezWstawianie(std::vector<triangle>& triangles)
{
    for (int i = 1; i < triangles.size(); ++i)
    {
        triangle key = triangles[i];
        float key_z = (key.p[0].z + key.p[1].z + key.p[2].z) / 3.0f;
        int j = i - 1;

        while (j >= 0)
        {
            float j_z = (triangles[j].p[0].z + triangles[j].p[1].z + triangles[j].p[2].z) / 3.0f;
            if (j_z < key_z)
            {
                triangles[j + 1] = triangles[j];
                --j;
            }
            else
            {
                break;
            }
        }
        triangles[j + 1] = key;
    }
}

void SortowanieBabelkowe(std::vector<triangle>& triangles)
{
    bool swapped;
    for (size_t i = 0; i < triangles.size() - 1; ++i)
    {
        swapped = false;
        for (size_t j = 0; j < triangles.size() - i - 1; ++j)
        {
            float z1 = (triangles[j].p[0].z + triangles[j].p[1].z + triangles[j].p[2].z) / 3.0f;
            float z2 = (triangles[j + 1].p[0].z + triangles[j + 1].p[1].z + triangles[j + 1].p[2].z) / 3.0f;
            if (z1 < z2)
            {
                std::swap(triangles[j], triangles[j + 1]);
                swapped = true;
            }
        }
        // If no two elements were swapped by inner loop, then break
        if (!swapped)
            break;
    }
}


void Heapify(std::vector<triangle>& triangles, int n, int i)
{
    int largest = i; // Initialize largest as root
    int left = 2 * i + 1; // left = 2*i + 1
    int right = 2 * i + 2; // right = 2*i + 2

    float largest_z = (triangles[largest].p[0].z + triangles[largest].p[1].z + triangles[largest].p[2].z) / 3.0f;

    // If left child is larger than root
    if (left < n)
    {
        float left_z = (triangles[left].p[0].z + triangles[left].p[1].z + triangles[left].p[2].z) / 3.0f;
        if (left_z < largest_z)
        {
            largest = left;
            largest_z = left_z;
        }
    }

    // If right child is larger than largest so far
    if (right < n)
    {
        float right_z = (triangles[right].p[0].z + triangles[right].p[1].z + triangles[right].p[2].z) / 3.0f;
        if (right_z < largest_z)
        {
            largest = right;
            largest_z = right_z;
        }
    }

    // If largest is not root
    if (largest != i)
    {
        std::swap(triangles[i], triangles[largest]);

        // Recursively heapify the affected sub-tree
        Heapify(triangles, n, largest);
    }
}

void HeapSort(std::vector<triangle>& triangles)
{
    int n = triangles.size();

    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        Heapify(triangles, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--)
    {
        // Move current root to end
        std::swap(triangles[0], triangles[i]);

        // Call max heapify on the reduced heap
        Heapify(triangles, i, 0);
    }
}


int Partition(std::vector<triangle>& triangles, int low, int high)
{
    float pivot_z = (triangles[high].p[0].z + triangles[high].p[1].z + triangles[high].p[2].z) / 3.0f;
    int i = low - 1;

    for (int j = low; j <= high - 1; j++)
    {
        float j_z = (triangles[j].p[0].z + triangles[j].p[1].z + triangles[j].p[2].z) / 3.0f;
        if (j_z > pivot_z)
        {
            i++;
            std::swap(triangles[i], triangles[j]);
        }
    }
    std::swap(triangles[i + 1], triangles[high]);
    return i + 1;
}

void QuickSort(std::vector<triangle>& triangles, int low, int high)
{
    if (low < high)
    {
        int pi = Partition(triangles, low, high);
        QuickSort(triangles, low, pi - 1);
        QuickSort(triangles, pi + 1, high);
    }
}

