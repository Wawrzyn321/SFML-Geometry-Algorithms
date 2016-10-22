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

        //segment line equation {y = wA*x + b}
        double wA = (mid2.y - mid1.y) / (mid2.x - mid1.x);
        if (mid2.x - mid1.x == 0) {
            wA = 0;
        }
        double wB = mid1.y - wA*mid1.x;

        //mid-point of mid-points
        sf::Vector2f mid_mid = lerp(mid1, mid2, 0.5);

        //equation of perpendicular line...
        double p_wA = -1 / wA;
        //... which passes through mid-point {m}
        double p_wB = mid_mid.y - p_wA*mid_mid.x;

        float x = (float)((p_wB - wB) / (wA - p_wA));
        if (wA - p_wA == 0) {
            x = 0;
        }
        float y = (float)(x*wA + wB);

        //final point
        sf::Vector2f end = lerp(b, sf::Vector2f(x, y), t);

        //add point on {a-b} segment
        subPoints.push_back(mid1);
        //add final point
        subPoints.push_back(point(end));
    }
    return subPoints;
}

vector<line> getLines(const vector<point> &points) {
    vector<line> lines;
    lines.push_back(line(points[0], points.back()));
    for (unsigned i = 0; i < points.size()-1; i++) 
        lines.push_back(line(points[i], points[i + 1]));
    return lines;
}

void subdivide(float tVal) {
    allPoints.insert(allPoints.end(), currentPoints.begin(), currentPoints.end());
    allLines.insert(allLines.end(), currentLines.begin(), currentLines.end());
    subPoints = subdivide(currentPoints, tVal);
    subLines = getLines(subPoints);
}

void dimShapes(vector<point> &points, vector<line> &lines, int minAlpha, float fadeAmount) {
    for (unsigned i = 0; i < lines.size(); i++) {
        sf::Color c = lines[i].rectangle.getFillColor();
        lines[i].rectangle.setFillColor(sf::Color(c.r, c.g, c.b, lerp(c.a, minAlpha, fadeAmount)));
    }
    for (unsigned i = 0; i < points.size(); i++) {
        sf::Color c = points[i].circle.getFillColor();
        points[i].circle.setFillColor(sf::Color(c.r, c.g, c.b, lerp(c.a, minAlpha, fadeAmount)));
    }
}

void showStartMessage() {
    cout << "Press LBM to add points. When ready, press RBM to close the shape "
        "and start\n smoothing. You can adjust amount of smoothing by mouse "
        "scroll. Press RBM again to repeat subdivision, R to reset." << endl;
}

int main()
{
    float smoothingValue = 0.5;
    bool isShapeClosed = false;
    int minAlpha = 31;
    float fadeAmount = 10 / 15.0f;
    float smoothingValueDelta = 0.1f;
    
    showStartMessage();

	window.create(sf::VideoMode(800, 600), "Subdivision smoothing", sf::Style::Close);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
            //change value of t
            if (event.type == sf::Event::MouseWheelMoved)
            {
                smoothingValue += smoothingValueDelta * event.mouseWheel.delta;
                subPoints = subdivide(currentPoints, smoothingValue);
                subLines = getLines(subPoints);
                window.setTitle("Subdivision smoothing, smoothing value: "+to_string(smoothingValue));
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
                isShapeClosed = false;
                smoothingValue = 0.5;
                
            }

            //adding new point
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !isShapeClosed){
				int mouseX = sf::Mouse::getPosition(window).x;
				int mouseY = sf::Mouse::getPosition(window).y;
				addPoint((float)mouseX, (float)mouseY);
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                if (currentPoints.size() > 2 && !isShapeClosed) {
                    //complete the polygon
                    currentLines.push_back(line(currentPoints[0], currentPoints[currentPoints.size() - 1]));
                    allLines.push_back(currentLines.back());

                    subdivide(smoothingValue);
                    dimShapes(allPoints, allLines, minAlpha, fadeAmount);

                    isShapeClosed = true;
                }
                else if (isShapeClosed) {
                    //subdivide again
                    currentPoints = subPoints;
                    currentLines = subLines;
                    subdivide(smoothingValue);
                    dimShapes(allPoints, allLines, minAlpha, fadeAmount);
                }
            }
		}

		refreshWindow();
	}
	return 0;
}
