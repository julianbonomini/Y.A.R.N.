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
};



#endif //POMODORO_APP_H
