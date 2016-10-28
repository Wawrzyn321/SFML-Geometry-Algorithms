#include "ConvexHull.h"

//get pointer to point of lowest x coordinate
point *getLeftmostPoint(vector<point> &v, float maxX) {
    point *p = NULL;
    float mX = maxX;
    for (unsigned i = 0; i < v.size(); i++) {
        if (v[i].x < mX) {
            mX = v[i].x;
            p = &v[i];
        }
    }
    return p;
}

//get pointer to point of highest x coordinate
point *getRightmostPoint(vector<point> &v) {
    point *p = NULL;
    float mX = 0;
    for (unsigned i = 0; i < v.size(); i++) {
        if (v[i].x > mX) {
            mX = v[i].x;
            p = &v[i];
        }
    }
    return p;
}

//get vector of points on the right of two given points
vector<point*> getPointsOnRight(vector<point> &v, point *vL, point *vP) {
    vector<point*> r;
    for (unsigned i = 0; i < v.size(); i++) {
        if (getSide(v[i].getPosition(), vL->getPosition(), vP->getPosition()) > 0) {
            r.push_back(&v[i]);
        }
    }
    return r;
}

//get vector of points on the right of two given points
vector<point*> getPointsOnRight(vector<point*> v, point *vL, point *vP) {
    vector<point*> r;
    for (unsigned i = 0; i < v.size(); i++) {
        if (getSide(v[i]->getPosition(), vL->getPosition(), vP->getPosition()) > 0) {
            r.push_back(v[i]);
        }
    }
    return r;
}

//get point located the farthest from line of given equation
point *getFarthestPoint(LineEquation lineEquation, vector<point*> v) {
    double len = 0;
    point *r = NULL;
    for (unsigned i = 0; i < v.size(); i++) {
        double l =
            abs(lineEquation.x*v[i]->x + lineEquation.y*v[i]->y + lineEquation.z)
            / sqrt(lineEquation.x*lineEquation.x + lineEquation.y*lineEquation.y);
        if (l > len) {
            len = l;
            r = v[i];
        }
    }
    return r;
}

//recurrent subroutine to {QuickHull}
vector<point*> QuickHullSub(point *A, point *B, vector<point*> points)
{
    point *C = getFarthestPoint(getLineEquation(A->getPosition(), B->getPosition()), points);
    if (!C)
        return vector<point*>();

    vector<point*> s1, s2, arg1, arg3;
    s1 = getPointsOnRight(points, A, C);
    s2 = getPointsOnRight(points, C, B);

    arg1 = QuickHullSub(A, C, s1);
    arg3 = QuickHullSub(C, B, s2);

    //QHS(A,C,s1) + C + QHS(C,B,s2)
    vector<point*> vec;
    vec.insert(vec.end(), arg1.begin(), arg1.end());
    vec.push_back(C);
    vec.insert(vec.end(), arg3.begin(), arg3.end());
    return vec;
}

//Quick Hull main
vector<point*> QuickHull(vector<point> points) {
    if (points.size() <=3) {
        return vector<point*>();
    }

    //to make things easier, {maxX} is a constant here
    point *A = getLeftmostPoint(points, 800);
    point *B = getRightmostPoint(points);

    //to get points on left of AB, just get points on right of BA!
    vector<point*> s1 = getPointsOnRight(points, A, B);
    vector<point*> s2 = getPointsOnRight(points, B, A);

    vector<point*> arg2 = QuickHullSub(A, B, s1);
    vector<point*> arg4 = QuickHullSub(B, A, s2);

    //A + QHS(A,B,s1) + B + QHS(B,A,s2)
    vector<point*> vec;
    vec.push_back(A);
    vec.insert(vec.end(), arg2.begin(), arg2.end());
    vec.push_back(B);
    vec.insert(vec.end(), arg4.begin(), arg4.end());
    return vec;
}
