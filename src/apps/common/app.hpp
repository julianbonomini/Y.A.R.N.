#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../../globals/theme.hpp"

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
    sf::RenderWindow& window;
    const sf::Font& font;
    const sf::Vector2f TOP_LEFT = sf::Vector2f(Theme::PADDING, Theme::TOOLBAR_OFFSET + Theme::TAB_HEIGHT + Theme::PADDING);
    const sf::Vector2f TOP_RIGHT = sf::Vector2f(Theme::WINDOW_WIDTH - Theme::PADDING, Theme::TOOLBAR_OFFSET + Theme::TAB_HEIGHT + Theme::PADDING);
    const sf::Vector2f BOTTOM_LEFT = sf::Vector2f(Theme::PADDING, Theme::WINDOW_HEIGHT - Theme::PADDING);
    const sf::Vector2f BOTTOM_RIGHT = sf::Vector2f(Theme::WINDOW_WIDTH - Theme::PADDING, Theme::WINDOW_HEIGHT - Theme::PADDING);
};
