#include "config_app.hpp"

#include <iostream>
#include <sstream>

ConfigApp::ConfigApp(sf::RenderWindow &window, const sf::Font &font, StateMachine &stateMachine, const std::string &appName)
    : App(appName), window(window), font(font), stateMachine(stateMachine)
{
    populateConfigOptions();
}

void ConfigApp::draw() {
    float labelPositionX = TOP_LEFT.x + Theme::PADDING;
    float valuePositionX = TOP_LEFT.x + Theme::PADDING + 150.f;
    float verticalOffset = Theme::PADDING;
    float labelSpacing = 15.f;

    // Loop through each config option dynamically
    for (size_t i = 0; i < configOptions.size(); ++i) {
        // Draw the label (the setting name, e.g., "Refresh Rate:")
        sf::Text labelText(font, configOptions[i].label, TextSizes::LABEL);
        labelText.setFillColor(Colors::Text);
        labelText.setPosition({labelPositionX, TOP_LEFT.y + verticalOffset});
        window.draw(labelText);

        // Draw the value for this config option (e.g., the selected refresh rate or tab)
        std::stringstream valueStream;
        valueStream << configOptions[i].currentValue;

        sf::Text valueText(font, valueStream.str(), TextSizes::VALUE);
        valueText.setFillColor(Colors::Text);
        valueText.setPosition({valuePositionX, TOP_LEFT.y + verticalOffset});
        window.draw(valueText);

        // Increment the vertical offset to position the next label/value pair
        verticalOffset += labelSpacing;
    }
}

void ConfigApp::populateConfigOptions() {
    // Refresh Rate
    ConfigOption refreshRateOption;
    refreshRateOption.label = "Refresh Rate";
    refreshRateOption.type = "dropdown";
    refreshRateOption.options = {"60", "120", "144"};
    refreshRateOption.currentValue = std::to_string(stateMachine.getOsConfig().refreshRate);
    configOptions.push_back(refreshRateOption);

    // Default Tab
    ConfigOption defaultTabOption;
    defaultTabOption.label = "Default Tab";
    defaultTabOption.type = "dropdown";
    defaultTabOption.options = {"0", "1", "2", "3"};
    defaultTabOption.currentValue = std::to_string(stateMachine.getOsConfig().defaultTab);
    configOptions.push_back(defaultTabOption);
}