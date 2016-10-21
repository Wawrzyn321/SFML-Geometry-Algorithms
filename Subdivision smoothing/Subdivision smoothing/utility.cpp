#include "utility.h"


double distance(float x1, float y1, float x2, float y2) {
    return sqrt((double)(x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

double distance(sf::Vector2f p1, sf::Vector2f p2) {
    float a = p1.x - p2.x;
    float b = p1.y - p2.y;
    return sqrt((double)a*a + b*b);
}

inline bool checkIntersectionHelper(sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f v3) {
    return (v2.y - v1.y)*(v3.x - v1.x) <= (v3.y - v1.y)*(v2.x - v1.x);
}

bool isIntersection(sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f v3, sf::Vector2f v4) {
    if (checkIntersectionHelper(v1, v2, v3) == checkIntersectionHelper(v1, v2, v4))
        return false;
    if (checkIntersectionHelper(v3, v4, v1) == checkIntersectionHelper(v3, v4, v2))
        return false;
    return true;
}

sf::Vector2f lerp(sf::Vector2f a, sf::Vector2f b, float t) {
    sf::Vector2f v = b - a;
    return a + sf::Vector2f(v.x*t, v.y*t);
}

int lerp(int a, int b, float t) {
    return a + (b - a)*t;
}