#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../../globals/theme.hpp"

class App {
public:
    // Constructor to initialize the window, font, and tab name
    App(const std::string &appName)
        : appName(appName) {}

    virtual void handleEvent(const sf::Event::KeyPressed& keyPressed) = 0;

    virtual ~App() = default;

    // Virtual function that all apps must implement
    virtual void draw() = 0;

    // Public variables
    std::string appName; // The name of the tab associated with this app

protected:
    // sf::RenderWindow& window;
    // const sf::Font& font;
    const sf::Vector2f TOP_LEFT = sf::Vector2f(Areas::PADDING, Areas::TOOLBAR_OFFSET + Areas::TAB_HEIGHT + Areas::PADDING);
    const sf::Vector2f TOP_RIGHT = sf::Vector2f(Areas::WINDOW_WIDTH - Areas::PADDING, Areas::TOOLBAR_OFFSET + Areas::TAB_HEIGHT + Areas::PADDING);
    const sf::Vector2f BOTTOM_LEFT = sf::Vector2f(Areas::PADDING, Areas::WINDOW_HEIGHT - Areas::HELP_BAR_HEIGHT - Areas::PADDING - Areas::PADDING);
    const sf::Vector2f BOTTOM_RIGHT = sf::Vector2f(Areas::WINDOW_WIDTH - Areas::PADDING, Areas::WINDOW_HEIGHT - Areas::PADDING);
};
