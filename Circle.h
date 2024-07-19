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

    Circle()
        : x(0), y(0), vx(0), vy(0), radius(0), color(olc::WHITE), active(false) {}


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

    int colorSum() const {
        return color.r + color.g + color.b;
    }

    // bool operator<(const Circle& other) const {
    //     std::cout << colorSum() << ", " << other.colorSum() << std::endl;
    //     return colorSum() < other.colorSum(); // porównanie na podstawie sumy składowych koloru
    // }

    // bool operator>(const Circle& other) const {
    //     std::cout << colorSum() << ", " << other.colorSum() << std::endl;
    //     return colorSum() > other.colorSum(); // porównanie na podstawie sumy składowych koloru
    // }

    bool operator<(const Circle& other) const {
        std::cout << radius << ", " << other.radius << std::endl;
        return radius < other.radius; // porównanie na podstawie promienia
    }

    bool operator>(const Circle& other) const {
        std::cout << radius << ", " << other.radius << std::endl;
        return radius > other.radius; // porównanie na podstawie promienia
    }

    // bool operator<=(const Circle& other) const {
    //     return radius <= other.radius;
    // }

    // bool operator>=(const Circle& other) const {
    //     return radius >= other.radius;
    // }


};

#endif // CIRCLE_H
