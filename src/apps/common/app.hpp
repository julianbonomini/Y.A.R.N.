#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class App {
public:
    // Constructor to initialize the window, font, and tab name
    App(sf::RenderWindow& window, const sf::Font& font, const std::string &appName)
        : window(window), font(font), appName(appName) {}

    virtual ~App() = default;

    // Virtual function that all apps must implement
    virtual void draw() = 0;

    // Public variables
    std::string appName; // The name of the tab associated with this app

protected:
    sf::RenderWindow& window;  // Reference to the SFML render window
    const sf::Font& font;      // Reference to the font
};
