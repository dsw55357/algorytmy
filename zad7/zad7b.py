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

# Definicje kolorów
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)

# Ustawienia Pygame
WIDTH, HEIGHT = 800, 600
FPS = 60

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
    screen.fill(WHITE)
    
    # Rysowanie krawędzi
    for (u, v), weight in edges.items():
        color = BLUE if (u, v) in mst_edges or (v, u) in mst_edges else BLACK
        pygame.draw.line(screen, color, vertices[u], vertices[v], 2)
        mid_point = ((vertices[u][0] + vertices[v][0]) // 2, (vertices[u][1] + vertices[v][1]) // 2)
        text = font.render(str(weight), True, BLACK)
        screen.blit(text, mid_point)
    
    # Rysowanie wierzchołków
    for vertex, pos in vertices.items():
        pygame.draw.circle(screen, RED, pos, 10)
        text = font.render(vertex, True, BLACK)
        screen.blit(text, (pos[0] - 10, pos[1] - 30))

def main():
    running = True
    mst_edges = set()

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # Rysowanie grafu
        draw_graph(screen, vertices, edges, mst_edges)
        
        pygame.display.flip()
        clock.tick(FPS)

    pygame.quit()

if __name__ == "__main__":
    main()
