#ifndef CONFIG_APP_H
#define CONFIG_APP_H

#pragma once
#include "../common/app.hpp"
#include "../../core/state_machine.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

struct ConfigOption {
    std::string label; // The label/name of the setting (e.g., "Refresh Rate")
    std::string type;  // Type of control: "dropdown", "checkbox", etc.
    std::vector<std::string> options; // Options for dropdown or list (e.g., ["60", "120", "144"])
    std::string currentValue; // Current selected value, in string form
};

class ConfigApp : public App {
public:
    ConfigApp(sf::RenderWindow &window, const sf::Font &font, StateMachine &stateMachine, const std::string &appName);

    void handleEvent(const sf::Event::KeyPressed& keyPressed) override;

    void draw() override;

    // TODO: add interactivity here

private:
    sf::RenderWindow& window;
    const sf::Font& font;
    StateMachine& stateMachine;

    std::vector<ConfigOption> configOptions;
    void populateConfigOptions();
};

#endif // CONFIG_APP_H
