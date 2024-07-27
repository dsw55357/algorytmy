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


void Merge(std::vector<triangle>& triangles, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<triangle> L(n1);
    std::vector<triangle> R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = triangles[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = triangles[mid + 1 + j];

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2)
    {
        float z1 = (L[i].p[0].z + L[i].p[1].z + L[i].p[2].z) / 3.0f;
        float z2 = (R[j].p[0].z + R[j].p[1].z + R[j].p[2].z) / 3.0f;

        if (z1 >= z2)
        {
            triangles[k] = L[i];
            i++;
        }
        else
        {
            triangles[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        triangles[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        triangles[k] = R[j];
        j++;
        k++;
    }
}

void MergeSort(std::vector<triangle>& triangles, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        MergeSort(triangles, left, mid);
        MergeSort(triangles, mid + 1, right);

        Merge(triangles, left, mid, right);
    }
}

// Funkcja obliczająca średnią głębokość trójkąta
float calculateAverageDepth(const triangle& tri) {
    return (tri.p[0].z + tri.p[1].z + tri.p[2].z) / 3.0f;
}

// Funkcja sortowania trójkątów przez zliczanie
void CountingSort(std::vector<triangle>& triangles) {
/*

    if (triangles.empty()) return;

    // Znajdź minimalną i maksymalną wartość głębokości
    float minDepth = std::numeric_limits<float>::max();
    float maxDepth = std::numeric_limits<float>::lowest();
    
    for (const auto& tri : triangles) {
        float depth = calculateAverageDepth(tri);
        if (depth < minDepth) minDepth = depth;
        if (depth > maxDepth) maxDepth = depth;
    }

    // Przeskalowanie głębokości do zakresu całkowitego
    int range = static_cast<int>(std::ceil(maxDepth - minDepth)) + 1;
    std::vector<int> count(range, 0);

    // Zliczanie wystąpień
    for (const auto& tri : triangles) {
        int depthIndex = static_cast<int>(std::floor(calculateAverageDepth(tri) - minDepth));
        count[depthIndex]++;
    }

    // Kumulacja zliczeń
    for (size_t i = 1; i < count.size(); ++i) {
        count[i] += count[i - 1];
    }

    // Tworzenie tablicy wyjściowej
    std::vector<triangle> output(triangles.size());
    for (int i = triangles.size() - 1; i >= 0; --i) {
        int depthIndex = static_cast<int>(std::floor(calculateAverageDepth(triangles[i]) - minDepth));
        output[count[depthIndex] - 1] = triangles[i];
        count[depthIndex]--;
    }
    
    // Odwrócenie tablicy wyjściowej, aby trójkąty z większą głębokością były na początku
    //std::reverse(output.begin(), output.end());

    // Przepisanie wyników do tablicy wejściowej
    triangles = output;
    */

    if (triangles.empty()) return;

    // Znajdź minimalną i maksymalną wartość głębokości
    float minDepth = std::numeric_limits<float>::max();
    float maxDepth = std::numeric_limits<float>::lowest();
    
    for (const auto& tri : triangles) {
        float depth = calculateAverageDepth(tri);
        if (depth < minDepth) minDepth = depth;
        if (depth > maxDepth) maxDepth = depth;
    }

    // Przeskalowanie głębokości do zakresu całkowitego
    int range = static_cast<int>(std::ceil(maxDepth - minDepth)) + 1;
    std::vector<int> count(range, 0);

    // Zliczanie wystąpień
    for (const auto& tri : triangles) {
        int depthIndex = static_cast<int>(std::floor(calculateAverageDepth(tri) - minDepth));
        count[depthIndex]++;
    }

    // Kumulacja zliczeń
    for (size_t i = 1; i < count.size(); ++i) {
        count[i] += count[i - 1];
    }

    // Tworzenie tablicy wyjściowej w porządku malejącym
    std::vector<triangle> output(triangles.size());
    for (const auto& tri : triangles) {
        int depthIndex = static_cast<int>(std::floor(calculateAverageDepth(tri) - minDepth));
        output[count[depthIndex] - 1] = tri;
        count[depthIndex]--;
    }

    // Przepisanie wyników do tablicy wejściowej
    triangles = output;

/*

Sortowanie przez zliczanie jest generalnie używane do sortowania w porządku rosnącym, ale możemy łatwo dostosować algorytm, aby sortował w porządku malejącym. Wystarczy zmienić sposób wstawiania elementów do tablicy wyjściowej.

Sortowanie przez zliczanie w porządku malejącym
W sortowaniu przez zliczanie tworzysz tablicę zliczeń, a następnie używasz jej do wstawiania elementów w odpowiednie pozycje w tablicy wyjściowej. Aby uzyskać sortowanie malejące, musimy wstawiać elementy od końca do początku.

Wyjaśnienia
Obliczanie średniej głębokości: Funkcja calculateAverageDepth oblicza średnią głębokość dla danego trójkąta.
Znajdowanie zakresu głębokości: Minimalna i maksymalna głębokość są obliczane na podstawie danych trójkątów.
Zliczanie wystąpień: Głębokości trójkątów są przeskalowane do indeksów i ich wystąpienia są zliczane.
Kumulacja zliczeń: Tablica zliczeń jest modyfikowana, aby zawierała skumulowane sumy.
Tworzenie tablicy wyjściowej w porządku malejącym: Trójkąty są wstawiane do tablicy wyjściowej od końca, aby uzyskać porządek malejący.
Przepisanie wyników: Tablica wynikowa jest kopiowana z powrotem do tablicy wejściowej.



*/

}