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
    unsigned shiftY = windowSize.y/ 10;
    for (unsigned i = 0; i < pointsNum; i++) {
        float x = (float)(shiftX+rand() % (windowSize.x*4/5));
        float y = (float)(shiftY+rand() % (windowSize.y*4/5));
        points.push_back(point(x, y));
    }
}

int getRightMostIndex(vector<point> &points) {
    int pointIndex = 0;
    for (unsigned i = 1; i < points.size(); i++) {
        if (points[pointIndex].x > points[i].x) {
            pointIndex = i;
        }
    }
    return pointIndex;
}

int getLeftMostIndex(vector<point> &points) {
    int pointIndex = 0;
    for (unsigned i = 1; i < points.size(); i++) {
        if (points[pointIndex].x  < points[i].x) {
            pointIndex = i;
        }
    }
    return pointIndex;
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


int main() {
    srand((unsigned int)time(NULL));

    window.create(sf::VideoMode(windowSize, windowSize), "", sf::Style::Close);

    sf::Clock timer;
    timer.restart();

    addRandomPoints(100, points, window.getSize());
    vector<point> p = otoczka(points);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

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