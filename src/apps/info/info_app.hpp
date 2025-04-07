#ifndef INFO_APP_H
#define INFO_APP_H

#pragma once
#include "../common/app.hpp"
#include <SFML/Graphics.hpp>

class InfoApp : public App {
public:
    InfoApp(sf::RenderWindow &window, const sf::Font &font, const std::string &appName);

    void draw() override; // Implement the drawing logic for this app

private:
    sf::RenderWindow& window;  // Reference to the SFML render window
    const sf::Font& font;      // Reference to the font
};



#endif //INFO_APP_H
