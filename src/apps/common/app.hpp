#pragma once
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <string>
#include "../../globals/ui_globals.hpp"

class StateMachine;

class App {
public:
    // Constructor to initialize the window, font, and tab name
    App(const std::string &appName, sf::RenderTarget &renderer, const sf::Font &font)
        : appName(appName), renderer(renderer), font(font) {
    }

    virtual ~App() = default;

    virtual bool getHasOpenModal() const {
        return hasOpenModal;
    }

    virtual bool isSettingsOpen() const {
        return settingsOpen;
    }

    virtual bool isHelpOpen() const {
        return helpOpen;
    }

    virtual void setHasOpenModal(bool value) {
        hasOpenModal = value;
    }

    virtual void setSettingsOpen(bool value) {
        settingsOpen = value;
    }

    virtual void setHelpOpen(bool value) {
        helpOpen = value;
    }

    template<typename... Args>
    void app_log(Args&&... args) {
        std::ostringstream oss;
        oss << appName << "_LOG: ";
        ((oss << args << " "), ...);
        std::cout << oss.str() << std::endl;
    }

    // Virtual function that all apps must implement
    virtual void draw() = 0;

    // Virtual function that all apps must implement
    virtual void handleEvent(const sf::Event::KeyPressed &keyPressed) = 0;

    // Virtual function that all apps must implement
    virtual void handleSettings() = 0;

    // Virtual function that all apps must implement
    virtual void handleHelp() = 0;

    // Public variables
    std::string appName; // The name of the tab associated with this app

protected:
    sf::RenderTarget &renderer;
    const sf::Font &font;

    bool hasOpenModal = false;
    bool settingsOpen = false;
    bool helpOpen = false;

    const sf::Vector2f TOP_LEFT_ANCHOR = sf::Vector2f(Layout::PADDING * 2, Layout::TOOLBAR_OFFSET + Layout::TAB_HEIGHT + Layout::PADDING * 2);
    const sf::Vector2f TOP_RIGHT_ANCHOR = sf::Vector2f(Layout::WINDOW_WIDTH - Layout::PADDING * 2, Layout::TOOLBAR_OFFSET + Layout::TAB_HEIGHT + Layout::PADDING * 2);
    const sf::Vector2f BOTTOM_LEFT_ANCHOR = sf::Vector2f(Layout::PADDING * 2, Layout::WINDOW_HEIGHT - Layout::FOOTER_HEIGHT - Layout::PADDING - Layout::PADDING * 2);
    const sf::Vector2f BOTTOM_RIGHT_ANCHOR = sf::Vector2f(Layout::WINDOW_WIDTH - Layout::PADDING * 2, Layout::WINDOW_HEIGHT - Layout::PADDING * 2);

    const sf::Vector2f TOP_LEFT_MODAL_ANCHOR = sf::Vector2f((Layout::WINDOW_WIDTH - Layout::MODAL_WIDTH) / 2, (Layout::WINDOW_HEIGHT - Layout::MODAL_HEIGHT) / 2);

    void drawModalRectangle(std::string modalTitle);

    std::string wrapText(const std::string &text, float maxWidth, unsigned int characterSize);

    sf::FloatRect getGridBox(float colStart, float rowStart, float colSpan, float rowSpan);
};
