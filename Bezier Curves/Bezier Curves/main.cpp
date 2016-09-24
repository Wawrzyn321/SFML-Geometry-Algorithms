#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "BezierCurves.h"

using namespace std;

const int fps = 60;
const int refreshRate = 1000 / fps;
int accumulator = 0;

const int MOUSE_COLLISION_DISTANCE = 50;
const int windowSize = 600;
sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Bezier Curves showcase");


line startingLine;
vector<bezierLine> bLines;

void showHelp(){
    cout << "Press LMB to start drawing a line." << endl;
    cout << "Select a line with RMB." << endl;
    cout << "Click RMB and drag one of dots, marked on curve, to move control points." << endl;
    cout << "With curve selected, press Del to delete it." << endl;
    cout << "Press R to reset." << endl;
}

void refreshWindow(){
    window.clear(sf::Color::Black);

    for (size_t i = 0; i < bLines.size(); i++)
        bLines[i].draw(window);

    startingLine.draw(window);
    window.display();
}


int main(){
    srand((unsigned int)time(NULL));

    window.create(sf::VideoMode(windowSize, windowSize), "Bezier Curves showcase", sf::Style::Close);

    int lmX = 0;
    int lmY = 0;
    int sX, sY;
    bool drawing = false;
    bool holdingControlPoint = false;

    bezierLine *mouse_bezier = NULL;
    int choosenPoint = NULL;

    sf::Clock timer;
    timer.restart();

    showHelp();

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();


            //drawing lines
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                int mX = sf::Mouse::getPosition(window).x;
                int mY = sf::Mouse::getPosition(window).y;

                for (size_t i = 0; i < bLines.size(); i++){
                    bLines[i].showControls = false;
                    bLines[i].isChoosen = false;
                }
                drawing = true;
                sX = mX;
                sY = mY;
            }


            //selecting and moving control points
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right){
                int mX = sf::Mouse::getPosition(window).x;
                int mY = sf::Mouse::getPosition(window).y;

                for (size_t i = 0; i < bLines.size() && !holdingControlPoint; i++){
                    if (bLines[i].showControls && !holdingControlPoint){
                        if (squaredDistance(sf::Vector2f(mX, mY), bLines[i].point_p1.getPosition()) < MOUSE_COLLISION_DISTANCE){
                            mouse_bezier = &bLines[i];
                            choosenPoint = 0;
                            holdingControlPoint = true;
                        }
                        else if (squaredDistance(sf::Vector2f(mX, mY), bLines[i].point_c1.getPosition()) < MOUSE_COLLISION_DISTANCE){
                            mouse_bezier = &bLines[i];
                            choosenPoint = 1;
                            holdingControlPoint = true;
                        }
                        else if (squaredDistance(sf::Vector2f(mX, mY), bLines[i].point_c2.getPosition()) < MOUSE_COLLISION_DISTANCE){
                            mouse_bezier = &bLines[i];
                            choosenPoint = 2;
                            holdingControlPoint = true;
                        }
                        else if (squaredDistance(sf::Vector2f(mX, mY), bLines[i].point_p2.getPosition()) < MOUSE_COLLISION_DISTANCE){
                            mouse_bezier = &bLines[i];
                            choosenPoint = 3;
                            holdingControlPoint = true;
                        }
                    }

                    if (!bLines[i].isChoosen)
                        if (bLines[i].getCollision(sf::Vector2f(mX, mY))){
                            for (size_t j = 0; j < bLines.size(); j++){
                                bLines[j].showControls = false;
                                bLines[j].isChoosen = false;
                            }
                            bLines[i].showControls = true;
                            bLines[i].isChoosen = true;
                            break;
                        }
                        else{
                            bLines[i].showControls = false;
                            bLines[i].isChoosen = false;
                        }
                }

                    
            }

            //add a line
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
                int mX = sf::Mouse::getPosition(window).x;
                int mY = sf::Mouse::getPosition(window).y;
                if (drawing){
                    if (!(sX == mX && mY == sY)){
                        bezierLine l = bezierLine(sf::Vector2f(sX, sY), sf::Vector2f(mX, mY));
                        bLines.push_back(l);
                    }
                    //reset startingLine
                    startingLine.setPoints({ -1, -1 }, { -1, -1 });
                    drawing = false;
                }
            }

            
            //RBM
            //if none is pointed, deselect others
            //if mouse points a line, select it (simple...)
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right){
                int mX = sf::Mouse::getPosition(window).x;
                int mY = sf::Mouse::getPosition(window).y;
                if (holdingControlPoint){
                    holdingControlPoint = false;
                    mouse_bezier = NULL;
                    choosenPoint = NULL;
                }

                for (size_t i = 0; i < bLines.size(); i++)
                    if (!bLines[i].isChoosen){
                        bLines[i].isChoosen = false;
                        bLines[i].showControls = false;
                    }
                    else if (!bLines[i].getCollision(sf::Vector2f(mX, mY)) && !bLines[i].getPointsCollision(sf::Vector2f(mX, mY))){
                        bLines[i].isChoosen = false;
                        bLines[i].showControls = false;
                    }
            }

            if (event.MouseMoved){
                int mX = sf::Mouse::getPosition(window).x;
                int mY = sf::Mouse::getPosition(window).y;
                if (drawing)
                    startingLine.setPoints(sf::Vector2f(sX, sY), sf::Vector2f(mX, mY));
                else if (holdingControlPoint)
                    mouse_bezier->setPoint(choosenPoint, sf::Vector2f(mX, mY));
                else{
                    for (size_t i = 0; i < bLines.size(); i++){
                        if (bLines[i].isChoosen){
                            bLines[i].clearDots();

                            //highlighting points
                            if (squaredDistance(sf::Vector2f(mX, mY), bLines[i].point_p1.getPosition()) < MOUSE_COLLISION_DISTANCE)
                                bLines[i].point_p1.circle.setFillColor(sf::Color::Red);

                            if (squaredDistance(sf::Vector2f(mX, mY), bLines[i].point_p2.getPosition()) < MOUSE_COLLISION_DISTANCE)
                                bLines[i].point_p2.circle.setFillColor(sf::Color::Red);

                            if (squaredDistance(sf::Vector2f(mX, mY), bLines[i].point_c1.getPosition()) < MOUSE_COLLISION_DISTANCE)
                                bLines[i].point_c1.circle.setFillColor(sf::Color::Red);

                            if (squaredDistance(sf::Vector2f(mX, mY), bLines[i].point_c2.getPosition()) < MOUSE_COLLISION_DISTANCE)
                                bLines[i].point_c2.circle.setFillColor(sf::Color::Red);

                        }
                        else{
                            if (bLines[i].getCollision(sf::Vector2f(mX, mY))){
                                bLines[i].showControls = true;
                            }
                            else
                                bLines[i].showControls = false;
                        }
                    }
                }
            }

            //reset
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
                bLines.clear();

            //delete selected line
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Delete){
                unsigned int i = 0;
                while (i < bLines.size()){
                    if (bLines[i].isChoosen)
                        bLines.erase(bLines.begin() + i);
                    else
                        i++;
                }
            }

        }

        //handling time
        accumulator += timer.getElapsedTime().asMicroseconds();
        timer.restart();
        if (accumulator >= refreshRate * 1000) {
            refreshWindow();
            accumulator = 0;
        }
    }
    return 0;
}