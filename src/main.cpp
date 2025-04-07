#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils/network.hpp"
#include "layout/toolbar.hpp"
#include "theme.hpp"
#include "layout/app_space.hpp"


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

    std::vector<std::string> tabs = { "Info", "Weather", "Stocks", "Settings" };
    int activeTab = 0; // You can change this with key input, etc.

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
                activeTab = (activeTab - 1 + tabs.size()) % tabs.size();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
                    activeTab = (activeTab + 1) % tabs.size();
            }
        }
        window.clear(Theme::Background);

 		std::string ip = Network::getIp();

        Toolbar::draw(window, font);
        AppSpace::draw(window, font, tabs, activeTab);

        window.display();
    }

    return 0;
}