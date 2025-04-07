#include<iostream>
#include "info_app.hpp"
#include <SFML/Graphics.hpp>
#include <string>

InfoApp::InfoApp(sf::RenderWindow &window, const sf::Font &font, const std::string &appName)
    : App(appName), window(window), font(font) {
    infoData.push_back({"VERSION:", "Ostrich 0.0.1"});
    infoData.push_back({"CPU:", "Intel Core i7 3.6 GHz"});
    infoData.push_back({"RAM:", "16GB"});
    infoData.push_back({"USED STORAGE:", "100GB of 500GB"});
    infoData.push_back({"APPS:", "7"});
    infoData.push_back({"OS:", "Ubuntu 20.04"});
    infoData.push_back({"UPTIME:", "12 days, 4 hours"});
    infoData.push_back({"NETWORK:", "Ethernet: Connected"});
}

void InfoApp::handleEvent(const sf::Event::KeyPressed &keyPressed) {
}


void InfoApp::draw() {
    float labelPositionX = TOP_LEFT.x + Theme::PADDING;
    float valuePositionX = TOP_LEFT.x + Theme::PADDING + 150.f;
    float verticalOffset = Theme::PADDING;
    float labelSpacing = 15.f;

    // Iterate over all key-value pairs
    for (const auto &entry: infoData) {
        sf::Text labelText(font, entry.first);
        labelText.setPosition({labelPositionX, TOP_LEFT.y + verticalOffset});
        labelText.setFillColor(Colors::Text);
        labelText.setCharacterSize(TextSizes::LABEL);

        sf::Text valueText(font, entry.second);
        valueText.setPosition({valuePositionX, TOP_LEFT.y + verticalOffset});
        valueText.setFillColor(Colors::Text);
        valueText.setCharacterSize(TextSizes::LABEL);

        // Draw label and value
        window.draw(labelText);
        window.draw(valueText);

        // Increase the vertical offset for the next pair
        verticalOffset += labelSpacing; // Adjust spacing between each pair
    }
}
