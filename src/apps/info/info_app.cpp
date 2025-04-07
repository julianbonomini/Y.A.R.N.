#include "info_app.hpp"
#include <SFML/Graphics.hpp>

InfoApp::InfoApp(sf::RenderWindow& window, const sf::Font& font, const std::string& appName)
    : App(window, font, appName), window(window), font(font) {
    // Constructor logic can go here (optional)
}

void InfoApp::draw() {
    // Example: Draw some text in the InfoApp
    sf::Text infoText(font, appName, 24); // Example text
    infoText.setFillColor(sf::Color::Black); // Set text color
    infoText.setPosition({100.f, 100.f}); // Position the text on the window
    // Draw the text
    window.draw(infoText);
}

