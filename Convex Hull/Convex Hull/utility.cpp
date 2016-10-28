#include "utility.h"


double distance(float x1, float y1, float x2, float y2) {
    return sqrt((double)(x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

/*
Dervive line formula from given points: Ax + By + C = 0.
Returned type is SFML's Vector3:
    x - A
    y - B
    z - C
*/
LineEquation getLineEquation(sf::Vector2f a, sf::Vector2f b) {
    if (a.x == b.x) {
        return sf::Vector3f(1, 0, -a.x);
    }
    else if (a.y == b.y) {
        return sf::Vector3f(0, 1, -a.y);
    }
    else {
        float bV = (a.x - b.x) / (a.y - b.y);
        float cV = (bV*a.y - a.x);
        return sf::Vector3f(1, -bV, cV);
    }
}

//get side of point {v} relative to vector {v1}{v2}
double getSide(sf::Vector2f v, sf::Vector2f v1, sf::Vector2f v2) {
    return (v1.x - v2.x) *(v.y - v2.y) - (v1.y - v2.y)*(v.x - v2.x);
}