
### Wyszukiwanie liniowe z dowodem poprawności za pomocą niezmiennika pętli

Zanim przejdziemy do kodu, najpierw przypomnijmy sobie trzy kluczowe własności niezmiennika pętli:
1. **Inicjalizacja**: Niezmiennik musi być prawdziwy przed rozpoczęciem pętli.
2. **Utrzymanie**: Niezmiennik musi być prawdziwy po każdej iteracji pętli.
3. **Zakończenie**: Gdy pętla się kończy, niezmiennik i warunek zakończenia pętli dają sensowny wynik.

### Wyszukiwanie liniowe

Algorytm wyszukiwania liniowego przegląda tablicę od lewej do prawej, porównując każdy element z wartością `x`. Jeśli znajdzie element równy `x`, zwraca jego indeks, w przeciwnym razie zwraca `None`.

### Kod w Pythonie

```python
def linear_search(A, x):
    # Inicjalizacja: przed rozpoczęciem pętli, żadna wartość w przeszukanym zakresie (początkowo puste) nie jest równa x.
    for i in range(len(A)):
        # Utrzymanie: jeżeli x nie znajduje się w A[0:i], to sprawdzamy dalej.
        if A[i] == x:
            return i  # Znaleziono element, zwracamy indeks
    # Zakończenie: jeśli przeszukaliśmy całą tablicę, a nie znaleźliśmy x, zwracamy None.
    return None

# Przykładowe dane wejściowe
A = [5, 3, 8, 2, 7, 1]
x = 7

# Uruchomienie wyszukiwania liniowego
result = linear_search(A, x)

if result is not None:
    print(f"Znaleziono wartość {x} na indeksie {result}")
else:
    print(f"Wartość {x} nie występuje w tablicy")
```

### Niezmiennik pętli

1. **Inicjalizacja**:
   - Przed pierwszą iteracją (na początku pętli), żaden element nie został sprawdzony. Niezmiennik mówi, że elementy przeszukane do tej pory (początkowo pusta część) nie zawierają wartości `x`. To jest prawda, ponieważ żadna część tablicy jeszcze nie została przeszukana.

2. **Utrzymanie**:
   - Po każdej iteracji pętli sprawdzamy nowy element `A[i]`. Jeśli `A[i]` jest równe `x`, algorytm natychmiast zwraca `i`. W przeciwnym razie niezmiennik mówi, że przeszukane elementy (od początku tablicy do bieżącego indeksu) nie zawierają wartości `x`, co pozostaje prawdą, ponieważ sprawdzamy wszystkie elementy po kolei.

3. **Zakończenie**:
   - Pętla kończy się, gdy przeszukaliśmy całą tablicę. Jeśli pętla przeszła przez wszystkie elementy bez znalezienia `x`, zwracamy `None`. W takim przypadku niezmiennik gwarantuje, że w żadnym z elementów tablicy nie było wartości `x`.

### Dowód poprawności:

- **Inicjalizacja**: Przed rozpoczęciem pętli nie sprawdzono żadnych elementów, więc nie ma dowodów na istnienie `x` w przeszukanym fragmencie (który początkowo jest pusty). Stąd niezmiennik jest prawdziwy.
  
- **Utrzymanie**: Na każdej iteracji sprawdzamy, czy obecny element `A[i]` jest równy `x`. Jeśli tak, zwracamy indeks, a więc algorytm działa poprawnie. Jeśli nie, kontynuujemy wyszukiwanie, a niezmiennik mówi, że nie znaleziono `x` w sprawdzonych elementach.

- **Zakończenie**: Kiedy pętla się kończy (sprawdziliśmy wszystkie elementy tablicy), jeśli nie znaleziono `x`, zwracamy `None`, co jest poprawnym wynikiem.

### Złożoność czasowa:

- Algorytm ma złożoność **O(n)**, ponieważ w najgorszym przypadku musimy przeszukać wszystkie elementy tablicy.

### Podsumowanie:
Ten algorytm jest poprawny zgodnie z zasadami niezmiennika pętli. Na każdej iteracji pętli niezmiennik gwarantuje, że dotychczasowe sprawdzenie elementów nie zawiera wartości `x`.