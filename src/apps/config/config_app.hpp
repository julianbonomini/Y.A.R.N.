#ifndef CONFIG_APP_H
#define CONFIG_APP_H

#pragma once
#include "../../core/state_machine/state_machine.hpp"
#include <SFML/Graphics.hpp>

#include "../common/app_with_config.hpp"

class ConfigApp : public App {
public:
    ConfigApp(const std::string &appName, sf::RenderTarget &target, const sf::Font &font, StateMachine &stateMachine, int totalAmountOfApps);

    void draw() override;

    void handleEvent(const sf::Event::KeyPressed &keyPressed) override;

    void handleHelp() override;

    void handleSettings() override;

    void initConfigFromDisk() override;

private:
    StateMachine &stateMachine;
    std::vector<BaseConfigOptions> configOptions;
    int totalAmountOfApps;
    bool editModeEnabled = false;
    bool unsavedChangesFlag = false;

    void drawControlsHelpBox();

    void drawCurrOptionHelpBox();

    void moveDown();

    void moveUp();

    void changeOptionRight();

    void changeOptionLeft();

    void saveConfigToDisk();
};

#endif // CONFIG_APP_H
