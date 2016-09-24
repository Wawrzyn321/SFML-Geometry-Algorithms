#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "func.h"
#include <vector>

#pragma warning (disable:4244 4700 4305 C4018)//how it works?

using namespace std;

bool ifcan = true;
sf::RenderWindow window(sf::VideoMode(800, 600), "triangulation");
int delay = 750;

string whatsOn = "none";
bool sound = true;



class point{
public:
	sf::CircleShape circle;
	int x, y;
	point(int ix, int iy){
		x = ix;
		y = iy;
		circle = sf::CircleShape(4, 10);
		circle.setOrigin(circle.getRadius(), circle.getRadius());
		circle.setPosition(x, y);
		circle.setFillColor(sf::Color(100, 100, 255));
	}
};

class line{
public:
	sf::RectangleShape rectangle;
	int x, y;
	line(int x1, int y1, int x2, int y2){
		x = (x1 + x2) / 2;
		y = (y1 + y2) / 2;
		rectangle = sf::RectangleShape(sf::Vector2f(3, odleglosc(x1, y1, x2, y2)));
		rectangle.setOrigin(rectangle.getSize().x / 2, rectangle.getSize().y / 2);
		double roto = -atan((double)(x1 - x2) / (y1 - y2));
		rectangle.setRotation(0);
		rectangle.rotate(roto * 180 / pi);
		rectangle.setPosition(x, y);
		rectangle.setFillColor(sf::Color(100, 100, 255));
	}
};

class triangle{
public:
	sf::ConvexShape tri;
	triangle(int x1, int y1, int x2, int y2, int x3, int y3){
		tri.setPointCount(3);
		tri.setPoint(0, sf::Vector2f(x1, y1));
		tri.setPoint(1, sf::Vector2f(x2, y2));
		tri.setPoint(2, sf::Vector2f(x3, y3));
	}
};

vector<point> points;
vector<line> lines;
vector<triangle> triangles;

void refreshWindow(){
	window.clear();
	for (int i = 0; i < triangles.size(); i++)
		window.draw(triangles[i].tri);
	for (int i = 0; i < lines.size(); i++)
		window.draw(lines[i].rectangle);
	for (int i = 0; i < points.size(); i++)
		window.draw(points[i].circle);
	window.display();
}

double angleBetweenPoints(int ind1, int ind2, int ind3){	//usable only in this project
	return angleBetweenVectors(points[ind2].x, points[ind2].y, points[ind1].x, points[ind1].y, points[ind2].x, points[ind2].y, points[ind3].x, points[ind3].y);
}

bool areVecsConvex(int ind1, int ind2, int ind3){ //usable only in this project
	return isPointConvex(points[ind2].x, points[ind2].y, points[ind1].x, points[ind1].y, points[ind2].x, points[ind2].y, points[ind3].x, points[ind3].y, 1);
}

bool isPointConvex(int ind, bool isCW){//usable only in this project
	int kp = ind - 1;
	int kn = ind + 1;
	kp %= points.size();
	kn %= points.size();
	//cout << "checking points " << kp << " " << ind << " " << kn << endl;
	int x1 = points[kp].x;
	int y1 = points[kp].y;
	int x2 = points[ind].x;
	int y2 = points[ind].y;
	int x3 = x2;
	int y3 = y2;
	int x4 = points[kn].x;
	int y4 = points[kn].y;
	float k;
	k = atan2((x2 - x1)*(y4 - y3) - (x4 - x3)*(y2 - y1), (x2 - x1)*(x4 - x3) + (y2 - y1)*(y4 - y3)) * 180 / 3.1415926535897932384626433832795;
	if (isCW)
		return k >= 0;
	else
		return k < 0;
}

bool isClockWise(){//usable only in this project
	float sum = 0;
	for (int i = 0; i < points.size(); i++){
		int k1 = i + 1;
		int k2 = i + 2;
		if (k1 >= points.size()) k1 %= points.size();
		if (k2 >= points.size()) k2 %= points.size();
		if (areVecsConvex(i, k1, k2)){
			sum += angleBetweenPoints(i, k1, k2);
			//cout << angleBetweenPoints(i, k1, k2) << endl;
		}
		else{
			sum += 360 - angleBetweenPoints(i, k1, k2);
			//cout << 360 - angleBetweenPoints(i, k1, k2) << " +360!" << endl;
		}
	}
	//cout<<endl << sum << endl;
	if (round(sum) == 180 * (points.size() - 2))
		return true;
	else
		return false;
}

bool arePointsConvex(int v1, int v2, int v3){//usable only in this project
	float sum = 0;
	if (areVecsConvex(v3, v1, v2))
		sum += angleBetweenPoints(v3, v1, v2);
	else
		sum += 360 - angleBetweenPoints(v3, v1, v2);
	if (areVecsConvex(v1, v2, v3))
		sum += angleBetweenPoints(v1, v2, v3);
	else
		sum += 360 - angleBetweenPoints(v1, v2, v3);
	if (areVecsConvex(v2, v3, v1))
		sum += angleBetweenPoints(v2, v3, v1);
	else
		sum += 360 - angleBetweenPoints(v2, v3, v1);
	//cout<<endl << sum << endl;
	if (round(sum) == 180)
		return true;
	else
		return false;
}

void addPoint(int lX, int lY, int nX, int nY){
	for (int i = 0; i < points.size(); i++)	//don't duplicate points!
		if (nX == points[i].x && nY == points[i].y)
			return;
	points.push_back(point(nX, nY));
	if (!(lX == lY) && lX != -1)	//draw line, if we have more than 1 point
		lines.push_back(line(lX, lY, nX, nY));
}

void closeShape(){
	lines.push_back(line(points[0].x, points[0].y, points[points.size() - 1].x, points[points.size() - 1].y));
	refreshWindow();
	vector<point> verts = points;
	bool cw = isClockWise();
    delay = 0;
	/*
	if (cw)
		cout << "shape was drew clockwise" << endl;
	else
		cout << "shape was drew counter clocwise" << endl;
	*/
	while (verts.size() > 2){
		bool enf = true;
        int s = 0;
        int t = 0;
		while (enf){
            t++;
            if (verts.size() - 2 < t)
                cout << "cos sie spierdolilo" << endl;
			s %= verts.size();
			int m = (s + 1) % verts.size();
			int e = (s + 2) % verts.size();
			int xS = verts[s].x;
			int yS = verts[s].y;
			int xM = verts[m].x;
			int yM = verts[m].y;
			int xE = verts[e].x;
			int yE = verts[e].y;
			/*
			cout << "checked verts:" << endl;
			cout << xS << ", " << yS << endl;
			cout << xM << ", " << yM << endl;
			cout << xE << ", " << yE << endl;
			*/
			triangles.push_back(triangle(xS, yS, xM, yM, xE, yE));
			bool isConvex = false;
			if (isPointConvex(xS, yS, xM, yM, xM, yM, xE, yE, !cw) && isPointConvex(xM, yM, xE, yE, xE, yE, xS, yS, !cw) && isPointConvex(xE, yE, xS, yS, xS, yS, xM, yM, !cw))
				isConvex = true;
			if (isConvex){
				triangles[triangles.size() - 1].tri.setFillColor(sf::Color::Green);
				//cout << "is convex" << endl;
			}
			else{
				triangles[triangles.size() - 1].tri.setFillColor(sf::Color::Red);
				//cout << "not convex" << endl;
				s++;
			}
			bool noPointsIn = true;
			if (isConvex){
				for (int i = 0; i < verts.size(); i++)
					if (i != s && i != m && i != e)
						if (pointInTriangle(verts[i].x, verts[i].y, xS, yS, xM, yM, xE, yE) && noPointsIn)
							noPointsIn = false;
				if (noPointsIn){
					//cout << "no points in triangle" << endl;
					enf = false;
				}
				else{
					triangles[triangles.size() - 1].tri.setFillColor(sf::Color::Red);
					//cout << "point in triangle" << endl;
					s++;
				}
			}
			refreshWindow();
			Sleep(delay);
			if (!noPointsIn || !isConvex){
				triangles.erase(triangles.begin() + triangles.size() - 1);
				//cout << "bad, erasing triangle" << endl;
			}
			else{
				//cout << "ok, drawing triangle" << endl;
				//cout << "deleting vert "<<m<<" at " <<verts[m].x<<", "<<verts[m].y<< endl;
				verts.erase(verts.begin() + m);
				triangles[triangles.size() - 1].tri.setFillColor(sf::Color::White);
				//refreshWindow();
				//Sleep(delay);
			}
		}
	}
}

bool ccwOrOn(double ax, double ay, double bx, double by, double cx, double cy){
	return (by - ay)*(cx - ax) <= (cy - ay)*(bx - ax);
}

bool intersection2(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4){
	if (ccwOrOn(x1, y1, x2, y2, x3, y3) == ccwOrOn(x1, y1, x2, y2, x4, y4))
		return false;
	if (ccwOrOn(x3, y3, x4, y4, x1, y1) == ccwOrOn(x3, y3, x4, y4, x2, y2))
		return false;
	return true;
}

bool intersection(int ax, int ay, int bx, int by, int cx, int cy, int dx, int dy){
	int ex, ey, fx, fy, px, py;
	ex = bx - ax;
	ey = by - ay;
	fx = dx - cx;
	fy = dy - cy;
	px = -ey;
	py = ex;
	int abcx = ax - cx;
	int abcy = ay - cy;
	int dacp = abcx*px + abcy*py;
	float dfp = fx*px + fy*py;
	cout << (dacp / dfp) << endl;
	/*
	if(dfp==0)
	return h = -1;
	else
	return dacp/dfp;*/
	return dacp / dfp > 0 && dacp / dfp < 1;
}


int main()
{
	int lX = -1, lY = -1;

	window.create(sf::VideoMode(800, 600), "triangulation (s - sound on/off, r - reset)", sf::Style::Resize | sf::Style::Close);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
				window.close();
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R){
				ifcan = true;
				cout.clear();
				points.clear();
				lines.clear();
				triangles.clear();
				lX = -1;
				lY = -1;
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && ifcan){
				int myszX = sf::Mouse::getPosition(window).x;
				int myszY = sf::Mouse::getPosition(window).y;
				bool inte = false;
				if (points.size() > 2)
					for (int i = 0; i < points.size() - 2; i++)
						if (intersection2(points[points.size() - 1].x, points[points.size() - 1].y, myszX, myszY, points[i].x, points[i].y, points[i + 1].x, points[i + 1].y)){
							//cout << "!"<<endl;
							inte = true;
							break;
						}
				if (inte)
					cout << "indicated point makes intersecting line." << endl;
				else{
					addPoint(lX, lY, myszX, myszY);
					lX = myszX;
					lY = myszY;
				}
				/*
				if (points.size() >= 3)
				if (isPointConvex(points.size() - 2))
				cout << angleBetweenPoints(points.size() - 1, points.size() - 2, points.size() - 3) << " " << isPointConvex(points.size() - 2) << endl;
				else
				cout << 360-angleBetweenPoints(points.size() - 1, points.size() - 2, points.size() - 3) << " " << isPointConvex(points.size() - 2) << endl;
				*/
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && points.size() > 2 && ifcan){
				bool inte = false;
				for (int i = 0; i < points.size() - 2; i++)
					if (intersection2(points[0].x, points[0].y, points[points.size() - 1].x, points[points.size() - 1].y, points[i].x, points[i].y, points[i + 1].x, points[i + 1].y)){
						//cout << "!"<<endl;
						inte = true;
						break;
					}
				if (!inte){
					int oldelay = delay;
					delay = 750;
					if (points.size() > 15){
						delay = delay - points.size() * 25;
						if (delay < 10)
							delay = 10;
						if (delay >= 500)
							delay = 500;
					}
					closeShape();
					lX = -1;
					lY = -1;
					ifcan = false;
				}
				else
					cout << "cannot close shape: indicated point makes intersecting line." << endl;
			}


		}

		refreshWindow();
	}
	return 0;
}
