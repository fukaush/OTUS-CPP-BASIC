#pragma once
#include <cmath>
#include <fstream>
#include "Painter.hpp"
#include "Point.hpp"
#include "Velocity.hpp"

constexpr double factor = 4.0 / 3.0 * M_PI;

class Ball {
    double radius = 0.0;
    bool isCollidable = true;
    Color color = Color(0, 0, 0);
    Velocity velocity = Velocity();
    Point center = Point();
    double mass;
public:
    Ball() = default;
    Ball(const double _radius, const Color _color, const Velocity _velocity, const Point _center, const bool _isCollidable):
        radius{_radius}, color {_color}, velocity(_velocity), center{_center}, isCollidable{_isCollidable} {
        mass = calculateMass(radius);
    };

    static double calculateMass(double radius) {
        return factor * pow(radius, 3);
    }

    void setRadius(const double radius);
    void setCenter(const Point& center);
    void setVelocity(const Velocity& velocity);

    Velocity getVelocity() const;
    Point getCenter() const;
    double getRadius() const;
    double getMass() const;
    bool collidable() const {
        return isCollidable;
    }
    void draw(Painter& painter) const;
};