#pragma once
#include "Painter.hpp"
#include "Point.hpp"
#include "Velocity.hpp"

#include <fstream>

class Ball {
    double radius = 0.0;
    bool isCollidable = true;
    Color color = Color(0, 0, 0);
    Velocity velocity = Velocity();
    Point center = Point();
public:
    Ball() = default;
    Ball(const double &_radius, const Color &_color, const Velocity &_velocity, const Point &_center, const bool &_isCollidable):
        radius{_radius}, color {_color}, velocity(_velocity), center{_center}, isCollidable{_isCollidable} {};

    void setRadius(const double& radius);
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