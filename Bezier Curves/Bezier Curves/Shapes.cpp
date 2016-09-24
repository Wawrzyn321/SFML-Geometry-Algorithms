#include "Shapes.h"

//point

point::point(){
    circle = sf::CircleShape(4, 10);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setFillColor(niceBlue);
}

point::point(int ix, int iy){
    pos = sf::Vector2f(ix, iy);
    circle = sf::CircleShape(4, 10);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setPosition(pos);
    circle.setFillColor(niceBlue);
}

point::point(sf::Vector2f _pos){
    pos = _pos;
    circle = sf::CircleShape(4, 10);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setPosition(_pos);
    circle.setFillColor(niceBlue);
}

void point::setPosition(sf::Vector2f _newPos){
    pos = _newPos;
    circle.setPosition(pos);
}

sf::Vector2f point::getPosition(){
    return pos;
}

void point::draw(sf::RenderWindow &wind){
    wind.draw(circle);
}


//line

line::line(sf::Vector2f p1, sf::Vector2f p2){
    start = p1;
    end = p2;
    x = (int)(p1.x + p2.x) / 2;
    y = (int)(p1.y + p2.y) / 2;
    double l = distance(p1.x, p1.y, p2.x, p2.y);
    rectangle = sf::RectangleShape(sf::Vector2f(3, l));
    rectangle.setOrigin(rectangle.getSize().x / 2, rectangle.getSize().y / 2);
    double roto = -atan((double)(p1.x - p2.x) / (p1.y - p2.y));
    rectangle.setRotation(0);
    rectangle.rotate(roto * 180 / pi);
    rectangle.setPosition(x, y);
    rectangle.setFillColor(niceBlue);
}

void line::setPoints(sf::Vector2f p1, sf::Vector2f p2){
    start = p1;
    end = p2;
    x = (int)(p1.x + p2.x) / 2;
    y = (int)(p1.y + p2.y) / 2;
    rectangle = sf::RectangleShape(sf::Vector2f(3, distance(p1.x, p1.y, p2.x, p2.y)));
    rectangle.setOrigin(rectangle.getSize().x / 2, rectangle.getSize().y / 2);
    double roto = -atan((double)(p1.x - p2.x) / (p1.y - p2.y));
    rectangle.setRotation(0);
    rectangle.rotate(roto * 180 / pi);
    rectangle.setPosition(x, y);
}

void line::draw(sf::RenderWindow &wind){
    wind.draw(rectangle);
}