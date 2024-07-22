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
            if (j_z > key_z)
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
