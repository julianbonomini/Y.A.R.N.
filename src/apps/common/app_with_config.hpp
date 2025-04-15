#ifndef APP_WITH_CONFIG_H
#define APP_WITH_CONFIG_H

#pragma once
#include "app.hpp"
#include <SFML/Graphics.hpp>
#include "../../core/state_machine/state_machine.hpp"

class AppWithConfig : public App {
public:
    AppWithConfig(const std::string &appName, sf::RenderTarget &target, const sf::Font &font, StateMachine &stateMachine)
        : App(appName, target, font), stateMachine(stateMachine) {
    }

    // Override default App implementation to always set first option as active
    void setSettingsOpen(bool value) {
        settingsOpen = value;
        if (value) {
            configOptions[0].selected = true;
            for (size_t i = 1; i < configOptions.size(); ++i) {
                configOptions[i].selected = false;
            }
        }
    }

    // Constructor to initialize the window, font, and tab name
    virtual ~AppWithConfig() = default;

protected:
    StateMachine &stateMachine;
    std::vector<BaseConfigOptions> configOptions;
    bool unsavedChangesFlag = false;

    sf::FloatRect drawSettings();

    void drawAppConfigOptions(sf::FloatRect bounds);

    void drawCurrOptionHelpBox();

    void moveDown();

    void moveUp();

    void changeOptionRight();

    void changeOptionLeft();

    void closeWithoutChanges();

    void closeWithUnsavedChanges();

    void saveAndClose(AppConfigTypes appConfigType, const std::vector<BaseConfigOptions> &toBeSaved);

    void handleSettingsInputs(const sf::Event::KeyPressed &keyPressed);
};


#endif //APP_WITH_CONFIG_H
