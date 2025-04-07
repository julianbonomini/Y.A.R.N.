#include "config_app.hpp"

#include<iostream>
#include <SFML/Graphics.hpp>
#include <string>

ConfigApp::ConfigApp(sf::RenderWindow& window, const sf::Font& font, const std::string& appName)
    : App(window, font, appName), window(window), font(font) {
    configLabels.push_back("Test:");
}


void ConfigApp::draw() {

    float labelPositionX = TOP_LEFT.x + Theme::PADDING;
    float valuePositionX = TOP_LEFT.x + Theme::PADDING + 150.f;
    float verticalOffset = Theme::PADDING;
    float labelSpacing = 15.f;

    for (const auto& entry : configLabels) {
        sf::Text labelText(font, entry);
        labelText.setPosition({labelPositionX, TOP_LEFT.y + verticalOffset});
        labelText.setFillColor(Colors::Text);
        labelText.setCharacterSize(TextSizes::LABEL);

        // Draw label and value
        window.draw(labelText);

        // Increase the vertical offset for the next pair
        verticalOffset += labelSpacing; // Adjust spacing between each pair
    }
}

