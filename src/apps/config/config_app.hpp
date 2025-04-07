#ifndef CONFIG_APP_H
#define CONFIG_APP_H

#pragma once
#include "../common/app.hpp"
#include <SFML/Graphics.hpp>

class ConfigApp : public App {
public:
    ConfigApp(sf::RenderWindow &window, const sf::Font &font, const std::string &appName);

    void draw() override;

private:
    sf::RenderWindow& window;
    const sf::Font& font;
    std::vector<std::string> configLabels;
};

#endif //CONFIG_APP_H
