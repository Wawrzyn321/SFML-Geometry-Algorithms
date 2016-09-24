#include "BezierCurves.h"


sf::Vector2f calculateBezierPoint(double t, sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3){
    //http://www.devmag.org.za/downloads/bezier_curves/BezierPath.cs
    double u = 1 - t;
    double tt = t*t;
    double uu = u*u;
    double uuu = uu * u;
    double ttt = tt * t;

    sf::Vector2f p = uuu * p0;
    p += 3 * uu * t * p1;
    p += 3 * u * tt * p2;
    p += ttt * p3;
    return p;
}

void bezierR(vector<sf::Vector2f> &vec, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, int &pos, double t0, double t1){
    double mid = (t0 + t1) / 2.0;
    sf::Vector2f newPoint = calculateBezierPoint(mid, p1, p2, p3, p4);
    vec.insert(vec.begin() + pos, newPoint);
    if (squaredDistance(vec[pos - 1], newPoint) > THRESHOLD_squared && abs(t0 - mid) > 0.01){
        bezierR(vec, p1, p2, p3, p4, pos, t0, mid);
        pos++;
    }
    if (squaredDistance(vec[pos + 1], newPoint) > THRESHOLD_squared && abs(t1 - mid)>0.01){
        pos++;
        bezierR(vec, p1, p2, p3, p4, pos, mid, t1);
    }
}

void startBezierCurve(vector<sf::Vector2f> &vec, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4){
    vec.clear();

    sf::Vector2f n1 = calculateBezierPoint(0, p1, p2, p3, p4);
    sf::Vector2f n2 = calculateBezierPoint(1, p1, p2, p3, p4);

    vec.push_back(n1);
    vec.push_back(n2);

    int pos = 1;
    bezierR(vec, p1, p2, p3, p4, pos, 0, 1);
}


bezierLine::bezierLine(sf::Vector2f _p1, sf::Vector2f _p2){
        p1 = _p1;
        p2 = _p2;
        c1 = p1 + (p2 - p1) / 3.0;
        c2 = p1 + (p2 - p1) * 2 / 3.0;

        point_p1 = point(p1);
        point_p2 = point(p2);
        point_c1 = point(c1);
        point_c2 = point(c2);

        line_control1 = line(p1, c1);
        line_control2 = line(p2, c2);

        segments.clear();
        startBezierCurve(poses, p1, c1, c2, p2);
        for (size_t i = 0; i < poses.size() - 1; i++)
            segments.push_back(line(poses[i], poses[i + 1]));
        showControls = false;
    }
bezierLine::bezierLine(sf::Vector2f _p1, sf::Vector2f _c1, sf::Vector2f _c2, sf::Vector2f _p2){
        p1 = _p1;
        p2 = _p2;
        c1 = _c1;
        c2 = _c2;

        point_p1 = point(p1);
        point_p2 = point(p2);
        point_c1 = point(c1);
        point_c2 = point(c2);

        line_control1 = line(p1, c1);
        line_control2 = line(p2, c2);

        segments.clear();
        startBezierCurve(poses, p1, c1, c2, p2);
        for (size_t i = 0; i < poses.size() - 1; i++)
            segments.push_back(line(poses[i], poses[i + 1]));
    }

bool bezierLine::setPoint(int pointID, sf::Vector2f coords){
        switch (pointID){
        case 0:
            p1 = coords;
            point_p1 = point(p1);
            line_control1 = line(p1, c1);
            break;
        case 1:
            c1 = coords;
            point_c1 = point(c1);
            line_control1 = line(p1, c1);
            break;
        case 2:
            c2 = coords;
            point_c2 = point(c2);
            line_control2 = line(p2, c2);
            break;
        case 3:
            p2 = coords;
            point_p2 = point(p2);
            line_control2 = line(p2, c2);
            break;
        default:
            return false;
        }

        segments.clear();
        startBezierCurve(poses, p1, c1, c2, p2);
        for (size_t i = 0; i < poses.size() - 1; i++)
            segments.push_back(line(poses[i], poses[i + 1]));
        return true;
    }

void bezierLine::draw(sf::RenderWindow &wind){
        if (showControls || isChoosen){
            point_p1.draw(wind);
            point_p2.draw(wind);
            point_c1.draw(wind);
            point_c2.draw(wind);
            line_control1.draw(wind);
            line_control2.draw(wind);
        }
        for (unsigned int i = 0; i < segments.size(); i++)
            segments[i].draw(wind);
    }

bool bezierLine::getCollision(sf::Vector2f pos){
        for (size_t i = 0; i < segments.size() - 1; i++){
            double len = pointLineDistance(pos.x, pos.y, segments[i].x, segments[i].y, segments[i + 1].x, segments[i + 1].y);
            if (len<10)
                return true;
        }
        return false;
    }

bool bezierLine::getPointsCollision(sf::Vector2f pos){
        if (squaredDistance(p1, pos) < 60 || squaredDistance(p2, pos) < 60 || squaredDistance(c1, pos) < 60 || squaredDistance(c2, pos) < 60)
            return true;
        else
            return false;
    }

void bezierLine::clearDots(){
        point_p1.circle.setFillColor(niceBlue);
        point_p2.circle.setFillColor(niceBlue);
        point_c1.circle.setFillColor(niceBlue);
        point_c2.circle.setFillColor(niceBlue);
    }
