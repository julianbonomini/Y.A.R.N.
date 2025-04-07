#ifndef CONFIG_APP_H
#define CONFIG_APP_H

#pragma once
#include "../common/app.hpp"
#include "../../core/state_machine.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class ConfigApp : public App {
public:
    ConfigApp(sf::RenderWindow &window, const sf::Font &font, StateMachine &stateMachine, const std::string &appName);

    void draw() override;

    // later we'll add interactivity here

private:
    sf::RenderWindow& window;
    const sf::Font& font;
    StateMachine& stateMachine;

    std::vector<int> refreshRateOptions = {1, 30, 60};
    int selectedRefreshRateIndex = 0; // will be initialized from config
};

#endif // CONFIG_APP_H
