#include <iostream>  // Include the input/output library
#include <SFML/Graphics.hpp>  // Include the SFML graphics module
#include "utils/network.hpp"
#include "layout/toolbar.hpp"
#include "theme.hpp"


int main() {
    std::cout << "Booting..." << std::endl;

    auto window = sf::RenderWindow(sf::VideoMode({800, 480}), "TickPi", sf::Style::Default, sf::State::Windowed);
    // sf::State::Fullscreen for full screen
    window.setFramerateLimit(120);
    sf::Font font;
    if (!font.openFromFile("./assets/fonts/FiraCodeNerdFont-Medium.ttf")) {
        std::cout << "Error loading default font" << std::endl;
        return 1;
    }
    sf::Text text(font);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        window.clear(Theme::Background);

 		std::string ip = Network::getIp();

        Toolbar::draw(window, font);

        window.display();
    }

    return 0;
}