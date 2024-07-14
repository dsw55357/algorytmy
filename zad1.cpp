/*

Zaprojektuj i zapisz w wybrany sposób algorytm znajdowania otoczki wypukłej dla
skończonego zbioru punktów na płaszczyźnie, gdzie znane są współrzędne kartezjańskie
punktów.

Otoczka wypukła (convex hull) zbioru punktów $ P $ to najmniejszy wielokąt wypukły taki, że każdy punkt ze zbioru $ P $ leży albo na brzegu wielokąta albo w jego wnętrzu. W skrócie będziemy zapisywali: CH(P). źródło: http://informatyka.wroc.pl/node/910?page=0,0

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

    void addRandomPoints(int n) {
        for (int i = 0; i < n; ++i) {
            int x = rand() % ScreenWidth();
            int y = rand() % ScreenHeight();
            points.push_back({x, y});
        }
        computed = false; // Konieczność ponownego obliczenia otoczki
    }

public:
    bool OnUserCreate() override {
        // Punkty do wizualizacji
        points = {{100, 150}, {200, 250}, {300, 200}, {400, 300}, {250, 400}, {150, 350}, {350, 100}, {450, 150}};
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLACK);

        // Obsługa klawisza ESC do zamknięcia programu
        bool quit =  false;
		if (GetKey(olc::Key::ESCAPE).bReleased)
			quit = true;

        // Obsługa klawisza R do dodania losowych punktów
        if (GetKey(olc::R).bPressed) {
            addRandomPoints(5); // Dodaj 5 losowych punktów
        }

        // Rysuj punkty
        for (const auto& point : points) {
            Draw(point.x, point.y, olc::WHITE);
        }

        // Rysuj punkty
        for (const auto& point : points) {
            FillCircle(point.x, point.y, 2, olc::WHITE);
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

        show_menu();

        return !quit;
    }

	void show_menu() {

        DrawString(8, ScreenHeight()-30, "R - add new random points", olc::GREEN);
        DrawString(8, ScreenHeight()-15, "ESC - exit", olc::GREEN);
		DrawString(15, 15, "Znajdowania otoczki wypuklej", olc::WHITE );
		DrawString(ScreenWidth()-300, ScreenHeight()-15, "Powered by olcPixelGameEngine, 2024(7)", olc::CYAN );
	}    
};

int main() {

    std::cout << "Otoczka wypukła składa się z punktów: \n";

    ConvexHullVisualizer demo;
    if (demo.Construct(600, 600, 1, 1))
        demo.Start();

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
- addRandomPoints: Dodana funkcja, która generuje n losowych punktów w obrębie ekranu i dodaje je do wektora points. Po dodaniu nowych punktów ustawia computed na false, aby wymusić ponowne obliczenie otoczki wypukłej.
- Obsługa klawisza R: W funkcji OnUserUpdate dodano obsługę klawisza R, który wywołuje funkcję addRandomPoints z argumentem 5, co powoduje dodanie pięciu losowych punktów za każdym razem, gdy klawisz R jest naciśnięty.
- Rysowanie punktów: Użyto FillCircle do rysowania punktów, aby były one lepiej widoczne na ekranie.

Teraz aplikacja nie tylko pozwala na wizualizację otoczki wypukłej, ale także umożliwia dynamiczne dodawanie nowych punktów w losowy sposób za pomocą klawisza R oraz zamknięcie programu za pomocą klawisza ESC.


### Compiling in Linux
~~~~~~~~~~~~~~~~~~
You will need a modern C++ compiler, so update yours!
To compile use the command:

g++ -o start zad1.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17


*/