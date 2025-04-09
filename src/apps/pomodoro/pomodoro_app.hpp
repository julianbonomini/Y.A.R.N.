#ifndef POMODORO_APP_H
#define POMODORO_APP_H


#pragma once
#include "../common/app.hpp"
#include <SFML/Graphics.hpp>

class PomodoroApp : public App {
public:
    PomodoroApp(sf::RenderWindow &window, const sf::Font &font, const std::string &appName);

    void draw() override;

    void handleEvent(const sf::Event::KeyPressed &keyPressed) override;

    void handleHelp() override;

    void handleSettings() override;

private:
    sf::Time workTimeInSeconds = sf::seconds(5); //25 * 60;
    sf::Time playTimeInSeconds = sf::seconds(5); //5 * 60;
    sf::Time lastUpdate = sf::Time::Zero;
    sf::Time remainingTime = workTimeInSeconds;
    sf::Clock timerClock;

    bool isSessionRunning = false;
    bool isWorkTime = true;

    void updateClock();

    void startPauseTimer();

    void resetSession();

    void drawWorkCounter();

    void drawPlayCounter();

    void drawControls();

    void drawStartStopButton();

    void drawResetButton();
};


#endif //POMODORO_APP_H
