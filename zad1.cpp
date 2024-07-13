/*

Zaprojektuj i zapisz w wybrany sposób algorytm znajdowania otoczki wypukłej dla
skończonego zbioru punktów na płaszczyźnie, gdzie znane są współrzędne kartezjańskie
punktów.

Aby znaleźć otoczkę wypukłą dla skończonego zbioru punktów na płaszczyźnie, możemy użyć algorytmu Grahama, który jest jednym z najpopularniejszych algorytmów do tego celu. Algorytm Grahama sortuje punkty według kąta, który tworzą z punktem o najmniejszej współrzędnej y, a następnie konstruuje otoczkę wypukłą poprzez iteracyjne dodawanie punktów i sprawdzanie, czy tworzą one zakręt w lewo lub w prawo.

*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <cmath>

struct Point {
    int x, y;
};

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

int main() {
    std::vector<Point> points = {{0, 3}, {1, 1}, {2, 2}, {4, 4}, {0, 0}, {1, 2}, {3, 1}, {3, 3}};
    
    std::vector<Point> hull = convexHull(points);

    std::cout << "Otoczka wypukła składa się z punktów: \n";
    for (const auto& point : hull) {
        std::cout << "(" << point.x << ", " << point.y << ")\n";
    }

    return 0;
}


/*

Wyjaśnienie:
Struktura Point: Definiuje punkt na płaszczyźnie z współrzędnymi x i y.
findLowestPoint: Znajduje punkt o najmniejszej współrzędnej y. Jeśli jest kilka takich punktów, wybiera ten o najmniejszej współrzędnej x.
orientation: Oblicza orientację trzech punktów, aby sprawdzić, czy tworzą one zakręt w prawo, w lewo czy są kolinearne.
compare: Sortuje punkty według kąta, który tworzą z punktem bazowym.
convexHull: Implementuje algorytm Grahama do znajdowania otoczki wypukłej.
main: Testuje algorytm na przykładowym zbiorze punktów i wyświetla wynikową otoczkę wypukłą.

*/