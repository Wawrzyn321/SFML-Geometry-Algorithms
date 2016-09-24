#include "Geometry.h"


double distance(float x1, float y1, float x2, float y2){
    return sqrt((double)(x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

double distance(sf::Vector3f eq, sf::Vector2f p){
    return  abs(p.x*eq.x + p.y*eq.y + eq.z) / sqrt(eq.x*eq.x + eq.y*eq.y);
}

double squaredDistance(sf::Vector2f p1, sf::Vector2f p2){
    return (double)(p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
}


double dotProduct(sf::Vector2f a, sf::Vector2f b){
    return a.x*b.x + a.y * b.y;
}

sf::Vector3f getStandardLineEquation(sf::Vector2f a, sf::Vector2f b){
    if (a == b)
        return{ 0, 0, 0 };
    if (a.x - b.x == 0)
        return{ 1, 0, -a.x };
    else{
        double aw = (double)(a.y - b.y) / (a.x - b.x);
        double bw = a.y - aw*a.x;
        return{ (float)aw, -1.0f, (float)bw };
    }
}

double angleBetweenPoints(sf::Vector2f p1, sf::Vector2f p2){
    double a = atan2(p1.y - p2.y, p1.x - p1.x);
    a = (a > 0 ? a : (2 * pi + a)) * 360 / (2 * pi);
    return a;
}

void normalize(float& angle){
    while (angle < -180)
        angle += 360;
    while (angle >  180)
        angle -= 360;
}

sf::Vector2f operator/(const sf::Vector2f &v1, float n){
    if (n == 0)
        return { 0, 0 };
    else
        return sf::Vector2f(v1.x / n, v1.y / n);
}

sf::Vector2f operator*(float n, const sf::Vector2f &v1){
    return sf::Vector2f(v1.x * n, v1.y * n);
}

sf::Vector2f operator*(const sf::Vector2f &v1, float n){
    return sf::Vector2f(v1.x * n, v1.y * n);
}

double pointLineDistance(float xP, float yP, float x1, float y1, float x2, float y2){
    //https://www.allegro.cc/forums/thread/595918
    float A = xP - x1;//vector from one end point to the test point
    float B = yP - y1;
    float C = x2 - x1;//vector from one end point to the other end point
    float D = y2 - y1;

    float dot = A * C + B * D;//some interesting math coming from the geometry of the algorithm
    float len_sq = C * C + D * D;
    float param = dot / len_sq;

    float xx, yy;//the coordinates of the point on the line segment closest to the test point

    //the parameter tells us which point to pick
    //if it is outside 0 to 1 range, we pick one of the endpoints
    //otherwise we pick a point inside the line segment
    if (param < 0){
        xx = x1;
        yy = y1;
    }
    else if (param > 1)  {
        xx = x2;
        yy = y2;
    }
    else  {
        xx = x1 + param * C;
        yy = y1 + param * D;
    }

    return distance(xP, yP, xx, yy);//distance from the point to the segment
}
