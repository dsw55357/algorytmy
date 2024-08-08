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
    // for (const auto& triangle : triangles) {
    //     std::cout << " trojkat " << ", z1:" << triangle.p[0].z << ", z2:" << triangle.p[1].z << ", z3:" << triangle.p[2].z << std::endl;
    // }

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
    int scaleFactor = 1000;  // Skalowanie wartości głębokości
    int range = static_cast<int>(std::ceil(maxDepth * scaleFactor - minDepth * scaleFactor)) + 1;
    std::vector<int> count(range, 0);

    // Zliczanie wystąpień
    for (const auto& tri : triangles) {
        int depthIndex = static_cast<int>(std::floor(calculateAverageDepth(tri) * scaleFactor - minDepth * scaleFactor));
        count[depthIndex]++;
    }

    // Kumulacja zliczeń
    for (size_t i = 1; i < count.size(); ++i) {
        count[i] += count[i - 1];
    }

    // Tworzenie tablicy wyjściowej w porządku malejącym
    std::vector<triangle> output(triangles.size());
    for (const auto& tri : triangles) {
        int depthIndex = static_cast<int>(std::floor(calculateAverageDepth(tri) * scaleFactor - minDepth * scaleFactor));
        output[count[depthIndex] - 1] = tri;
        count[depthIndex]--;
    }

    // Przepisanie wyników do tablicy wejściowej
    triangles = output;

    // for (auto& tri : triangles) {
    //     std::cout << "0: " <<tri.p[0].z << ", 1: " <<tri.p[1].z << ", 2: " <<tri.p[2].z << std::endl;
    // }
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

/*

Sortowanie pozycyjne

*/

// Pomocnicza funkcja do znajdowania maksymalnej wartości w wektorze
int getMaxDepth(const std::vector<triangle>& triangles) {
    float maxDepth = std::numeric_limits<float>::lowest();
    for (const auto& tri : triangles) {
        float depth = calculateAverageDepth(tri);
        if (depth > maxDepth) {
            maxDepth = depth;
        }
    }
    return static_cast<int>(maxDepth * 1000); // Przeskalowanie głębokości do liczb całkowitych
}

// Pomocnicza funkcja do znajdowania minimalnej wartości w wektorze
float getMinDepth(const std::vector<triangle>& triangles) {
    float minDepth = std::numeric_limits<float>::max();
    for (const auto& tri : triangles) {
        float depth = calculateAverageDepth(tri);
        if (depth < minDepth) {
            minDepth = depth;
        }
    }
    return minDepth;
}

// Pomocnicza funkcja do sortowania przez zliczanie na podstawie pozycji cyfr
void countingSortTriangles2(std::vector<triangle>& triangles, int exp) {
    int n = triangles.size();
    std::vector<triangle> output(n);
    int count[10] = {0};

    // Zliczanie wystąpień cyfr
    for (int i = 0; i < n; i++) {
        int depth = static_cast<int>(calculateAverageDepth(triangles[i]) * 1000); // Przeskalowanie głębokości do liczb całkowitych
        // count[(depth / exp) % 10]++;
        int index = (depth / exp) % 10;
        if (index < 0 || index >= 10) {
            std::cerr << "Błąd: indeks " << index << " jest poza zakresem dla depth " << depth << " i exp " << exp << std::endl;
            continue;
        }
        count[index]++;
    }

    // Kumulacja zliczeń
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    // Tworzenie tablicy wyjściowej
    for (int i = n - 1; i >= 0; i--) {
        int depth = static_cast<int>(calculateAverageDepth(triangles[i]) * 1000); // Przeskalowanie głębokości do liczb całkowitych
        // output[count[(depth / exp) % 10] - 1] = triangles[i];
        // count[(depth / exp) % 10]--;
        int index = (depth / exp) % 10;
        if (index < 0 || index >= 10) {
            std::cerr << "Błąd: indeks " << index << " jest poza zakresem dla depth " << depth << " i exp " << exp << std::endl;
            continue;
        }
        output[count[index] - 1] = triangles[i];
        count[index]--;
    }

    // std::reverse(output.begin(), output.end());

    // Przepisanie wyników do tablicy wejściowej
    for (int i = 0; i < n; i++) {
        triangles[i] = output[i];
    }
}


// Pomocnicza funkcja do sortowania przez zliczanie na podstawie pozycji cyfr
void countingSortTriangles(std::vector<triangle>& triangles, int exp, float minDepth) {
    int n = triangles.size();
    std::vector<triangle> output(n);
    int count[10] = {0};

    // Zliczanie wystąpień cyfr
    for (int i = 0; i < n; i++) {
        int depth = static_cast<int>((calculateAverageDepth(triangles[i]) - minDepth) * 1000); // Przeskalowanie głębokości do liczb całkowitych
        count[(depth / exp) % 10]++;
    }

    // Kumulacja zliczeń
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    // Tworzenie tablicy wyjściowej
    for (int i = n - 1; i >= 0; i--) {
        int depth = static_cast<int>((calculateAverageDepth(triangles[i]) - minDepth) * 1000); // Przeskalowanie głębokości do liczb całkowitych
        output[count[(depth / exp) % 10] - 1] = triangles[i];
        count[(depth / exp) % 10]--;
    }

    // std::reverse(output.begin(), output.end());

    // Przepisanie wyników do tablicy wejściowej
    for (int i = 0; i < n; i++) {
        triangles[i] = output[i];
    }
}

void RadixSort(std::vector<triangle>& triangles) {
    // int m = getMaxDepth(triangles);
    float minDepth = getMinDepth(triangles);
    float maxDepth = getMaxDepth(triangles);
    float offset = minDepth < 0 ? -minDepth : 0;
    int m = static_cast<int>((maxDepth - minDepth) * 1000);

    // Dodanie przesunięcia do wszystkich wartości głębokości
    for (auto& tri : triangles) {
        for (int i = 0; i < 3; i++) {
            tri.p[i].z += offset;
        }
    }

    for (int exp = 1; m / exp > 0; exp *= 10) {
        // countingSortTriangles(triangles, exp);
        countingSortTriangles(triangles, exp, minDepth);
    }

}

/*

!! Sortowanie przez zliczanie (i sortowanie pozycyjne) jest głównie zaprojektowane do pracy z liczbami nieujemnymi.  !!

 sortowanie pozycyjne (Radix Sort) często korzysta z sortowania przez zliczanie (Counting Sort) jako kroku pomocniczego do sortowania liczb na poszczególnych pozycjach cyfry. Jest to dlatego, że sortowanie przez zliczanie jest stabilne i działa w czasie liniowym, co sprawia, że jest idealne do sortowania cyfr na poszczególnych pozycjach bez zmiany kolejności elementów o tych samych wartościach cyfr.

Dlaczego sortowanie przez zliczanie jest używane w Radix Sort?
- Stabilność: Sortowanie przez zliczanie jest stabilne, co oznacza, że nie zmienia względnego porządku elementów o tej samej wartości. To jest kluczowe w Radix Sort, ponieważ musimy zachować porządek sortowania według mniej znaczących cyfr, kiedy sortujemy według bardziej znaczących cyfr.
- Złożoność czasowa: Sortowanie przez zliczanie działa w czasie liniowym O(n + k), gdzie n to liczba elementów, a k to zakres wartości. W Radix Sort sortujemy cyfry, więc k jest stałe i małe (np. 10 dla systemu dziesiętnego), co sprawia, że jest bardzo efektywne.
 - Bezpośrednie zastosowanie: Można bezpośrednio użyć sortowania przez zliczanie do sortowania cyfr na każdej pozycji, co upraszcza implementację Radix Sort.

*/

// Funkcja sortująca kubełkowo trójkąty
// void bucketSortTriangles(std::vector<triangle>& triangles) {
//     int n = triangles.size();
//     if (n <= 0)
//         return;

//     // Znajdź minimalną i maksymalną wartość głębokości
//     float minDepth = std::numeric_limits<float>::max();
//     float maxDepth = std::numeric_limits<float>::lowest();
    
//     for (const auto& tri : triangles) {
//         float depth = calculateAverageDepth(tri);
//         if (depth < minDepth) minDepth = depth;
//         if (depth > maxDepth) maxDepth = depth;
//     }

//     // Przeskalowanie głębokości
//     minDepth *= 1000;
//     maxDepth *= 1000;
    
//     // Tworzymy n pustych kubełków
//     std::vector<std::vector<triangle>> buckets(n);

//     // Umieszczamy trójkąty w odpowiednich kubełkach
//     for (const auto& tri : triangles) {
//         int bucketIndex = static_cast<int>(n * (calculateAverageDepth(tri) * 1000 - minDepth) / (maxDepth - minDepth + 1));
//         buckets[bucketIndex].push_back(tri);
//     }

//     // Sortujemy każdy kubełek
//     for (auto& bucket : buckets) {
//         std::sort(bucket.begin(), bucket.end(), [](const triangle& t1, const triangle& t2) {
//             return calculateAverageDepth(t1) > calculateAverageDepth(t2);
//         });
//     }

//     // Scalanie wszystkich kubełków do tablicy wynikowej
//     int index = 0;
//     for (const auto& bucket : buckets) {
//         for (const auto& tri : bucket) {
//             triangles[index++] = tri;
//         }
//     }

//     std::sort(triangles.begin(), triangles.end(), [](const triangle& t1, const triangle& t2) {
//             return calculateAverageDepth(t1) > calculateAverageDepth(t2);
//         });
// }

void bucketSortTriangles(std::vector<triangle>& triangles) {

    int n = triangles.size();
    if (n <= 0)
        return;

    // for (const auto& triangle : triangles) {
    //     std::cout << " trojkat " << ", z1:" << triangle.p[0].z << ", z2:" << triangle.p[1].z << ", z3:" << triangle.p[2].z << std::endl;
    // }

    // Znajdź minimalną i maksymalną wartość głębokości
    float minDepth = std::numeric_limits<float>::max();
    float maxDepth = std::numeric_limits<float>::lowest();
    
    for (const auto& tri : triangles) {
        float depth = calculateAverageDepth(tri);
        if (depth < minDepth) minDepth = depth;
        if (depth > maxDepth) maxDepth = depth;
        std::cout << " trojkat >> " << ", minDepth:" << minDepth << ", maxDepth:" << maxDepth << std::endl;
        std::cout << " trojkat >> " << ", z1:" << tri.p[0].z << ", z2:" << tri.p[1].z << ", z3:" << tri.p[2].z << std::endl;
    }

    // Przeskalowanie głębokości do zakresu całkowitego
    minDepth *= 100000;
    maxDepth *= 100000;
    
    int range = static_cast<int>(std::ceil(maxDepth - minDepth)) + 1;
    // std::cout << "Range : " << range << std::endl;
    std::vector<int> count(range, 0);

    // Zliczanie wystąpień
    for (const auto& tri : triangles) {
        int depthIndex = static_cast<int>(std::floor(calculateAverageDepth(tri) * 100000 - minDepth));
        count[depthIndex]++;
    }

    // Kumulacja zliczeń
    for (size_t i = 1; i < count.size(); ++i) {
        count[i] += count[i - 1];
    }

    // Tworzenie tablicy wyjściowej w porządku malejącym
    std::vector<triangle> output(triangles.size());
    for (int i = n - 1; i >= 0; --i) {
        int depthIndex = static_cast<int>(std::floor(calculateAverageDepth(triangles[i]) * 100000 - minDepth));
        output[--count[depthIndex]] = triangles[i];
        // std::cout << count[depthIndex] << " trojkat " << calculateAverageDepth(triangles[i]) << ", z1:" << triangles[i].p[0].z << ", z2:" << triangles[i].p[1].z << ", z3:" << triangles[i].p[2].z << std::endl;
    }

    // Przepisanie wyników do tablicy wejściowej
    // std::cout << count.size() << std::endl;
    //triangles = output;
    triangles.clear();
    std::copy(output.begin(), output.end(),
              std::back_inserter(triangles));

}
