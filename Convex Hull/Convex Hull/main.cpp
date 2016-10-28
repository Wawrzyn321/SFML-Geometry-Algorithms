#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Shapes.h"
#include "utility.h"
#include <cmath>
#include <iostream>

using namespace std;

const int fps = 60;
const int refreshRate = 1000 / fps;
int accumulator = 0;

const int windowSize = 600;
sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "");

vector<point> points;
vector<line> lines;
line currentLine;

typedef sf::Vector3f LineEquation;

void refreshWindow() {
    window.clear(sf::Color::Black);
    for (unsigned i = 0; i < points.size(); i++) {
        points[i].draw(window);
    }
    for (unsigned i = 0; i < lines.size(); i++) {
        lines[i].draw(window);
    }
    currentLine.draw(window);
    window.display();
}

double angleBetweenPoints(point p1, point p2) {
    double x = p2.x - p1.x;
    double y = p2.y - p1.y;
    return atan2(x, y);
}

void addRandomPoints(unsigned pointsNum, vector<point> &points, sf::Vector2u windowSize) {
    unsigned shiftX = windowSize.x / 10;
    unsigned shiftY = windowSize.y / 10;
    for (unsigned i = 0; i < pointsNum; i++) {
        float x = (float)(shiftX + rand() % (windowSize.x * 4 / 5));
        float y = (float)(shiftY + rand() % (windowSize.y * 4 / 5));
        points.push_back(point(x, y));
    }
}

double getSide(sf::Vector2f v, sf::Vector2f v1, sf::Vector2f v2) {
    return (v1.x - v2.x) *(v.y - v2.y) - (v1.y - v2.y)*(v.x - v2.x);
}

point *getLeftMost(vector<point> &v) {
    point *p = NULL;
    float mX = windowSize;
    for (unsigned i = 0; i < v.size(); i++) {
        if (v[i].x < mX) {
            mX = v[i].x;
            p = &v[i];
        }
    }
    return p;
}

point *getRightMost(vector<point> &v) {
    point *p = NULL;
    float mX = 0;
    for (unsigned i = 0; i < v.size(); i++) {
        if (v[i].x > mX) {
            mX = v[i].x;
            p = &v[i];
        }
    }
    return p;
}

vector<point*> getPointsOnRightOrTop(vector<point> &v, point *vL, point *vP) {
    vector<point*> r;
    for (unsigned i = 0; i < v.size(); i++) {
        if (getSide(v[i].getPosition(), vL->getPosition(), vP->getPosition()) > 0) {
            r.push_back(&v[i]);
        }
    }
    return r;
}
vector<point*> getPointsOnRightOrTop(vector<point*> v, point *vL, point *vP) {
    vector<point*> r;
    for (unsigned i = 0; i < v.size(); i++) {
        if (getSide(v[i]->getPosition(), vL->getPosition(), vP->getPosition()) > 0) {
            r.push_back(v[i]);
        }
    }
    return r;
}

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

point * getFarthestPoint(LineEquation le, vector<point*> v) {
    double len = 0;
    point *r = NULL;
    for (unsigned i = 0; i < v.size(); i++) {
        //a co jak a i be to zera?
        double l =
            abs(le.x*v[i]->x + le.y*v[i]->y + le.z)
            / sqrt(le.x*le.x + le.y*le.y);
        if (l > len) {
            len = l;
            r = v[i];
        }
    }
    return r;
}
//
//void makeConvexHull(vector<point> &points, vector<line> &lines, line &currentLine) {
//    int p1Index = getFirstPointYMinIndex(points, currentLine);
//    int q1Index = getFirstPointYMaxIndex(points, currentLine);
//    currentLine = line(points[p1Index], points[q1Index]);
//    int i = 0;
//    while (true) {
//        cout << i<<" "<<p1Index << endl;
//        int sIndex = i;
//        int nextIndex=-1;
//        double angle = 2 * pi;
//        for (unsigned j = 0; j < points.size(); j++) {
//            if (j != p1Index) {
//                double a = angleBetweenVectors(
//                    points[sIndex].getPosition(),
//                    points[j].getPosition(),
//                    { 0, 0 },
//                    { 100, 0 }
//                );
//                if (angle > a) {
//                    nextIndex = j;
//                    angle = a;
//                }
//            }
//        }
//        cout << "MIN ANGLE " << angle << endl;
//        lines.push_back(line(points[p1Index], points[nextIndex]));
//        p1Index = nextIndex;
//        if (points[q1Index].x==points[nextIndex].x && points[q1Index].y==points[nextIndex].y) {
//            break;
//        }
//        i++;
//    }
//
//
//}

vector<point*> QuickHull(point A, point B, vector<point*> points)
{
    point *C = getFarthestPoint(getLineEquation(A.getPosition(), B.getPosition()), points);

    vector<point*> s1, s2, arg1, arg3;

    if (C) {
        s1 = getPointsOnRightOrTop(points, &A, C);
        s2 = getPointsOnRightOrTop(points, C, &B);

        arg1 = QuickHull(A, *C, s1);
        arg3 = QuickHull(*C, B, s2);
    }

    vector<point*> vec;
    vec.insert(vec.end(), arg1.begin(), arg1.end());
    if (C) {
        vec.push_back(C);
    }
    vec.insert(vec.end(), arg3.begin(), arg3.end());
    return vec;
}

vector<point*> otoczka(vector<point> points) {
    point *A = getLeftMost(points);
    point *B = getRightMost(points);

    vector<point*> s1 = getPointsOnRightOrTop(points, A, B);
    vector<point*> s2 = getPointsOnRightOrTop(points, B, A);

    vector<point*> arg2 = QuickHull(*A, *B, s1);
    vector<point*> arg4 = QuickHull(*B, *A, s2);

    vector<point*> vec;
    if (A) {
        vec.push_back(A);
    }
    vec.insert(vec.end(), arg2.begin(), arg2.end());
    if (B) {
        vec.push_back(B);
    }
    vec.insert(vec.end(), arg4.begin(), arg4.end());
    return vec;
}

int main() {
    srand((unsigned int)time(NULL));

    window.create(sf::VideoMode(windowSize, windowSize), "", sf::Style::Close);

    sf::Clock timer;
    timer.restart();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();


            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                points.clear();
                lines.clear();
                addRandomPoints(100, points, window.getSize());
                vector<point*> p = otoczka(points);
                for (unsigned i = 0; i < p.size(); i++) {
                    point p1 = *p[i];
                    point p2 = *p[(i + 1) % p.size()];
                    lines.push_back(line(p1, p2));
                }
            }

        }


        //handling time
        accumulator += (int)timer.getElapsedTime().asMicroseconds();
        timer.restart();
        if (accumulator >= refreshRate * 1000) {
            refreshWindow();
            accumulator = 0;
        }
    }
    return 0;
}