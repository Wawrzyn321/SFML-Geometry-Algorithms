#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "ConvexHull.h"
#include "Shapes.h"
#include "utility.h"
#include <cmath>
#include <iostream>

using namespace std;

const int windowSize = 600;
sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Convex hull");

vector<point> points;
vector<line> lines;

void refreshWindow() {
    window.clear(sf::Color::Black);
    for (unsigned i = 0; i < points.size(); i++) {
        points[i].draw(window);
    }
    for (unsigned i = 0; i < lines.size(); i++) {
        lines[i].draw(window);
    }
    window.display();
}

void addRandomPoints(unsigned pointsNum, vector<point> &points, sf::Vector2u windowSize) {
    //add random points in range {<100, winSize-100>, <100, winSize-100>}
    unsigned shiftX = windowSize.x / 10;
    unsigned shiftY = windowSize.y / 10;
    for (unsigned i = 0; i < pointsNum; i++) {
        float x = (float)(shiftX + rand() % (windowSize.x * 4 / 5));
        float y = (float)(shiftY + rand() % (windowSize.y * 4 / 5));
        points.push_back(point(x, y));
    }
}

void addRandomPoint(vector<point> &points, sf::Vector2u windowSize) {
    unsigned shiftX = windowSize.x / 10;
    unsigned shiftY = windowSize.y / 10;
    float x = (float)(shiftX + rand() % (windowSize.x * 4 / 5));
    float y = (float)(shiftY + rand() % (windowSize.y * 4 / 5));
    points.push_back(point(x, y));
}

void makeHull(){
    //get hull points
    vector<point*> p = QuickHull(points);

    //join points
    if (p.size() > 0) {
        sf::Vector2f v1 = p[0]->getPosition();
        for (unsigned i = 1; i < p.size(); i++) {
            sf::Vector2f v2 = p[i]->getPosition();
            lines.push_back(line(v1, v2));
            v1 = v2;
        }
        lines.push_back(line(v1, p[0]->getPosition()));
    }
}

int main() {
    srand((unsigned int)time(NULL));

    //int numOfPoints = 100;

    window.create(sf::VideoMode(windowSize, windowSize), "Convex Hull. Use mouse scroll to add more points", sf::Style::Close);

    addRandomPoints(20, points, window.getSize());
    makeHull();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            /*
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                points.clear();
                lines.clear();
                addRandomPoints(numOfPoints, points, window.getSize());
                makeHull();
            }
            */
            if (event.type == sf::Event::MouseWheelMoved)
            {
                lines.clear();
                if (event.mouseWheel.delta>0) {
                    //add more points
                    addRandomPoint(points, window.getSize());
                }
                else {
                    //remove last point
                    if (points.size() > 0) {
                        points.erase(points.begin() + points.size() - 1);
                    }
                }
                if (points.size()>2) {
                    makeHull();
                }
            }
        }

        refreshWindow();
    }
    return 0;
}