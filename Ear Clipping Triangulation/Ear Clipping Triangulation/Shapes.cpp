#include "Shapes.h"
#include "utility.h"


point::point(float ix, float iy) {
    x = ix;
    y = iy;
    circle = sf::CircleShape(4, 10);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setPosition(x, y);
    circle.setFillColor(niceBlue);
}
void point::draw(sf::RenderWindow & wind)
{
    wind.draw(circle);
}
sf::Vector2f point::getPosition() {
    return sf::Vector2f(x, y);
}

line::line(float x1, float y1, float x2, float y2) {
    x = (x1 + x2) / 2;
    y = (y1 + y2) / 2;
    rectangle = sf::RectangleShape(sf::Vector2f(3, (float)distance(x1, y1, x2, y2)));
    rectangle.setOrigin(rectangle.getSize().x / 2, rectangle.getSize().y / 2);
    float roto = -atan((x1 - x2) / (y1 - y2));
    rectangle.setRotation(roto * 180 / (float)pi);
    rectangle.setPosition(x, y);
    rectangle.setFillColor(niceBlue);
}
line::line(point p1, point p2) {
    x = (p1.x + p2.x) / 2;
    y = (p1.y + p2.y) / 2;
    rectangle = sf::RectangleShape(sf::Vector2f(3, (float)distance(p1.x, p1.y, p2.x, p2.y)));
    rectangle.setOrigin(rectangle.getSize().x / 2, rectangle.getSize().y / 2);
    float roto = -atan((p1.x - p2.x) / (p1.y - p2.y));
    rectangle.setRotation(roto * 180 / (float)pi);
    rectangle.setPosition(x, y);
    rectangle.setFillColor(niceBlue);
}

void line::draw(sf::RenderWindow & wind)
{
    wind.draw(rectangle);
}

triangle::triangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    tri.setPointCount(3);
    tri.setPoint(0, sf::Vector2f(x1, y1));
    tri.setPoint(1, sf::Vector2f(x2, y2));
    tri.setPoint(2, sf::Vector2f(x3, y3));
}

triangle::triangle(point p1, point p2, point p3)
{
    tri.setPointCount(3);
    tri.setPoint(0, p1.getPosition());
    tri.setPoint(1, p2.getPosition());
    tri.setPoint(2, p3.getPosition());
}

void triangle::draw(sf::RenderWindow & wind)
{
    wind.draw(tri);
}
