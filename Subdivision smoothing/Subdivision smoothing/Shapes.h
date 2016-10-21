#ifndef SHAPES_H
#define SHAPES_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
class point {
public:
    sf::CircleShape circle;
    float x, y;
    point::point(sf::Vector2f vec);
    point(float ix, float iy);
    sf::Vector2f getPosition();
    void draw(sf::RenderWindow &wind);
};

class line {
public:
    sf::RectangleShape rectangle;
    float x, y;
    line(float x1, float y1, float x2, float y2);
    line::line(point p1, point p2);
    void draw(sf::RenderWindow &wind);
};

class triangle {
public:
    sf::ConvexShape tri;
    triangle(float x1, float y1, float x2, float y2, float x3, float y3);
    triangle(point p1, point p2, point p3);
    void draw(sf::RenderWindow &wind);
};


#endif
