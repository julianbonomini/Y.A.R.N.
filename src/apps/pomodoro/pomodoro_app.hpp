#ifndef POMODORO_APP_H
#define POMODORO_APP_H


#pragma once
#include <SFML/Graphics.hpp>

#include "../../core/state_machine/pomodoro_state.hpp"
#include "../common/app_with_config.hpp"

class PomodoroApp : public AppWithConfig {
public:
    PomodoroApp(const std::string &appName, sf::RenderTarget &render, const sf::Font &font, StateMachine &stateMachine, PomodoroState &pomodoroState);

    void draw() override;

    void handleEvent(const sf::Event::KeyPressed &keyPressed) override;

    void handleHelp() override;

    void handleSettings() override;

    void initConfigFromDisk() override;

private:
    PomodoroState &pomodoro_state_;

    void startPauseTimer();

    void resetSession();

    void drawWorkClock();

    void drawPlayClock();

    void drawControls();

    void drawStartStopButton();

    void drawResetButton();
};


#endif //POMODORO_APP_H
