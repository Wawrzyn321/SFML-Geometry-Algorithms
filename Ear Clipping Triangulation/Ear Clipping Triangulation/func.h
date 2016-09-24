#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <cmath>

using namespace std;

const double pi = 3.1415926535897932384626433832795;

bool mouseCollideText(int mX, int mY, sf::Text text)
{
	if (mX >= text.getPosition().x && mX <= text.getPosition().x + 0.64*text.getCharacterSize()*text.getString().getSize() && mY >= text.getPosition().y && mY <= text.getPosition().y + text.getCharacterSize())
		return true;
	else
		return false;
}

bool mouseCollideCircle(int mX, int mY, sf::CircleShape circle)
{
	if (sqrt((double)pow((circle.getPosition().x - mX), 2) + pow((circle.getPosition().y - mY), 2)) <= circle.getRadius())
		return true;
	else
		return false;
}

bool mouseCollideRect(int mX, int mY, sf::RectangleShape rect)
{
	if (mX >= rect.getPosition().x && mX <= rect.getPosition().x + rect.getSize().x && mY >= rect.getPosition().y && mY <= rect.getPosition().y + rect.getSize().y)
		return true;
	else
		return false;
}

int WAgetTextLenght(sf::Text tekst){
	return (int)(0.64*tekst.getCharacterSize()*tekst.getString().getSize());
}

int WAsetTextCenterX(sf::Text tekst, sf::Window wind){
	return (int)(wind.getSize().x - 0.64*tekst.getCharacterSize()*tekst.getString().getSize()) / 2;
}

float SnR(float katS){
	return (float)(katS*pi / 180);
}
float RnS(float katR){
	return (float)(katR * 180 / pi);
}

int WAsetRectCenterX(sf::RectangleShape rect, sf::Window wind){
	return (int)(wind.getSize().x - rect.getSize().x) / 2;
}
int WAsetRectCenterY(sf::RectangleShape rect, sf::Window wind){
	return (int)(wind.getSize().y - rect.getSize().y) / 2;
}
int geetCurrentTime(sf::Clock clc){
	sf::Time time1 = clc.getElapsedTime();
	return time1.asMilliseconds();
}

std::string its(int x){
	std::ostringstream temp;
	temp << x;
	return temp.str();
}
std::string fts(float x){
	std::ostringstream temp;
	temp << x;
	return temp.str();
}
std::string bts(bool x){
	std::ostringstream temp;
	temp << x;
	return temp.str();
}

std::string dts(double x){
	std::ostringstream temp;
	temp << x;
	return temp.str();
}

float pixToMet(float pix){
	return (float)(pix*0.2);
}
float metToPix(float met){
	return met * 50;
}
sf::Color getRandomColor(){
	int r = rand() % 255;
	int g = rand() % 255;
	int b = rand() % 255;
	//return sf::Color::Red;
	return sf::Color::Color(r, g, b);
}

double odleglosc(float x1, float y1, float x2, float y2){
	return sqrt((double)(x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

double angleBetweenVectors(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4){
	double k;
	double l1 = odleglosc(x1, y1, x2, y2);
	double l2 = odleglosc(x3, y3, x4, y4);
	double dp = (x2 - x1)*(x4 - x3) + (y2 - y1)*(y4 - y3);
	k = dp / l1 / l2;
	return acos(k) * 180 / pi;
}


sf::Color getRandomColorAlpha(){
	int r = rand() % 255;
	int g = rand() % 255;
	int b = rand() % 255;
	int a = rand() % 255;
	return sf::Color::Color(r, g, b, a);
}


int sti(std::string s){
	return atoi(s.c_str());
}

double lerp(double what, double start, double end){
	if (start<end)
		return -1;
	if (what<start)
		return start;
	else if (what>end)
		return end;
	else
		return what;
}

float lerp2(float v0, float v1, float t) {
	return (1 - t)*v0 + t*v1;
}

double area(int x1, int y1, int x2, int y2, int x3, int y3){
	double det = x1*(y2 - y3) + x2*(y3 - y1) + x3*(y1 - y2);
	return abs(det) / 2.0;
}

bool pointInTriangle(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3){
	double utterArea = area(x1, y1, x2, y2, x3, y3);
	double AArea = area(x1, y1, px, py, x3, y3);
	double BArea = area(px, py, x2, y2, x3, y3);
	double CArea = area(x1, y1, x2, y2, px, py);
	if (utterArea == AArea + BArea + CArea)
		return true;
	return false;
}

bool isPointConvex(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, bool isCW){//update!
	float k;
	k = atan2((x2 - x1)*(y4 - y3) - (x4 - x3)*(y2 - y1), (x2 - x1)*(x4 - x3) + (y2 - y1)*(y4 - y3)) * 180 / 3.1415926535897932384626433832795;
	if (isCW)
		return k < 0;
	else
		return k > 0;

}