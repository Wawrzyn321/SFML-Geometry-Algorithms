#include "utility.h"


double distance(float x1, float y1, float x2, float y2) {
    return sqrt((double)(x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

double distance(sf::Vector2f p1, sf::Vector2f p2) {
    float a = p1.x - p2.x;
    float b = p1.y - p2.y;
    return sqrt((double)a*a + b*b);
}


double triangleArea(sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f v3)
{
    double det = v1.x*(v2.y - v3.y) + v2.x*(v3.y - v1.y) + v3.x*(v1.y - v2.y);
    return abs(det) / 2.0;
}

bool isPointInTriangle(sf::Vector2f v, sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f v3)
{
    double totalArea = triangleArea(v1, v2, v3);
    double AArea = triangleArea(v1, v, v3);
    double BArea = triangleArea(v, v2, v3);
    double CArea = triangleArea(v1, v2, v);
    return totalArea == AArea + BArea + CArea;
}


double angleBetweenVectors(sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f v3, sf::Vector2f v4) {
    double k;
    double l1 = distance(v1, v2);
    double l2 = distance(v3, v4);
    double dp = (v2.x - v1.x)*(v4.x - v3.x) + (v2.y - v1.y)*(v4.y - v3.y);
    k = dp / l1 / l2;
    return acos(k) * 180 / pi;
}

double angleBetweenPoints(sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f v3) {
    return angleBetweenVectors(v2, v1, v2, v3);
}


bool isPointConvex(sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f v3, sf::Vector2f v4, bool isCW) {
    double k;
    k = atan2((v2.x - v1.x)*(v4.y - v3.y) - (v4.x - v3.x)*(v2.y - v1.y), (v2.x - v1.x)*(v4.x - v3.x) + (v2.y - v1.y)*(v4.y - v3.y));
    if (isCW)
        return k < 0;
    else
        return k > 0;
}

bool areVecsConvex(sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f v3) {
    return isPointConvex(v2, v1, v2, v3, true);
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
