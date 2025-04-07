#include "toolbar.hpp"
#include <SFML/Graphics.hpp>
#include <ctime>
#include "../theme/theme.hpp"
#include <iostream>

Toolbar::Toolbar(sf::RenderWindow &window, const sf::Font &font)
    : window(window), font(font) {
}


void Toolbar::draw() {
    sf::Text leftText(font, "Ostrich");
    leftText.setCharacterSize(12);
    leftText.setFillColor(Theme::Text);
    leftText.setPosition({10.f, 10.f});

    // Get current date and time
    time_t now = std::time(nullptr);

    char timeStr[64];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    sf::Text rightText(font, timeStr);
    rightText.setCharacterSize(12);
    rightText.setFillColor(Theme::Text);
    rightText.setPosition({window.getSize().x - 145.f, 10.f});  // Position the text
    // Draw a thin horizontal line below the text
    sf::RectangleShape line(sf::Vector2f(window.getSize().x, 1.f)); // Full-width line, 1 pixel tall
    line.setFillColor(Theme::Text); // Line color (same as text color)
    line.setPosition({0.f, 32.f});    // Position the line below the text
    window.draw(line);  // Draw the line

    window.draw(leftText);
    window.draw(rightText);
}

