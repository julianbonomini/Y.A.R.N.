#ifndef POMODORO_APP_H
#define POMODORO_APP_H


#pragma once
#include <SFML/Graphics.hpp>

#include "../common/app_with_config.hpp"

class PomodoroApp : public AppWithConfig {
public:
    PomodoroApp(const std::string &appName, sf::RenderTarget &render, const sf::Font &font, StateMachine &stateMachine);

    void draw() override;

    void handleEvent(const sf::Event::KeyPressed &keyPressed) override;

    void handleHelp() override;

    void handleSettings() override;

    void initConfigFromDisk() override;

private:
    sf::Time workTimeInSeconds = sf::Time::Zero;
    sf::Time playTimeInSeconds = sf::Time::Zero;
    sf::Time lastUpdate = sf::Time::Zero;
    sf::Time remainingTime = sf::Time::Zero;
    sf::Clock timerClock;

    bool isSessionRunning = false;
    bool isWorkTime = true;

    void updateClock();

    void startPauseTimer();

    void resetSession();

    void drawWorkClock();

    void drawPlayClock();

    void drawControls();

    void drawStartStopButton();

    void drawResetButton();
};


#endif //POMODORO_APP_H
