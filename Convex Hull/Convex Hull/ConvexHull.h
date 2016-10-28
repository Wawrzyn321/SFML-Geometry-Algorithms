#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include <SFML\Graphics.hpp>
#include <vector>
#include "Shapes.h"
#include "utility.h"

using namespace std;

point *getLeftmostPoint(vector<point> &v, float maxX);

point *getRightmostPoint(vector<point> &v);

vector<point*> getPointsOnRight(vector<point> &v, point *vL, point *vP);

vector<point*> getPointsOnRight(vector<point*> v, point *vL, point *vP);

point *getFarthestPoint(LineEquation lineEquation, vector<point*> v);

vector<point*> QuickHullSub(point A, point B, vector<point*> points);

vector<point*> QuickHull(vector<point> points);


#endif