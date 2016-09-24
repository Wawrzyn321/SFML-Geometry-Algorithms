#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#ifndef GEOMETRY_H
#define GEOMETRY_H

const double pi = 3.1415926535897932384626433832795;

double distance(float x1, float y1, float x2, float y2);

double distance(sf::Vector3f eq, sf::Vector2f p);

double squaredDistance(sf::Vector2f p1, sf::Vector2f p2);

double dotProduct(sf::Vector2f a, sf::Vector2f b);

sf::Vector3f getStandardLineEquation(sf::Vector2f a, sf::Vector2f b);

double angleBetweenPoints(sf::Vector2f p1, sf::Vector2f p2);

void normalize(float& angle);

sf::Vector2f operator/(const sf::Vector2f &v1, float n);

sf::Vector2f operator*(float n, const sf::Vector2f &v1);

sf::Vector2f operator*(const sf::Vector2f &v1, float n);

double pointLineDistance(float xP, float yP, float x1, float y1, float x2, float y2);

#endif
