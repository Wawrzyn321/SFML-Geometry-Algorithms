#ifndef  UTILITY_H
#define UTILITY_H


#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <cmath>

using namespace std;

class point;
const double pi = 3.1415926535897932384626433832795;
const sf::Color niceBlue = sf::Color(100, 100, 255);

double distance(float x1, float y1, float x2, float y2);

double distance(sf::Vector2f v1, sf::Vector2f v2);


double triangleArea(sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f v3);

bool isPointInTriangle(sf::Vector2f v, sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f v3);


double angleBetweenVectors(sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f v3, sf::Vector2f v4);

double angleBetweenPoints(sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f v3);


#endif // ! UTILITY_H