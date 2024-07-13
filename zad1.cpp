/*

Zaprojektuj i zapisz w wybrany sposób algorytm znajdowania otoczki wypukłej dla
skończonego zbioru punktów na płaszczyźnie, gdzie znane są współrzędne kartezjańskie
punktów.

Aby znaleźć otoczkę wypukłą dla skończonego zbioru punktów na płaszczyźnie, możemy użyć algorytmu Grahama, który jest jednym z najpopularniejszych algorytmów do tego celu. Algorytm Grahama sortuje punkty według kąta, który tworzą z punktem o najmniejszej współrzędnej y, a następnie konstruuje otoczkę wypukłą poprzez iteracyjne dodawanie punktów i sprawdzanie, czy tworzą one zakręt w lewo lub w prawo.

*/

// skorzystam z olcPixelGameEngine w celu wizualizacji działania algorytmu
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <cmath>

struct Point {
    int x, y;
};


class ConvexHullVisualizer : public olc::PixelGameEngine {
public:
    ConvexHullVisualizer() {
        sAppName = "Convex Hull Visualization";
    }

private:
    std::vector<Point> points;
    std::vector<Point> hull;
    bool computed = false;

    // Funkcja do znalezienia punktu o najmniejszej współrzędnej y (jeśli jest kilka, to najmniejszej współrzędnej x)
    Point findLowestPoint(std::vector<Point>& points) {
        Point lowest = points[0];
        for (const auto& point : points) {
            if (point.y < lowest.y || (point.y == lowest.y && point.x < lowest.x)) {
                lowest = point;
            }
        }
        return lowest;
    }

    // Funkcja do obliczania orientacji trzech punktów
    // 0 -> punkty są kolinearne
    // 1 -> zakręt w prawo
    // 2 -> zakręt w lewo
    int orientation(const Point& p, const Point& q, const Point& r) {
        int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
        if (val == 0) return 0; // kolinearne
        return (val > 0) ? 1 : 2; // prawy lub lewy zakręt
    }

    // Funkcja do sortowania punktów według kąta z punktem bazowym
    bool compare(const Point& p1, const Point& p2, const Point& base) {
        int o = orientation(base, p1, p2);
        if (o == 0) // kolinearne
            return (std::hypot(base.x - p1.x, base.y - p1.y) < std::hypot(base.x - p2.x, base.y - p2.y));
        return (o == 2); // lewy zakręt
    }

    // Funkcja do znajdowania otoczki wypukłej za pomocą algorytmu Grahama
    std::vector<Point> convexHull(std::vector<Point>& points) {
        if (points.size() < 3) return {};

        Point base = findLowestPoint(points);
        std::sort(points.begin(), points.end(), [&](const Point& p1, const Point& p2) {
            return compare(p1, p2, base);
        });

        std::stack<Point> hull;
        hull.push(points[0]);
        hull.push(points[1]);

        for (size_t i = 2; i < points.size(); ++i) {
            while (hull.size() > 1) {
                Point top = hull.top();
                hull.pop();
                if (orientation(hull.top(), top, points[i]) != 1) { // dopóki jest lewy zakręt
                    hull.push(top);
                    break;
                }
            }
            hull.push(points[i]);
        }

        std::vector<Point> result;
        while (!hull.empty()) {
            result.push_back(hull.top());
            hull.pop();
        }

        return result;
    }
public:
    bool OnUserCreate() override {
        // Punkty do wizualizacji
        points = {{100, 150}, {200, 250}, {300, 200}, {400, 300}, {250, 400}, {150, 350}, {350, 100}, {450, 150}};
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLACK);

        bool quit =  false;
		if (GetKey(olc::Key::ESCAPE).bReleased)
			quit = true;

        // Rysuj punkty
        for (const auto& point : points) {
            Draw(point.x, point.y, olc::WHITE);
        }

        // Oblicz otoczkę wypukłą
        if (!computed) {
            hull = convexHull(points);
            computed = true;
        }

        // Rysuj otoczkę wypukłą
        for (size_t i = 0; i < hull.size(); ++i) {
            Point p1 = hull[i];
            Point p2 = hull[(i + 1) % hull.size()];
            DrawLine(p1.x, p1.y, p2.x, p2.y, olc::RED);
        }

        return !quit;
    }
};

int main() {

    // std::vector<Point> points = {{0, 3}, {1, 1}, {2, 2}, {4, 4}, {0, 0}, {1, 2}, {3, 1}, {3, 3}};
    // std::vector<Point> hull = convexHull(points);

    std::cout << "Otoczka wypukła składa się z punktów: \n";

    ConvexHullVisualizer demo;
    if (demo.Construct(600, 600, 1, 1))
        demo.Start();

/*
    for (const auto& point : hull) {
        std::cout << "(" << point.x << ", " << point.y << ")\n";
    }
*/

    return 0;
}


/*

Wyjaśnienie:

- Struktura Point: Definiuje punkt na płaszczyźnie z współrzędnymi x i y.
- ConvexHullVisualizer: Klasa dziedzicząca po olc::PixelGameEngine do wizualizacji otoczki wypukłej.
- findLowestPoint: Znajduje punkt o najmniejszej współrzędnej y. Jeśli jest kilka takich punktów, wybiera ten o najmniejszej współrzędnej x.
- orientation: Oblicza orientację trzech punktów, aby sprawdzić, czy tworzą one zakręt w prawo, w lewo czy są kolinearne.
- compare: Sortuje punkty według kąta, który tworzą z punktem bazowym.
- convexHull: Implementuje algorytm Grahama do znajdowania otoczki wypukłej.
- OnUserCreate: Funkcja inicjalizująca, która dodaje punkty do wizualizacji.
- OnUserUpdate: Funkcja aktualizująca, która rysuje punkty i otoczkę wypukłą na ekranie.


### Compiling in Linux
~~~~~~~~~~~~~~~~~~
You will need a modern C++ compiler, so update yours!
To compile use the command:

g++ -o start zad1.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17


*/