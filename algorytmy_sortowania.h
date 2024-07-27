#ifndef ALGORYTMY_SORTOWANIA_H
#define ALGORYTMY_SORTOWANIA_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

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


/*

Algorytm sortowania przez kopcowanie w miejscu (ang. in-place heap sort) polega na tym, że najpierw przekształcamy tablicę wejściową w kopiec, a następnie iteracyjnie przenosimy największy element z kopca do końca tablicy, jednocześnie przywracając własność kopca dla pozostałej części tablicy. Dzięki temu nie potrzebujemy dodatkowej pamięci poza samą tablicą wejściową.

Sortowanie przez kopcowanie (Heap Sort) jest przykładem algorytmu sortowania w miejscu (in-place sorting). Oznacza to, że sortowanie odbywa się bez użycia dodatkowej pamięci poza samą tablicą wejściową. Heap Sort przekształca tablicę wejściową w kopiec maksymalny lub kopiec minimalny, a następnie iteracyjnie przenosi największy lub najmniejszy element z kopca do końca tablicy, przywracając własność kopca dla pozostałej części tablicy. Proces ten odbywa się bez alokacji dodatkowej pamięci, co czyni go algorytmem sortowania w miejscu.

- Sortowanie przez kopcowanie: Jest to algorytm sortowania, który używa struktury danych zwanej kopcem. Może być zrealizowany jako sortowanie w miejscu.
- Sortowanie w miejscu: Oznacza, że algorytm sortowania działa bez użycia dodatkowej pamięci, oprócz tej potrzebnej do przechowywania danych wejściowych. Heap Sort jest jednym z takich algorytmów.


*/


/*

Algorytm Quicksort jest jednym z najszybszych algorytmów sortowania dla dużych zbiorów danych. Jego główną zaletą jest efektywność i możliwość sortowania "w miejscu", co oznacza, że nie wymaga dodatkowej pamięci oprócz samego zbioru danych.

Jak działa Quicksort:
- Wybór pivota: Algorytm wybiera element z tablicy, który nazywa się pivotem.
- Podział: Tablica jest podzielona na dwie podtablice: jedna zawierająca elementy mniejsze od pivota, a druga zawierająca elementy większe od pivota.
- Rekursja: Algorytm jest rekurencyjnie stosowany do dwóch podtablic.
- Scalanie: Ponieważ tablice są sortowane w miejscu, nie jest wymagane dodatkowe scalanie.

*/
void QuickSort(std::vector<triangle>& triangles, int low, int high);

/*

Sortowanie przez scalanie

Sortowanie przez scalanie (Merge Sort) to algorytm sortowania działający na zasadzie dziel i zwyciężaj. Algorytm dzieli tablicę na mniejsze podtablice, sortuje każdą z nich, a następnie scala posortowane podtablice, aby uzyskać ostateczną, posortowaną tablicę. Jest to algorytm stabilny i efektywny, szczególnie dla dużych zbiorów danych.

Jak działa Merge Sort
- Podział: Tablica jest rekurencyjnie dzielona na dwie połowy, aż każda podtablica ma jeden element.
- Scalanie: Posortowane podtablice są scalane, aby utworzyć większe, posortowane tablice, aż do uzyskania jednej, ostatecznej, posortowanej tablicy.

*/

void MergeSort(std::vector<triangle>& triangles, int left, int right);


/*

Sortowanie przez zliczanie (Counting Sort)
Sortowanie przez zliczanie (Counting Sort) jest algorytmem sortowania oparte na kluczach (wartościach), które są małymi liczbami całkowitymi. Jest to algorytm stabilny, ale działa efektywnie tylko wtedy, gdy zakres kluczy (wartości) jest znany i stosunkowo mały.


Zasada działania
- Zliczanie wystąpień: Dla każdej wartości w tablicy wejściowej algorytm zlicza, ile razy dana wartość występuje. Wyniki są zapisywane w tablicy pomocniczej.
- Kumulacja: Tablica pomocnicza jest następnie modyfikowana tak, aby każda komórka zawierała sumę wartości z poprzednich komórek. W ten sposób tablica pomocnicza będzie zawierać informacje o tym, gdzie każda wartość powinna się znaleźć w tablicy wyjściowej.
- Tworzenie tablicy wyjściowej: Algorytm przechodzi przez tablicę wejściową i umieszcza każdą wartość na odpowiedniej pozycji w tablicy wyjściowej, korzystając z informacji z tablicy pomocniczej.

Zastosowanie
Sortowanie przez zliczanie jest szczególnie użyteczne w przypadku:

Sortowania dużych zestawów danych z niewielkim zakresem wartości.
Sortowania danych całkowitoliczbowych, takich jak oceny egzaminów, wiek osób w populacji itp.

*/

void CountingSort(std::vector<triangle>& triangles);

/*

Implementacja testu wydajności algorytmów sortowania
Dla prostoty i łatwości implementacji, wykonamy sekwencyjne testowanie algorytmów sortowania. 

*/





#endif // ALGORYTMY_SORTOWANIA_H
