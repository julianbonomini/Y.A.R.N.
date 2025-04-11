#ifndef WEATHER_APP_H
#define WEATHER_APP_H

#pragma once
#include <SFML/Graphics.hpp>

#include "../common/app_with_config.hpp"

class WeatherApp : public AppWithConfig {
public:
    WeatherApp(const std::string &appName, sf::RenderTarget &render, const sf::Font &font, StateMachine &stateMachine);

    void draw() override;

    void handleEvent(const sf::Event::KeyPressed &keyPressed) override;

    void handleHelp() override;

    void handleSettings() override;

    void initConfigFromDisk() override;

private:
    void drawOverview();

    void drawLastUpdated();

    void drawHighLows();

    void drawSensorData();

    void drawDetails();
};


#endif //WEATHER_APP_H
