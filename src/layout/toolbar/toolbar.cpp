#include "toolbar.hpp"
#include <SFML/Graphics.hpp>
#include <ctime>
#include "../../globals/theme.hpp"
#include <iostream>

Toolbar::Toolbar(sf::RenderWindow &window, const sf::Font &font)
    : window(window), font(font) {
}


void Toolbar::draw() {
    sf::Text leftText(font, "Ostrich");
    leftText.setCharacterSize(12);
    leftText.setFillColor(Colors::Text);
    leftText.setPosition({10.f, 10.f});

    // Get current date and time
    time_t now = std::time(nullptr);

    char timeStr[64];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    sf::Text rightText(font, timeStr);
    rightText.setCharacterSize(12);
    rightText.setFillColor(Colors::Text);
    rightText.setPosition({window.getSize().x - 145.f, 10.f});
    // Draw a thin horizontal line below the text
    sf::RectangleShape line(sf::Vector2f(window.getSize().x, 1.f));
    line.setFillColor(Colors::Text);
    line.setPosition({0.f, 32.f});
    window.draw(line);

    window.draw(leftText);
    window.draw(rightText);
}
