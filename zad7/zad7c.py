"""

Algorytm Prime’a (znany również jako algorytm Prima) jest klasycznym algorytmem do znajdowania minimalnego drzewa rozpinającego (MST) w grafie. Minimalne drzewo rozpinające to podgraf, który zawiera wszystkie wierzchołki grafu wejściowego, jest drzewem (czyli jest spójny i acykliczny) oraz ma najmniejszą sumę wag krawędzi w porównaniu z innymi możliwymi drzewami rozpinającymi tego grafu.

Opis algorytmu

1. Inicjalizacja:

- Wybierz dowolny wierzchołek początkowy i dodaj go do zbioru wierzchołków należących do MST.
- Utwórz zbiór krawędzi należących do MST, początkowo pusty.

2. Wybór krawędzi:

- Wybierz najtańszą krawędź, która łączy wierzchołek w MST z wierzchołkiem spoza MST.
- Dodaj tę krawędź do MST i dołącz odpowiedni wierzchołek do zbioru wierzchołków MST.

3. Powtarzanie:

- Powtarzaj krok 2, aż wszystkie wierzchołki znajdą się w MST.

4. Zakończenie:

- Zwróć zbiór krawędzi tworzących MST.


 Implementacja algorytmu Prime'a w Pythonie i wizualizacja przy użyciu Pygame

"""

import pygame
import heapq
import random
import math


# Definicje kolorów
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
GREY = (192, 192, 192)


# Ustawienia Pygame
WIDTH, HEIGHT = 1024, 720
FPS = 60

vertices = {}
edges = {}

# Definicja wierzchołków i krawędzi
vertices = {
    'A': (100, 100),
    'B': (300, 100),
    'C': (500, 100),
    'D': (200, 300),
    'E': (400, 300),
    'F': (300, 500)
}

edges = {
    ('A', 'B'): 2,
    ('A', 'D'): 3,
    ('B', 'C'): 4,
    ('B', 'D'): 1,
    ('C', 'E'): 5,
    ('D', 'E'): 6,
    ('D', 'F'): 7,
    ('E', 'F'): 8
}

# Inicjalizacja Pygame
pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Algorytm Prime'a - MST")
clock = pygame.time.Clock()
font = pygame.font.SysFont('Arial', 20)

def draw_graph(screen, vertices, edges, mst_edges):
    screen.fill(GREY)
    
    # Rysowanie krawędzi    
    for (u, v), weight in edges.items():
        if (u, v) in mst_edges or (v, u) in mst_edges:
            weight_color = RED
        else:
            weight_color = BLACK

        color = BLUE if (u, v) in mst_edges or (v, u) in mst_edges else BLACK
        pygame.draw.line(screen, color, vertices[u], vertices[v], 2)
        mid_point = ((vertices[u][0] + vertices[v][0]) // 2, (vertices[u][1] + vertices[v][1]) // 2)
        text = font.render(str(weight), True, weight_color)
        screen.blit(text, mid_point)
    
    # Rysowanie wierzchołków
    for vertex, pos in vertices.items():
        pygame.draw.circle(screen, RED, pos, 10)
        text = font.render(vertex, True, BLACK)
        screen.blit(text, (pos[0] - 10, pos[1] - 30))

def prim_algorithm(vertices, edges):
    if not vertices:
        return set()

    start_vertex = next(iter(vertices))  # Wybieramy dowolny wierzchołek jako startowy
    visited = set([start_vertex])
    edges_to_process = []

    for (u, v), weight in edges.items():
        if u == start_vertex or v == start_vertex:
            heapq.heappush(edges_to_process, (weight, u, v))

    mst_edges = set()

    while edges_to_process:
        weight, u, v = heapq.heappop(edges_to_process)
        if v not in visited:
            visited.add(v)
            mst_edges.add((u, v))
            for (u_next, v_next), weight in edges.items():
                if u_next == v or v_next == v:
                    if u_next not in visited or v_next not in visited:
                        # heapq.heappush jest używane do dodawania krawędzi do kopca edges_to_process, który jest używany do wyboru krawędzi o najmniejszej wadze w każdej iteracji algorytmu.
                        heapq.heappush(edges_to_process, (weight, u_next, v_next))
                        # heapq.heappush jest funkcją w module heapq w Pythonie, która jest używana do dodawania elementu do kopca (ang. heap). Kopce są używane do implementacji kolejek priorytetowych. W Pythonie kopiec jest realizowany jako kopiec minimalny, co oznacza, że najmniejszy element znajduje się na szczycie kopca.

    return mst_edges

def add_vertex(pos):
    vertex = chr(ord('A') + len(vertices))
    vertices[vertex] = pos
    # Przerysowanie grafu po dodaniu nowego wierzchołka.
    redraw_graph()

def add_edge(u, v):
    weight = random.randint(1, 10)  # Losowa waga z przedziału 1-10
    edges[(u, v)] = weight
    edges[(v, u)] = weight  # Graf nieskierowany

# Aktualizowanie pozycji wierzchołków w sposób uporządkowany za każdym razem, gdy dodawany jest nowy wierzchołek. Graf zostanie przerysowany w uporządkowany sposób, a wierzchołki będą rozmieszczane równomiernie na okręgu w centrum okna.
def redraw_graph2():
    n = len(vertices)
    if n == 0:
        return

    radius = min(WIDTH, HEIGHT) // 3
    angle_gap = 2 * math.pi / n
    center_x, center_y = WIDTH // 2, HEIGHT // 2

    for i, vertex in enumerate(vertices):
        angle = i * angle_gap
        x = int(center_x + radius * math.cos(angle))
        y = int(center_y + radius * math.sin(angle))
        vertices[vertex] = (x, y)

def redraw_graph():
    n = len(vertices)
    if n == 0:
        return

    side = int(math.ceil(math.sqrt(n)))
    gap_x = WIDTH // (side + 1)
    gap_y = HEIGHT // (side + 1)

    for i, vertex in enumerate(vertices):
        row = i // side
        col = i % side
        x = (col + 1) * gap_x
        y = (row + 1) * gap_y
        vertices[vertex] = (x, y)


# Funkcja rysująca menu
def draw_menu():
    font = pygame.font.Font(None, 30)
    instructions = [
        "Menu::",
        "1. Kliknij lewym przyciskiem myszy, aby dodac wierzchołek.",
        "2. Kliknij prawym przyciskiem myszy, aby dodac krawedz.",
        "3. Kliknij ESC, aby wyjsc."
    ]
    for i, instruction in enumerate(instructions):
        text_surface = font.render(instruction, True, BLUE)
        screen.blit(text_surface, (10, 10 + i * 30))


def main():
    running = True
    mst_edges = set()

    redraw_graph()

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE: # obsługa klawisza ESC, aby umożliwić wyjście z programu.
                    running = False
            elif event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 1:  # Lewy przycisk myszy
                    # Dodanie nowego wierzchołka w miejscu kliknięcia.
                    pos = pygame.mouse.get_pos()
                    add_vertex(pos)
                elif event.button == 3:  # Prawy przycisk myszy
                    pos = pygame.mouse.get_pos()
                    for vertex, vertex_pos in vertices.items():
                        if (pos[0] - vertex_pos[0]) ** 2 + (pos[1] - vertex_pos[1]) ** 2 <= 100:
                            if 'start' not in globals():
                                global start
                                start = vertex
                            else:
                                # Funkcja add_edge(u, v, weight) dodaje krawędź o zadanej wadze między wierzchołkami u i v między dwoma klikniętymi wierzchołkami.
                                add_edge(start, vertex)  # Dodaj krawędź o losowej wadze
                                del globals()['start']

        # Rysowanie grafu
        mst_edges = prim_algorithm(vertices, edges)
        draw_graph(screen, vertices, edges, mst_edges)
        draw_menu()
        pygame.display.flip()
        clock.tick(FPS)

    pygame.quit()

if __name__ == "__main__":
    main()

"""

Cel Projektu
Celem projektu było stworzenie wizualizacji algorytmu Prima (Prim's algorithm) do znajdowania minimalnego drzewa rozpinającego (MST) w grafie. Wizualizacja została zrealizowana przy użyciu biblioteki Pygame. Użytkownik może interaktywnie dodawać wierzchołki i krawędzie oraz obserwować działanie algorytmu Prima w czasie rzeczywistym.


Główne Funkcje

1. Rysowanie Grafu

- Rysowanie wierzchołków i krawędzi grafu.
- Kolorowanie krawędzi będących częścią MST na niebiesko, a ich wag na czerwono.
- Aktualizacja pozycji wierzchołków po każdym dodaniu nowego wierzchołka.

2. Interaktywne Dodawanie Wierzchołków i Krawędzi

- Lewy przycisk myszy dodaje nowy wierzchołek.
- Prawy przycisk myszy dodaje krawędź między wybranymi wierzchołkami z losową wagą z przedziału 1-10.

3. Algorytm Prima

- Implementacja algorytmu Prima do znajdowania MST w grafie.
- Wizualizacja MST poprzez wyróżnienie odpowiednich krawędzi i wag.

4. Menu z Instrukcjami

- Wyświetlanie instrukcji obsługi w lewym górnym rogu ekranu.
- Instrukcje informujące, jak dodawać wierzchołki, krawędzie oraz jak zamknąć aplikację.

Struktura Kodowa

1. Inicjalizacja Pygame

- Ustawienia ekranu, kolory, częstotliwość odświeżania ekranu.

2. Definicje Struktur Danych

- vertices: słownik przechowujący pozycje wierzchołków.
- edges: słownik przechowujący wagi krawędzi między wierzchołkami.
- mst_edges: zbiór przechowujący krawędzie będące częścią MST.

3. Funkcje Pomocnicze

- draw_graph: Rysuje graf na ekranie.
- add_vertex: Dodaje nowy wierzchołek do grafu.
- add_edge: Dodaje nową krawędź z losową wagą.
- redraw_graph: Aktualizuje pozycje wierzchołków, aby były rozłożone równomiernie w okręgu.
- prim_algorithm: Implementacja algorytmu Prima do znajdowania MST.
- draw_menu: Rysuje menu z instrukcjami.

4. Pętla Główna Programu

- Obsługa zdarzeń (kliknięcia myszy, klawisz ESC).
- Aktualizacja i rysowanie grafu oraz menu.

"""