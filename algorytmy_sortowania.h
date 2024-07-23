#ifndef ALGORYTMY_SORTOWANIA_H
#define ALGORYTMY_SORTOWANIA_H

#include <vector>

#include "matrix.h"

/*

sortowanie bąbelkowe, przez wstawianie i przez wybieranie mają średnią i najgorszą złożoność czasową O(n^2).

*/

void SortowanieBabelkowe(std::vector<triangle>& triangles);
void SortowaniePrzezWstawianie(std::vector<triangle>& triangles);

/*

Sortowanie przez kopcowanie używa struktury danych znanej jako kopiec (ang. heap), która jest zwykle implementowana za pomocą tablicy.

Sortowanie: Następnie wymienia pierwszy element (największy w przypadku kopca maksymalnego) z ostatnim elementem kopca, zmniejsza rozmiar kopca i przywraca własność kopca. Proces ten jest powtarzany, aż kopiec zostanie opróżniony.

Sortowanie przez kopcowanie ma złożoność czasową O(nlogn).

*/

void HeapSort(std::vector<triangle>& triangles);


#endif // ALGORYTMY_SORTOWANIA_H
