#ifndef INFO_APP_H
#define INFO_APP_H

#pragma once
#include "../common/app.hpp"
#include <SFML/Graphics.hpp>

class InfoApp : public App {
public:
    InfoApp(sf::RenderWindow &window, const sf::Font &font, const std::string &appName);

    void handleEvent(const sf::Event::KeyPressed& keyPressed) override;

    void draw() override;

private:
    sf::RenderWindow& window;
    const sf::Font& font;
    std::vector<std::pair<std::string, std::string>> infoData;
};

#endif //INFO_APP_H
