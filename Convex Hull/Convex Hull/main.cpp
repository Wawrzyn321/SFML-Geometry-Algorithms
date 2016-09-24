#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

using namespace std;

const int fps = 60;
const int refreshRate = 1000 / fps;
int accumulator = 0;

const int MOUSE_COLLISION_DISTANCE = 50;
const int windowSize = 600;
sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "");

void refreshWindow() {
    window.clear(sf::Color::Black);

    window.display();
}


int main() {
    srand((unsigned int)time(NULL));

    window.create(sf::VideoMode(windowSize, windowSize), "", sf::Style::Close);

    sf::Clock timer;
    timer.restart();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

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