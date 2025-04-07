#include<iostream>
#include "info_app.hpp"
#include <SFML/Graphics.hpp>
#include "../../globals/theme.hpp"

InfoApp::InfoApp(sf::RenderWindow& window, const sf::Font& font, const std::string& appName)
    : App(window, font, appName), window(window), font(font) {
    // Constructor logic can go here (optional)
}

void InfoApp::draw() {
    // Create a rectangle that fills the whole view
    sf::RectangleShape backgroundRect({Theme::MAIN_APP_WIDTH, Theme::MAIN_APP_HEIGHT});
    backgroundRect.setFillColor(Theme::Accent);

    backgroundRect.setPosition(TOP_LEFT);

    // Draw it
    window.draw(backgroundRect);
}

