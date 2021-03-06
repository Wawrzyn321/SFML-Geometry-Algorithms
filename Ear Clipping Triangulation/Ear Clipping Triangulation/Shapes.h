#ifndef SHAPES_H
#define SHAPES_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
class point {
    sf::CircleShape circle;
public:
    float x, y;
    point(float ix, float iy);
    sf::Vector2f getPosition();
    void draw(sf::RenderWindow &wind);
};

class line {
    sf::RectangleShape rectangle;
    float x, y;
public:
    line(float x1, float y1, float x2, float y2);
    line::line(point p1, point p2);
    void draw(sf::RenderWindow &wind);
};

class triangle {
    sf::ConvexShape tri;
public:
    triangle(float x1, float y1, float x2, float y2, float x3, float y3);
    triangle(point p1, point p2, point p3);
    void setFillColor(sf::Color color);
    void draw(sf::RenderWindow &wind);
};


#endif
