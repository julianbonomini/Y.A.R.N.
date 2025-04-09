#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../../globals/theme.hpp"

class App {
public:
    // Constructor to initialize the window, font, and tab name
    App(const std::string &appName)
        : appName(appName) {}

    virtual ~App() = default;

    // Virtual function that all apps must implement
    virtual void draw() = 0;


    bool hasOpenModal = false;
    bool settingsOpen = false;
    bool helpOpen = false;

    // Virtual function that all apps must implement
    virtual void handleEvent(const sf::Event::KeyPressed& keyPressed) = 0;

    // Virtual function that all apps must implement
    virtual void handleSettings() = 0;

    // Virtual function that all apps must implement
    virtual void handleHelp() = 0;

    // Public variables
    std::string appName; // The name of the tab associated with this app

protected:
    // sf::RenderWindow& window;
    // const sf::Font& font;
    const sf::Vector2f TOP_LEFT_ANCHOR = sf::Vector2f(Areas::PADDING * 2, Areas::TOOLBAR_OFFSET + Areas::TAB_HEIGHT + Areas::PADDING * 2);
    const sf::Vector2f TOP_RIGHT_ANCHOR = sf::Vector2f(Areas::WINDOW_WIDTH - Areas::PADDING * 2, Areas::TOOLBAR_OFFSET + Areas::TAB_HEIGHT + Areas::PADDING * 2);
    const sf::Vector2f BOTTOM_LEFT_ANCHOR = sf::Vector2f(Areas::PADDING * 2, Areas::WINDOW_HEIGHT - Areas::FOOTER_HEIGHT - Areas::PADDING - Areas::PADDING * 2);
    const sf::Vector2f BOTTOM_RIGHT_ANCHOR = sf::Vector2f(Areas::WINDOW_WIDTH - Areas::PADDING * 2, Areas::WINDOW_HEIGHT - Areas::PADDING * 2);


    const sf::Vector2f TOP_LEFT_MODAL_ANCHOR = sf::Vector2f( (Areas::WINDOW_WIDTH - Areas::MODAL_WIDTH) / 2, (Areas::WINDOW_HEIGHT - Areas::MODAL_HEIGHT) / 2);

    sf::RectangleShape getModalRectangle();

    sf::FloatRect getGridBox(int colStart, int rowStart, int colSpan, int rowSpan);
};
