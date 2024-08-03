import pygame
import random
import math

"""

Przykład kodu, który ilustruje działanie algorytmu Kruskala na prostym grafie za pomocą Pygame.


Wierzchołki grafu są rozmieszczone losowo, a krawędzie MST są rysowane w kolorze niebieskim z krótkim opóźnieniem, aby zilustrować proces działania algorytmu.

Wyjaśnienie kodu

1. Inicjalizacja Pygame:

 - Ustawienia okna i podstawowych kolorów.

2.  Klasy Edge i DisjointSet:

- Edge reprezentuje krawędź w grafie.
- DisjointSet implementuje strukturę zbiorów rozłącznych (Union-Find).

Funkcja redraw_graph:

- Rysuje graf na ekranie. Krawędzie MST są rysowane na niebiesko, a pozostałe na czarno. - - Wierzchołki są rysowane na czerwono.

Algorytm Kruskala:

- Implementacja algorytmu Kruskala, która na bieżąco rysuje aktualny stan grafu po dodaniu każdej krawędzi do MST.

Funkcja main:

- Inicjalizuje przykładowe wierzchołki i krawędzie.
- Wywołuje algorytm Kruskala.
 -Główna pętla Pygame, która rysuje graf i obsługuje zdarzenia.


"""

# Inicjalizacja Pygame
pygame.init()

# Parametry okna
WIDTH, HEIGHT = 800, 600
WIN = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Algorytm Kruskala")

# Kolory
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
CYAN = (0, 255, 255)  # Definicja koloru CYAN
GREY = (192, 192, 192)

# Klasa reprezentująca krawędź
class Edge:
    def __init__(self, u, v, weight):
        self.u = u
        self.v = v
        self.weight = weight

# Klasa reprezentująca zbiór rozłączny (Union-Find)
# Klasa DisjointSet (znana również jako Union-Find) jest strukturą danych używaną do efektywnego zarządzania rozłącznymi zbiorami. W kontekście algorytmu Kruskala jest używana do śledzenia, które wierzchołki grafu są połączone, aby zapobiec tworzeniu cykli podczas budowania minimalnego drzewa rozpinającego (MST).
class DisjointSet:
    def __init__(self, vertices):
        # parent przechowuje rodzica każdego wierzchołka, początkowo każdy wierzchołek jest swoim własnym rodzicem.
        self.parent = {v: v for v in vertices}
        # rank przechowuje głębokość drzewa dla optymalizacji połączeń.
        self.rank = {v: 0 for v in vertices}

    # Metoda find zwraca korzeń zbioru, do którego należy dany element.
    # Implementuje "ścieżkę kompresji" (path compression), co przyspiesza przyszłe operacje, ustawiając każdego przodka bezpośrednio jako dziecko korzenia.
    def find(self, item):
        if self.parent[item] != item:
            self.parent[item] = self.find(self.parent[item])
        return self.parent[item]

    # Metoda union łączy dwa zbiory.
    # Używa "połączenia według rangi" (union by rank), co utrzymuje drzewo płaskim.
    # Zbiór z mniejszą rangą staje się podzbiorem zbioru z większą rangą. Jeśli rangi są równe, zwiększamy rangę korzenia nowego zbioru.
    def union(self, set1, set2):
        root1 = self.find(set1)
        root2 = self.find(set2)
        if root1 != root2:
            if self.rank[root1] > self.rank[root2]:
                self.parent[root2] = root1
            else:
                self.parent[root1] = root2
                if self.rank[root1] == self.rank[root2]:
                    self.rank[root2] += 1
# Klasa DisjointSet jest kluczowym komponentem wielu algorytmów grafowych, w tym algorytmu Kruskala. Dzięki metodom find z kompresją ścieżek i union z łączeniem według rangi, klasa ta umożliwia efektywne zarządzanie zbiorami rozłącznymi, co jest niezbędne do sprawdzania połączeń i zapobiegania cyklom w grafie.


# Funkcja rysująca graf
def redraw_graph(vertices, edges, mst_edges):
    WIN.fill(GREY)
    
    for edge in edges:
        u, v = edge.u, edge.v
        color = BLUE if edge in mst_edges else BLACK
        pygame.draw.line(WIN, color, vertices[u], vertices[v], 2)
    
    for v in vertices:
        pygame.draw.circle(WIN, RED, vertices[v], 5)  

# Algorytm Kruskala
def kruskal(vertices, edges):
    mst = []
    edges.sort(key=lambda edge: edge.weight)
    disjoint_set = DisjointSet(vertices)

    for edge in edges:
        if disjoint_set.find(edge.u) != disjoint_set.find(edge.v):
            disjoint_set.union(edge.u, edge.v)
            mst.append(edge)
            WIN.fill(GREY)  # Użycie koloru CYAN
            redraw_graph(vertices, edges, mst)
            draw_menu()
            pygame.display.flip()  # Użycie flip zamiast update
            pygame.time.wait(100)  # Opóźnienie dla animacji
    
    return mst

# Funkcja rysująca menu
def draw_menu():
    font = pygame.font.Font(None, 30)
    instructions = [
        "Menu::",
        "1. Kliknij lewym przyciskiem myszy, aby dodac wierzchołek.",
        "2. Kliknij ESC, aby wyjść."
    ]
    for i, instruction in enumerate(instructions):
        text_surface = font.render(instruction, True, BLUE)
        WIN.blit(text_surface, (10, 10 + i * 30))

# Funkcja wyświetlająca komunikat o pracy algorytmu
def draw_working_message():
    font = pygame.font.Font(None, 36)
    message = "Pracuję..."
    text_surface = font.render(message, True, BLACK)
    WIN.blit(text_surface, (WIDTH // 2 - text_surface.get_width() // 2, HEIGHT // 2 - text_surface.get_height() // 2))
    pygame.display.flip()

def main():
    running = True
    clock = pygame.time.Clock()

    vertices = {}
    edges = []
    mst_edges = []

    # Przykładowe wierzchołki i krawędzie
    # vertices = {i: (random.randint(50, WIDTH - 50), random.randint(50, HEIGHT - 50)) for i in range(10)}
    # edges = []
    
    # for i in vertices:
    #     for j in vertices:
    #         if i != j:
    #             weight = math.sqrt((vertices[i][0] - vertices[j][0]) ** 2 + (vertices[i][1] - vertices[j][1]) ** 2)
    #             edges.append(Edge(i, j, weight))

    # mst_edges = kruskal(vertices, edges)

    while running:
        clock.tick(60)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE: # obsługa klawisza ESC, aby umożliwić wyjście z programu.
                    running = False
                    # Obsługa zdarzenia pygame.MOUSEBUTTONDOWN, które wykrywa kliknięcie lewego przycisku myszy.
            elif event.type == pygame.MOUSEBUTTONDOWN:
                # Użytkownik może teraz dodawać wierzchołki poprzez kliknięcie myszą, a graf oraz minimalne drzewo rozpinające będą aktualizowane i wyświetlane na bieżąco.
                if event.button == 1:  # Lewy przycisk myszy
                    pos = event.pos
                    vertex_id = len(vertices)
                    # Dodawanie nowego wierzchołka do słownika vertices z jego pozycją kliknięcia.
                    vertices[vertex_id] = pos

                    # Dodanie nowych krawędzi do grafu
                    for i in vertices:
                        if i != vertex_id:
                            weight = math.sqrt((vertices[i][0] - pos[0]) ** 2 + (vertices[i][1] - pos[1]) ** 2)
                            edges.append(Edge(i, vertex_id, weight))

                    # # Wyświetlenie komunikatu "Pracuję..."
                    # WIN.fill(WHITE)
                    # draw_graph(vertices, edges, mst_edges)
                    # draw_menu()
                    # draw_working_message()

                    # Po uruchomieniu programu użytkownik może dodawać wierzchołki, klikając myszą. Gdy dodawany jest nowy wierzchołek i algorytm Kruskala przelicza minimalne drzewo rozpinające (MST), tytuł okna zmienia się na "Pracuję...". Po zakończeniu obliczeń tytuł okna wraca do "Algorytm Kruskala".

                    # Zmiana napisu w pasku tytułu na "Pracuję..."
                    pygame.display.set_caption("Algorytm Kruskala - Pracuję...")

                    # Przeliczenie MST
                    # Po dodaniu nowego wierzchołka i nowych krawędzi, wywoływany jest algorytm Kruskala, aby przeliczyć minimalne drzewo rozpinające.
                    mst_edges = kruskal(vertices, edges)

                    # Przywrócenie napisu w pasku tytułu
                    pygame.display.set_caption("Algorytm Kruskala")

        WIN.fill(GREY)
        redraw_graph(vertices, edges, mst_edges)
        # menu wyświetlane na ekranie
        draw_menu()
        pygame.display.flip()

    pygame.quit()

if __name__ == "__main__":
    main()


"""

Algorytm Kruskala to algorytm wykorzystywany w teorii grafów do znajdowania minimalnego drzewa rozpinającego (Minimum Spanning Tree, MST) dla grafu nieskierowanego. Minimalne drzewo rozpinające to podzbiór krawędzi grafu, który łączy wszystkie wierzchołki grafu, a suma wag tych krawędzi jest minimalna.


Kroki Algorytmu Kruskala

1. Inicjalizacja:

- Utwórz zbiór wszystkich krawędzi grafu.
- Każdy wierzchołek grafu traktuj jako osobny zbiór.


2. Sortowanie:

- Posortuj wszystkie krawędzie w grafie w porządku rosnącym wag krawędzi.

3. Łączenie krawędzi:

- Przechodź przez posortowane krawędzie i dodawaj każdą krawędź do drzewa rozpinającego, jeśli nie tworzy ona cyklu (można to sprawdzić za pomocą struktury Union-Find/Disjoint Set).
- Kontynuuj, aż drzewo rozpinające będzie miało V−1 krawędzi (gdzie V to liczba wierzchołków).

Struktura Union-Find jest używana do efektywnego zarządzania zbiorem elementów podzielonych na rozłączne podzbiory. Obsługuje ona operacje:

- Find: Zwraca przedstawiciela (korzeń) zbioru zawierającego dany element.
- Union: Łączy dwa rozłączne zbiory w jeden.

Aby uniknąć tworzenia cykli w algorytmie Kruskala, używa się tej struktury do sprawdzania, czy dwa wierzchołki danej krawędzi należą już do tego samego zbioru (co oznaczałoby, że dodanie tej krawędzi utworzy cykl).

Zastosowania

Algorytm Kruskala jest używany w wielu zastosowaniach, takich jak:

- Projektowanie sieci (np. sieci komputerowe, sieci drogowe).
- Minimalizacja kosztów połączeń.
- Optymalizacja dróg i innych infrastruktur.


Algorytm Kruskala jest jednym z dwóch popularnych algorytmów do znajdowania minimalnego drzewa rozpinającego, obok algorytmu Prima. Wybór algorytmu zależy od specyfiki problemu i struktury danych wejściowych.

"""