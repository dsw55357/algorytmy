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
