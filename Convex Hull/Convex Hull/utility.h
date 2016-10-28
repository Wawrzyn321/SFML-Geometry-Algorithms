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

typedef sf::Vector3f LineEquation;

double distance(float x1, float y1, float x2, float y2);

LineEquation getLineEquation(sf::Vector2f a, sf::Vector2f b);

double getSide(sf::Vector2f v, sf::Vector2f v1, sf::Vector2f v2);

#endif // ! UTILITY_H