#include "config_app.hpp"

#include <iostream>
#include <sstream>

ConfigApp::ConfigApp(sf::RenderWindow &window, const sf::Font &font, StateMachine &stateMachine, const std::string &appName)
    : App(appName), window(window), font(font), stateMachine(stateMachine)
{
    // Sync selectedRefreshRateIndex with the actual config value
    int configRefreshRate = stateMachine.getOsConfig().refreshRate;
    for (size_t i = 0; i < refreshRateOptions.size(); ++i) {
        if (refreshRateOptions[i] == configRefreshRate) {
            selectedRefreshRateIndex = static_cast<int>(i);
            break;
        }
    }
}

void ConfigApp::draw() {
    sf::Text title(font, "Settings", TextSizes::TITLE);
    title.setFillColor(Colors::Text);
    title.setPosition({50.f, 30.f});
    window.draw(title);

    // Draw Refresh Rate Label
    sf::Text label(font, "Refresh Rate:", TextSizes::LABEL);
    label.setFillColor(Colors::Text);
    label.setPosition({50.f, 100.f});
    window.draw(label);

    // Draw Refresh Rate Value
    std::stringstream ss;
    ss << refreshRateOptions[selectedRefreshRateIndex] << " Hz";

    sf::Text value(font, ss.str(), TextSizes::VALUE);
    value.setFillColor(Colors::Text);
    value.setPosition({250.f, 100.f});
    window.draw(value);
}