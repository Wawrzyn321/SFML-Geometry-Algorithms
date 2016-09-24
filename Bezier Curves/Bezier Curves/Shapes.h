#ifndef SHAPES_H
#define SHAPES_H
#include "Geometry.h"
#pragma warning(disable: 4244)

class point{
    sf::Color niceBlue = sf::Color(100, 100, 255);
    sf::Vector2f pos;
public:
    sf::CircleShape circle;
    point();
    point(int ix, int iy);
    point(sf::Vector2f _pos);
    void setPosition(sf::Vector2f _newPos);
    sf::Vector2f getPosition();
    void draw(sf::RenderWindow &wind);
};

class line{
    sf::Color niceBlue = sf::Color(100, 100, 255);
public:
    sf::RectangleShape rectangle;
    int x, y;
    sf::Vector2f start, end;
    line(){}
    line(sf::Vector2f p1, sf::Vector2f p2);
    void setPoints(sf::Vector2f p1, sf::Vector2f p2);
    void draw(sf::RenderWindow &wind);
};

#endif
