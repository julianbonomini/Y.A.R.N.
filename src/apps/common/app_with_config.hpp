#ifndef APP_WITH_CONFIG_H
#define APP_WITH_CONFIG_H

#pragma once
#include "app.hpp"
#include <SFML/Graphics.hpp>
#include "../../core/state_machine.hpp"

class AppWithConfig : public App {
public:
    AppWithConfig(sf::RenderWindow &window, const sf::Font &font, StateMachine &stateMachine, const std::string &appName)
        : App(appName, window, font), stateMachine(stateMachine) {
    }


    // Constructor to initialize the window, font, and tab name
    virtual ~AppWithConfig() = default;

protected:
    StateMachine &stateMachine;
    std::vector<BaseConfigOptions> configOptions;


    virtual void initConfigFromDisk() = 0;

    virtual void saveConfigToDisk() = 0;
};


#endif //APP_WITH_CONFIG_H
