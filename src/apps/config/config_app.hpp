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

    void handleEvent(const sf::Event::KeyPressed &keyPressed) override;

    void handleHelp() override;

    void handleSettings() override;

private:
    StateMachine &stateMachine;

    std::vector<ConfigOption> configOptions;
    bool editModeEnabled = false;
    bool unsavedChangesFlag = false;

    void initConfigFromDisk();

    void drawHelpBox();

    void moveDown();

    void moveUp();

    void changeOptionRight();

    void changeOptionLeft();
};

#endif // CONFIG_APP_H
