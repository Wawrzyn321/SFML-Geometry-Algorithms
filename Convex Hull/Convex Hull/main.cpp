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

void makeHull(){
    //get hull points
    vector<point*> p = QuickHull(points);
    //join points
    for (unsigned i = 0; i < p.size(); i++) {
        lines.push_back(line(
            *p[i],
            *p[(i + 1) % p.size()]
        ));
    }
}

int main() {
    srand((unsigned int)time(NULL));

    window.create(sf::VideoMode(windowSize, windowSize), "Convex Hull", sf::Style::Close);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                points.clear();
                lines.clear();
                addRandomPoints(100, points, window.getSize());
                makeHull();
            }

        }

        refreshWindow();
    }
    return 0;
}