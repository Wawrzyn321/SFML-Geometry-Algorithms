#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <vector>
#include "Shapes.h"
#include "utility.h"

using namespace std;

sf::RenderWindow window(sf::VideoMode(800, 600), "Subdivision smoothing");

vector<point> currentPoints;
vector<line> currentLines;
vector<point> subPoints;
vector<line> subLines;
vector<line> allLines;
vector<point> allPoints;

void refreshWindow(){
	window.clear();
    for (unsigned int i = 0; i < subLines.size(); i++)
        subLines[i].draw(window);
    for (unsigned int i = 0; i < subPoints.size(); i++)
        subPoints[i].draw(window);
    for (unsigned int i = 0; i < allLines.size(); i++)
        allLines[i].draw(window);
    for (unsigned int i = 0; i < allPoints.size(); i++)
        allPoints[i].draw(window);
    /*
    for (unsigned int i = 0; i < currentLines.size(); i++)
        currentLines[i].draw(window);
	for (unsigned int i = 0; i < currentPoints.size(); i++)
        currentPoints[i].draw(window);
        */
	window.display();
}

void addPoint(float  nX, float  nY) {
    //don't duplicate points!
    for (unsigned int i = 0; i < currentPoints.size(); i++)
        if (nX == currentPoints[i].x && nY == currentPoints[i].y)
            return;

    //add point
    currentPoints.push_back(point(nX, nY));
    allPoints.push_back(currentPoints.back());

    //draw line, if we have more than 1 point
    if (currentPoints.size() > 1) {
        currentLines.push_back(line(currentPoints[currentPoints.size() - 2], currentPoints[currentPoints.size() - 1]));
        allLines.push_back(currentLines.back());
    }
}

vector<point> subdivide(vector<point> &somePoints, float t) {
    vector<point> subPoints;

    for (unsigned i = 0; i < somePoints.size(); i++) {
        //start, middle and end of segment
        sf::Vector2f a = somePoints[i].getPosition();
        sf::Vector2f b = somePoints[(i + 1) % somePoints.size()].getPosition();
        sf::Vector2f c = somePoints[(i + 2) % somePoints.size()].getPosition();

        //mid-points of segments
        sf::Vector2f mid1 = lerp(a, b, 0.5);
        sf::Vector2f mid2 = lerp(b, c, 0.5);

        //mid-point of mid-points
        sf::Vector2f mid_mid = lerp(mid1, mid2, 0.5);

        //segment line equation {y = wA*x + b}
        double wA = (mid2.y - mid1.y) / (mid2.x - mid1.x);
        if (mid2.x - mid1.x == 0) {
            wA = 0;
        }
        double wB = mid1.y - wA*mid1.x;

        //equation of perpendicular line...
        double p_wA = -1 / wA;
        //... which passes through mid-point {m}
        double p_wB = mid_mid.y - p_wA*mid_mid.x;

        float x = (float)((p_wB - wB) / (wA - p_wA));
        if (wA - p_wA == 0) {
            x = 0;
        }
        float y = (float)(x*wA + wB);

        sf::Vector2f end = lerp(b, sf::Vector2f(x, y), t);

        subPoints.push_back(mid1);
        subPoints.push_back(point(end));
    }
    return subPoints;
}

vector<line> getLines(const vector<point> somePoints) {
    vector<line> someLines;
    someLines.push_back(line(somePoints[0], somePoints.back()));
    for (unsigned i = 0; i < somePoints.size()-1; i++) 
        someLines.push_back(line(somePoints[i], somePoints[i + 1]));
    return someLines;
}

void subdivide(float tVal) {
    for (unsigned i = 0; i < currentPoints.size(); i++) {
        allPoints.push_back(currentPoints[i]);
    }
    for (unsigned i = 0; i < currentLines.size(); i++) {
        allLines.push_back(currentLines[i]);
    }
    subPoints = subdivide(currentPoints, tVal);
    subLines = getLines(subPoints);
}

void dim(int minAlpha, float fadeAmount) {
    for (unsigned i = 0; i < allLines.size(); i++) {
        sf::Color c = allLines[i].rectangle.getFillColor();
        allLines[i].rectangle.setFillColor(sf::Color(c.r, c.g, c.b, lerp(c.a, minAlpha, fadeAmount)));
    }
    for (unsigned i = 0; i < allPoints.size(); i++) {
        sf::Color c = allPoints[i].circle.getFillColor();
        allPoints[i].circle.setFillColor(sf::Color(c.r, c.g, c.b, lerp(c.a, minAlpha, fadeAmount)));
    }
}

int main()
{
    float tVal = 0.5;
    bool canInteract = true;
    int minAlpha = 31;
    float fadeAmount = 10 / 100.0f;

	window.create(sf::VideoMode(800, 600), "Subdivision smoothing", sf::Style::Resize | sf::Style::Close);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{

            if (!canInteract) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) {
                    tVal += 0.1f;
                    subPoints = subdivide(currentPoints, tVal);
                    subLines = getLines(subPoints);
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D) {
                    tVal -= 0.1f;
                    subPoints = subdivide(currentPoints, tVal);
                    subLines = getLines(subPoints);
                }
            }

            //exit
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
				window.close();
			}


            //reset
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                currentPoints.clear();
                currentLines.clear();
                allLines.clear();
                allPoints.clear();
                subPoints.clear();
                subLines.clear();
                canInteract = true;
            }


            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && !canInteract) {
                currentPoints = subPoints;
                currentLines = subLines;
                subdivide(tVal);
                dim(minAlpha, fadeAmount);
            }

            //adding new point
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && canInteract){
				int mouseX = sf::Mouse::getPosition(window).x;
				int mouseY = sf::Mouse::getPosition(window).y;
				bool intersectionPresent = false;
                if (currentPoints.size() > 2) {
                    for (unsigned int i = 0; i < currentPoints.size() - 2; i++){
                        if (isIntersection(currentPoints[currentPoints.size() - 1].getPosition(), sf::Vector2f((float)mouseX, (float)mouseY), currentPoints[i].getPosition(), currentPoints[i + 1].getPosition())) {
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

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && currentPoints.size() > 2 && canInteract){
				bool intersectionPresent = false;
                for (unsigned int i = 0; i < currentPoints.size() - 2; i++) {
                    if (isIntersection(currentPoints[0].getPosition(), currentPoints[currentPoints.size() - 1].getPosition(), currentPoints[i].getPosition(), currentPoints[i + 1].getPosition())) {
                        intersectionPresent = true;
                        cout << "cannot close shape: indicated point makes intersecting line." << endl;
                        break;
                    }
                }
				if (!intersectionPresent){
                    //complete the shape (there's no intersecting lines)
                    currentLines.push_back(line(currentPoints[0], currentPoints[currentPoints.size() - 1]));
                    allLines.push_back(currentLines.back());
                    subdivide(tVal);
                    dim(minAlpha, fadeAmount);
					canInteract = false;
				}
			}
		}

		refreshWindow();
	}
	return 0;
}
