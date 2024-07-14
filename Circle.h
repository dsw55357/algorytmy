#ifndef CIRCLE_H
#define CIRCLE_H

#include <cmath>
#include "olcPixelGameEngine.h"

class Circle {
public:
    float x, y;
    float vx, vy;
    float radius;
    olc::Pixel color;
    bool active;

    Circle(float x, float y, float vx, float vy, float radius, olc::Pixel color)
        : x(x), y(y), vx(vx), vy(vy), radius(radius), color(color), active(true) {}

    void update(float fElapsedTime) {
        x += vx * fElapsedTime;
        y += vy * fElapsedTime;

        // Zderzenia z krawędziami ekranu
        if (x < 0 || x >= 600) vx *= -1;
        if (y < 0 || y >= 600) vy *= -1;
    }

    bool isColliding(const Circle& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float distance = sqrt(dx * dx + dy * dy);
        return distance < (radius + other.radius);
    }
};

#endif // CIRCLE_H
