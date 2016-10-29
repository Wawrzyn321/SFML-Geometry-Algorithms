#ifndef SHAPES_H
#define SHAPES_H


#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
class point {
    sf::CircleShape circle;
public:
    float x, y;
    point() {}
    point(float ix, float iy);
    sf::Vector2f getPosition();
    void draw(sf::RenderWindow &wind);
    inline bool operator!=(const point &p);
    inline bool operator==(const point &p);
};

class line {
    float x, y;
public:
    sf::RectangleShape rectangle;
    line() { }
    line(float x1, float y1, float x2, float y2);
    line(point p1, point p2);
    line(sf::Vector2f v1, sf::Vector2f v2);
    void draw(sf::RenderWindow &wind);
};

#endif
