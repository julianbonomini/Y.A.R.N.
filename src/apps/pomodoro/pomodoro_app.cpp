#include<iostream>
#include "pomodoro_app.hpp"
#include <SFML/Graphics.hpp>
#include <string>

PomodoroApp::PomodoroApp(sf::RenderWindow &window, const sf::Font &font, const std::string &appName)
    : App(appName, window, font) {
}

void PomodoroApp::handleEvent(const sf::Event::KeyPressed &keyPressed) {
}

void PomodoroApp::handleHelp() {
    if (helpOpen) {
        drawModalRectangle(window, "HELP");
    }
}

void PomodoroApp::handleSettings() {
    if (settingsOpen) {
        drawModalRectangle(window, "SETTINGS");
    }
}


void PomodoroApp::draw() {
    handleHelp();
    handleSettings();
}
