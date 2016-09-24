#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include "utility.h"
#include <vector>
#include "Shapes.h"

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

bool isShapeClockWise(vector<point> &sourcePoints){
	double sum = 0;
	for (unsigned int i = 0; i < sourcePoints.size(); i++){

		unsigned int k1 = i + 1;
        if (k1 >= sourcePoints.size())
            k1 -= sourcePoints.size();

        unsigned int k2 = i + 2;
		if (k2 >= sourcePoints.size())
            k2 -= sourcePoints.size();

		if (areVecsConvex(sourcePoints[i].getPosition(), sourcePoints[k1].getPosition(), sourcePoints[k2].getPosition())){
			sum += angleBetweenPoints(sourcePoints[i].getPosition(), sourcePoints[k1].getPosition(), sourcePoints[k2].getPosition());
		}
		else{
			sum += 360 - angleBetweenPoints(sourcePoints[i].getPosition(), sourcePoints[k1].getPosition(), sourcePoints[k2].getPosition());
		}
	}
    return round(sum) == 180 * (sourcePoints.size() - 2);
}

void addPoint(float  nX, float  nY) {
    //don't duplicate points!
    for (unsigned int i = 0; i < points.size(); i++)
        if (nX == points[i].x && nY == points[i].y)
            return;

    //add point
    points.push_back(point(nX, nY));

    //draw line, if we have more than 1 point
    if (points.size() > 1) {
        lines.push_back(line(points[points.size() - 2], points[points.size() - 1]));
    }
}

//with effects
void triangulation(const vector<point> &sourcePoints, bool delayEnabled, bool isCool = false){
    int delay;
    //calculate nice delay
    if (delayEnabled) {
        delay = 750;
        if (sourcePoints.size() > 15) {
            delay = delay - sourcePoints.size() * 25;
            if (delay < 10)
                delay = 10;
            if (delay >= 500)
                delay = 500;
        }
    }
    else {
        delay = 0;
        isCool = false;
    }

    if (delayEnabled) {
        refreshWindow();
        Sleep(delay);
    }

    //temporary vector of points
	vector<point> verts = sourcePoints;

    //was shape drew clockwise?
	bool isCW = isShapeClockWise(verts);
    unsigned int start = 0;
	while (verts.size() > 2){
		bool earNotFound = true;

        //when triangulation isCool and delay is enabled
        if (isCool) {
            //only visual, while isCool==true performance is a little bit worse
            start = 0;
        }
        else {
            ;
        }

		while (earNotFound){
			start %= verts.size();
            unsigned int middle = (start + 1) % verts.size();
            unsigned int end = (start + 2) % verts.size();

            triangles.push_back(triangle(verts[start], verts[middle], verts[end]));
            //is current point convex?
            bool isConvex =
                isPointConvex(verts[start].getPosition(), verts[middle].getPosition(), verts[middle].getPosition(), verts[end].getPosition(), !isCW)
                && isPointConvex(verts[middle].getPosition(), verts[end].getPosition(), verts[end].getPosition(), verts[start].getPosition(), !isCW)
                && isPointConvex(verts[end].getPosition(), verts[start].getPosition(), verts[start].getPosition(), verts[middle].getPosition(), !isCW);

			if (!isConvex){
                //reject the triangle
				triangles[triangles.size() - 1].tri.setFillColor(sf::Color::Red);
				start++;
                triangles.erase(triangles.begin() + triangles.size() - 1);
                continue;
			}

            bool noPointsIn = true;

            triangles[triangles.size() - 1].tri.setFillColor(sf::Color::Green);
            for (unsigned int i = 0; i < verts.size(); i++)
                if (i != start && i != middle && i != end)
                    if (isPointInTriangle(verts[i].getPosition(), verts[start].getPosition(), verts[middle].getPosition(), verts[end].getPosition()) && noPointsIn) {
                        noPointsIn = false;
                        break;
                    }

            if (noPointsIn) {
                //we found ear
                earNotFound = false;
            }
            else {
                //there's a point in triangle
                triangles[triangles.size() - 1].tri.setFillColor(sf::Color::Red);
                start++;
            }

            if (delayEnabled) {
                refreshWindow();
                Sleep(delay);
            }
			if (!noPointsIn){
                //reject the triangle
				triangles.erase(triangles.begin() + triangles.size() - 1);
			}
			else{
                //add triangle to set
				verts.erase(verts.begin() + middle);
				triangles[triangles.size() - 1].tri.setFillColor(sf::Color::White);

                if (delayEnabled) {
                    refreshWindow();
                    Sleep(delay);
                }
			}
		}
	}
}

//no effects
void triangulation(const vector<point> &sourcePoints) {
    //temporary vector of points
    vector<point> verts = sourcePoints;

    //was shape drew clockwise?
    bool isCW = isShapeClockWise(verts);
    unsigned int start = 0;
    while (verts.size() > 2) {

        bool earNotFound = true;
        while (earNotFound) {
            start %= verts.size();
            unsigned int middle = (start + 1) % verts.size();
            unsigned int end = (start + 2) % verts.size();

            triangles.push_back(triangle(verts[start], verts[middle], verts[end]));
            //is current point convex?
            bool isConvex =
                isPointConvex(verts[start].getPosition(), verts[middle].getPosition(), verts[middle].getPosition(), verts[end].getPosition(), !isCW)
                && isPointConvex(verts[middle].getPosition(), verts[end].getPosition(), verts[end].getPosition(), verts[start].getPosition(), !isCW)
                && isPointConvex(verts[end].getPosition(), verts[start].getPosition(), verts[start].getPosition(), verts[middle].getPosition(), !isCW);

            if (!isConvex) {
                //reject the triangle
                start++;
                triangles.erase(triangles.begin() + triangles.size() - 1);
                continue;
            }

            bool noPointsIn = true;

            for (unsigned int i = 0; i < verts.size(); i++)
                if (i != start && i != middle && i != end)
                    if (isPointInTriangle(verts[i].getPosition(), verts[start].getPosition(), verts[middle].getPosition(), verts[end].getPosition()) && noPointsIn) {
                        //there's a point in triangle
                        noPointsIn = false;
                        //reject the triangle
                        start++;
                        triangles.erase(triangles.begin() + triangles.size() - 1);
                        break;
                    }

            if (noPointsIn) {
                earNotFound = false;
                //add triangle to set
                verts.erase(verts.begin() + middle);
            }
        }
    }
}


int main()
{
    bool canInteract = true;
    bool delayEnabled = true;
    bool coolEnabled = true;
    cout << "cool and delay are enabled" << endl;

	window.create(sf::VideoMode(800, 600), "triangulation (r - reset, d - delay on/off, c - cool on/off)", sf::Style::Resize | sf::Style::Close);

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

            //enabling/disabling coolness
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) {
                coolEnabled = !coolEnabled;
                cout << "coolness " << (delayEnabled ? "en" : "dis") << "abled" << endl;
            }

            //reset
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                cout << "cool and delay are enabled" << endl;
                coolEnabled = true;
                delayEnabled = true;
				canInteract = true;
				cout.clear();
				points.clear();
				lines.clear();
				triangles.clear();
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
                    //complete the shape (there's no intersecting lines)
                    lines.push_back(line(points[0], points[points.size() - 1]));
					//closeShape(points, delayEnabled, coolEnabled);
                    triangulation(points);
					canInteract = false;
				}
			}
		}

		refreshWindow();
	}
	return 0;
}
