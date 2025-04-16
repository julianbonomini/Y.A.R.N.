#ifndef WEATHER_APP_H
#define WEATHER_APP_H

#pragma once
#include <SFML/Graphics.hpp>

#include "../../core/state_machine/weather_state.hpp"
#include "../common/app_with_config.hpp"

class WeatherApp : public AppWithConfig {
public:
    WeatherApp(
        const std::string &appName,
        sf::RenderTarget &render,
        const sf::Font &font,
        StateMachine &stateMachine,
        WeatherState &weatherState
    );

    void draw() override;

    void handleEvent(const sf::Event::KeyPressed &keyPressed) override;

    void handleHelp() override;

    void handleSettings() override;

    void initConfigFromDisk() override;

private:
    WeatherState &weatherState;

    void drawOverview();

    void drawLastUpdated();

    void drawExtraData();

    void drawSensorData();

    void drawDetails();
};


#endif //WEATHER_APP_H
