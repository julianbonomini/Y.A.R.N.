#ifndef CONFIG_APP_H
#define CONFIG_APP_H

#pragma once
#include "../../core/state_machine.hpp"
#include <SFML/Graphics.hpp>

#include "../common/app_with_config.hpp"

class ConfigApp : public AppWithConfig {
public:
    ConfigApp(sf::RenderWindow &window, const sf::Font &font, StateMachine &stateMachine, const std::string &appName);

    void draw() override;

    void handleEvent(const sf::Event::KeyPressed &keyPressed) override;

    void handleHelp() override;

    void handleSettings() override;

private:
    bool editModeEnabled = false;
    bool unsavedChangesFlag = false;

    void drawHelpBox();

    void moveDown();

    void moveUp();

    void changeOptionRight();

    void changeOptionLeft();

    void initConfigFromDisk() override;

    void saveConfigToDisk() override;
};

#endif // CONFIG_APP_H
