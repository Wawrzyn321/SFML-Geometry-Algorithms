#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "utility.h"
#include "Shapes.h"
#include <vector>
#include "main.h"

using namespace std;

sf::RenderWindow window(sf::VideoMode(800, 600), "triangulation");



vector<point> points;
vector<line> lines;
vector<triangle> triangles;

void refreshWindow(){
	window.clear();
    for (unsigned int i = 0; i < triangles.size(); i++)
        triangles[i].draw(window);
	for (unsigned int i = 0; i < lines.size(); i++)
        lines[i].draw(window);
	for (unsigned int i = 0; i < points.size(); i++)
        points[i].draw(window);
	window.display();
}


inline bool isPointConvex(point p1, point p2, point p3, bool isCW) {
    return isPointConvex(p1.getPosition(), p2.getPosition(), p2.getPosition(), p3.getPosition(), isCW);
}

bool isClockwise(){//usable only in this project
	double sum = 0;
	for (unsigned int i = 0; i < points.size(); i++){
		unsigned int k1 = i + 1;
        unsigned int k2 = i + 2;
		if (k1 >= points.size()) k1 %= points.size();
		if (k2 >= points.size()) k2 %= points.size();
		if (areVecsConvex(points[i].getPosition(), points[k1].getPosition(), points[k2].getPosition())){
			sum += angleBetweenPoints(points[i].getPosition(), points[k1].getPosition(), points[k2].getPosition());
		}
		else{
			sum += 360 - angleBetweenPoints(points[i].getPosition(), points[k1].getPosition(), points[k2].getPosition());
		}
	}
    return round(sum) == 180 * (points.size() - 2);
}

void addPoint(float  nX, float  nY) {
    //don't duplicate points!
    for (unsigned int i = 0; i < points.size(); i++)
        if (nX == points[i].x && nY == points[i].y)
            return;

    //add point
    points.push_back(point(nX, nY));

    //draw line, if we have more than 1 point
    if (points.size()>1)
        lines.push_back(line(points[points.size()-2], points[points.size()-1]));
}

void closeShape(bool delayEnabled){
    int delay;
    if (delayEnabled) {
        delay = 750;
        if (points.size() > 15) {
            delay = delay - points.size() * 25;
            if (delay < 10)
                delay = 10;
            if (delay >= 500)
                delay = 500;
        }
    }
    else {
        delay = 0;
    }
	lines.push_back(line(points[0], points[points.size() - 1]));
	refreshWindow();
	vector<point> verts = points;
	bool cw = isClockwise();
	while (verts.size() > 2){
		bool earNotFound = true;
        int s = 0;
        unsigned int t = 0;
		while (earNotFound){
            t++;
			s %= verts.size();
			int m = (s + 1) % verts.size();
			int e = (s + 2) % verts.size();
			float xS = verts[s].x;
			float yS = verts[s].y;
			float xM = verts[m].x;
			float yM = verts[m].y;
			float xE = verts[e].x;
			float yE = verts[e].y;

			triangles.push_back(triangle(verts[s], verts[m], verts[e]));
			bool isConvex = false;
			if (isPointConvex(verts[s].getPosition(), verts[m].getPosition(), verts[m].getPosition(), verts[e].getPosition(), !cw)
                && isPointConvex(verts[m].getPosition(), verts[e].getPosition(), verts[e].getPosition(), verts[s].getPosition(), !cw)
                && isPointConvex(verts[e].getPosition(), verts[s].getPosition(), verts[s].getPosition(), verts[m].getPosition(), !cw))
				isConvex = true;
			if (isConvex){
				triangles[triangles.size() - 1].tri.setFillColor(sf::Color::Green);
			}
			else{
				triangles[triangles.size() - 1].tri.setFillColor(sf::Color::Red);
				s++;
			}
			bool noPointsIn = true;
			if (isConvex){
				for (unsigned int i = 0; i < verts.size(); i++)
					if (i != s && i != m && i != e)
						if (isPointInTriangle(verts[i].getPosition(), verts[s].getPosition(), verts[m].getPosition(), verts[e].getPosition()) && noPointsIn)
							noPointsIn = false;
				if (noPointsIn){
					//cout << "no points in triangle" << endl;
					earNotFound = false;
				}
				else{
					triangles[triangles.size() - 1].tri.setFillColor(sf::Color::Red);
					//cout << "point in triangle" << endl;
					s++;
				}
			}
            if (delayEnabled) {
                refreshWindow();
                Sleep(delay);
            }
			if (!noPointsIn || !isConvex){
				triangles.erase(triangles.begin() + triangles.size() - 1);
			}
			else{
				verts.erase(verts.begin() + m);
				triangles[triangles.size() - 1].tri.setFillColor(sf::Color::White);
                if (delayEnabled) {
                    refreshWindow();
                    Sleep(delay);
                }
			}
		}
	}
}


int main()
{
    bool canInteract = true;
    bool delayEnabled = true;

	window.create(sf::VideoMode(800, 600), "triangulation (r - reset, d - delay on/off)", sf::Style::Resize | sf::Style::Close);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{

            //exit
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
				window.close();
			}

            //enabling/disabling delay
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D) {
                delayEnabled = !delayEnabled;
                cout << "delay " << (delayEnabled ? "en" : "dis") << "abled" << endl;
            }

            //reset
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
				canInteract = true;
				cout.clear();
				points.clear();
				lines.clear();
				triangles.clear();
                system("cls");
			}


            //adding new point
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && canInteract){
				int mouseX = sf::Mouse::getPosition(window).x;
				int mouseY = sf::Mouse::getPosition(window).y;
				bool intersectionPresent = false;
                if (points.size() > 2) {
                    for (unsigned int i = 0; i < points.size() - 2; i++){
                        if (isIntersection(points[points.size() - 1].getPosition(), sf::Vector2f((float)mouseX, (float)mouseY), points[i].getPosition(), points[i + 1].getPosition())) {
                            intersectionPresent = true;
                            cout << "indicated point makes intersecting line." << endl;
                            break;
                        }
                    }
                }
				if (!intersectionPresent){
					addPoint((float)mouseX, (float)mouseY);
				}
			}

            //closing the shape
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && points.size() > 2 && canInteract){
				bool intersectionPresent = false;
                for (unsigned int i = 0; i < points.size() - 2; i++) {
                    if (isIntersection(points[0].getPosition(), points[points.size() - 1].getPosition(), points[i].getPosition(), points[i + 1].getPosition())) {
                        intersectionPresent = true;
                        cout << "cannot close shape: indicated point makes intersecting line." << endl;
                        break;
                    }
                }
				if (!intersectionPresent){
					closeShape(delayEnabled);
					canInteract = false;
				}
			}
		}

		refreshWindow();
	}
	return 0;
}
