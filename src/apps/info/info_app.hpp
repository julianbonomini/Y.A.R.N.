#ifndef INFO_APP_H
#define INFO_APP_H

#pragma once
#include "../common/app.hpp"
#include <SFML/Graphics.hpp>

class InfoApp : public App {
public:
    InfoApp(sf::RenderWindow &window, const sf::Font &font, const std::string &appName);

    void draw() override;

    void handleEvent(const sf::Event::KeyPressed &keyPressed) override;

    void handleHelp() override;

    void handleSettings() override;

private:
    std::vector<std::pair<std::string, std::string> > infoData;
};

#endif //INFO_APP_H
