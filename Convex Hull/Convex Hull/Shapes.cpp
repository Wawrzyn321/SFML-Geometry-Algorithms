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

inline bool point::operator!=(const point & p)
{
    return x != p.x || y != p.y;
}

inline bool point::operator==(const point & p)
{
    return x == p.x && y == p.y;
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

line::line(sf::Vector2f v1, sf::Vector2f v2) {
    x = (v1.x + v2.x) / 2;
    y = (v1.y + v2.y) / 2;
    rectangle = sf::RectangleShape(sf::Vector2f(3, (float)distance(v1.x, v1.y, v2.x, v2.y)));
    rectangle.setOrigin(rectangle.getSize().x / 2, rectangle.getSize().y / 2);
    float roto = -atan((v1.x - v2.x) / (v1.y - v2.y));
    rectangle.setRotation(roto * 180 / (float)pi);
    rectangle.setPosition(x, y);
    rectangle.setFillColor(niceBlue);
}

void line::draw(sf::RenderWindow & wind)
{
    wind.draw(rectangle);
}
