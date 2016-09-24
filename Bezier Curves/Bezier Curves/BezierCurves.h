#ifndef BEZIER_CURVES_H
#define BEZIER_CURVES_H
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <vector>
#include "Shapes.h"

using namespace std;


const int THRESHOLD = 20;
const int THRESHOLD_squared = THRESHOLD*THRESHOLD;

sf::Vector2f calculateBezierPoint(double t, sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3);

void bezierR(vector<sf::Vector2f> &vec, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, int &pos, double t0, double t1);

void startBezierCurve(vector<sf::Vector2f> &vec, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4);


class bezierLine{
private:
    sf::Color niceBlue = sf::Color(100, 100, 255);

    vector<sf::Vector2f> poses;
    vector<line> segments;

    sf::Vector2f p1, p2, c1, c2;

    line line_control1, line_control2;
public:
    point point_p1, point_p2, point_c1, point_c2;
    bool showControls = true;
    bool isChoosen = false;

    bezierLine(){}
    bezierLine(sf::Vector2f _p1, sf::Vector2f _p2);
    bezierLine(sf::Vector2f _p1, sf::Vector2f _c1, sf::Vector2f _c2, sf::Vector2f _p2);

    bool setPoint(int pointID, sf::Vector2f coords);

    void draw(sf::RenderWindow &wind);

    bool getCollision(sf::Vector2f pos);

    bool getPointsCollision(sf::Vector2f pos);

    void clearDots();
};

#endif
