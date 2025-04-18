#include<iostream>
#include "pomodoro_app.hpp"

#include <sstream>
#include <SFML/Graphics.hpp>
#include <string>

#include "../../ui/themes/theme_manager.hpp"
#include "../../ui/utils/ui_helpers.hpp"

PomodoroApp::PomodoroApp(const std::string &appName, sf::RenderTarget &renderer, const sf::Font &font, StateMachine &stateMachine, PomodoroState &pomodoroState)
    : AppWithConfig(appName, renderer, font, stateMachine), pomodoro_state_(pomodoroState) {
    initConfigFromDisk();
}

void PomodoroApp::handleEvent(const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.scancode == sf::Keyboard::Scan::Space) {
        Logger::debug(appName, "Starting or pausing pomodoro clock");
        startPauseTimer();
    }

    if (keyPressed.scancode == sf::Keyboard::Scan::R) {
        resetSession();
    }

    if (settingsOpen) {
        if (keyPressed.scancode == sf::Keyboard::Scan::Down) {
            moveDown();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Right) {
            changeOptionRight();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Left) {
            changeOptionLeft();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Up) {
            moveUp();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Escape && unsavedChangesFlag) {
            closeWithUnsavedChanges();
            pomodoro_state_.setIsSessionRunning(false);
            pomodoro_state_.setIsWorkTime(true);
            pomodoro_state_.restartTimerClock();
            pomodoro_state_.setRemainingTime(pomodoro_state_.getWorkTimeInSeconds());
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Escape && !unsavedChangesFlag) {
            closeWithoutChanges();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::C && !unsavedChangesFlag) {
            closeWithoutChanges();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Enter) {
            pomodoro_state_.setIsSessionRunning(false);
            pomodoro_state_.setIsWorkTime(true);
            pomodoro_state_.restartTimerClock();
            saveAndClose(AppConfigTypes::POMODORO, configOptions);
            pomodoro_state_.setRemainingTime(pomodoro_state_.getWorkTimeInSeconds());
        }
    }
}

void PomodoroApp::handleHelp() {
    if (helpOpen) {
        drawModalRectangle("HELP");
    }
}

void PomodoroApp::handleSettings() {
    sf::FloatRect settingsBoxGlobalBounds = drawSettings();
    drawAppConfigOptions(settingsBoxGlobalBounds);
}

void PomodoroApp::startPauseTimer() {
    if (!pomodoro_state_.getIsSessionRunning()) {
        pomodoro_state_.setIsSessionRunning(true);
        pomodoro_state_.restartTimerClock();
        pomodoro_state_.startTimerClock();
        return;
    }
    pomodoro_state_.startOrPauseClock();
}

void PomodoroApp::resetSession() {
    pomodoro_state_.setIsSessionRunning(false);
    pomodoro_state_.restartTimerClock();
    pomodoro_state_.setLastUpdate(sf::Time::Zero);
    pomodoro_state_.setRemainingTime(pomodoro_state_.getWorkTimeInSeconds());
    pomodoro_state_.setIsWorkTime(true);
}

void PomodoroApp::draw() {
    drawWorkClock();
    drawPlayClock();
    drawControls();

    if (isSettingsOpen()) {
        handleSettings();
    }
    if (isHelpOpen()) {
        handleHelp();
    }
}

void PomodoroApp::drawControls() {
    drawStartStopButton();
    drawResetButton();
}

void PomodoroApp::drawStartStopButton() {
    auto startStopBox = getGridBox(2, 0, 1, 1);
    sf::RectangleShape rect({startStopBox.size.x, startStopBox.size.y});
    rect.setPosition(startStopBox.position);
    rect.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    rect.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);

    sf::Text text(font, "START/PAUSE");
    text.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    text.setCharacterSize(FontSizes::LABEL);

    // Center text
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::Vector2f textPos = UIHelpers::snapToGrid({textBounds.position.x + textBounds.size.x / 2, textBounds.position.y + textBounds.size.y / 2});
    text.setOrigin(textPos);
    text.setPosition(UIHelpers::snapToGrid(startStopBox.position + startStopBox.size / 2.0f));

    renderer.draw(text);
}

void PomodoroApp::drawResetButton() {
    auto box = getGridBox(2, 1, 1, 1);
    sf::RectangleShape rect({box.size.x, box.size.y});
    rect.setPosition(box.position);
    rect.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    rect.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);

    sf::Text text(font, "RESET");
    text.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    text.setCharacterSize(FontSizes::LABEL);

    sf::FloatRect textBounds = text.getLocalBounds();
    sf::Vector2f textPos = UIHelpers::snapToGrid({textBounds.position.x + textBounds.size.x / 2, textBounds.position.y + textBounds.size.y / 2});
    text.setOrigin(textPos);
    text.setPosition(UIHelpers::snapToGrid(box.position + box.size / 2.f));
    renderer.draw(text);
}

void PomodoroApp::drawWorkClock() {
    auto box = getGridBox(0, 0, 2, 4);
    sf::RectangleShape rect({box.size.x, box.size.y});
    rect.setPosition(box.position);
    rect.setFillColor(pomodoro_state_.getIsWorkTime() ? ThemeManager::instance().getCurrentTheme().secondary() : ThemeManager::instance().getCurrentTheme().background());
    rect.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);

    std::stringstream counterStream;
    sf::Time timeLeft = pomodoro_state_.getIsSessionRunning() && pomodoro_state_.getIsWorkTime() ? pomodoro_state_.getRemainingTime() : pomodoro_state_.getWorkTimeInSeconds();
    int mins = static_cast<int>(timeLeft.asSeconds()) / 60;
    int secs = static_cast<int>(timeLeft.asSeconds()) % 60;
    counterStream << std::setfill('0') << std::setw(2) << mins << ":" << std::setw(2) << secs;


    sf::Text label(font, "WORK TIMER");
    label.setFillColor(pomodoro_state_.getIsWorkTime() ? ThemeManager::instance().getCurrentTheme().background() : ThemeManager::instance().getCurrentTheme().primary());
    label.setCharacterSize(FontSizes::LABEL);
    sf::Vector2f labelPos = UIHelpers::snapToGrid({box.position + sf::Vector2f(Layout::PADDING, Layout::PADDING)});
    label.setPosition(labelPos);
    renderer.draw(label);

    sf::Text counter(font, counterStream.str());
    counter.setFillColor(pomodoro_state_.getIsWorkTime() ? ThemeManager::instance().getCurrentTheme().background() : ThemeManager::instance().getCurrentTheme().primary());
    counter.setCharacterSize(FontSizes::HUGE_TEXT);
    sf::FloatRect counterBounds = counter.getLocalBounds();
    sf::Vector2f counterPos = UIHelpers::snapToGrid({counterBounds.position.x + counterBounds.size.x / 2, counterBounds.position.y + counterBounds.size.y / 2});
    counter.setOrigin(counterPos);
    counter.setPosition(UIHelpers::snapToGrid(box.position + box.size / 2.0f));
    renderer.draw(counter);
}

void PomodoroApp::drawPlayClock() {
    auto box = getGridBox(3, 0, 2, 4);
    sf::RectangleShape rect({box.size.x, box.size.y});
    rect.setPosition(box.position);
    rect.setFillColor(!pomodoro_state_.getIsWorkTime() ? ThemeManager::instance().getCurrentTheme().secondary() : ThemeManager::instance().getCurrentTheme().background());
    rect.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);

    std::stringstream counterStream;
    sf::Time timeLeft = pomodoro_state_.getIsSessionRunning() && !pomodoro_state_.getIsWorkTime() ? pomodoro_state_.getRemainingTime() : pomodoro_state_.getPlayTimeInSeconds();
    int mins = static_cast<int>(timeLeft.asSeconds()) / 60;
    int secs = static_cast<int>(timeLeft.asSeconds()) % 60;
    counterStream << std::setfill('0') << std::setw(2) << mins << ":" << std::setw(2) << secs;

    sf::Text text(font, "BREAK TIMER");
    text.setFillColor(!pomodoro_state_.getIsWorkTime() ? ThemeManager::instance().getCurrentTheme().background() : ThemeManager::instance().getCurrentTheme().primary());
    text.setCharacterSize(FontSizes::LABEL);
    text.setPosition({box.position + sf::Vector2f(Layout::PADDING, Layout::PADDING)});
    renderer.draw(text);

    sf::Text counter(font, counterStream.str());
    counter.setFillColor(!pomodoro_state_.getIsWorkTime() ? ThemeManager::instance().getCurrentTheme().background() : ThemeManager::instance().getCurrentTheme().primary());
    counter.setCharacterSize(50);
    sf::FloatRect counterBounds = counter.getLocalBounds();
    sf::Vector2f counterPos = UIHelpers::snapToGrid({counterBounds.position.x + counterBounds.size.x / 2, counterBounds.position.y + counterBounds.size.y / 2});
    counter.setOrigin(counterPos);
    counter.setPosition(UIHelpers::snapToGrid(box.position + box.size / 2.0f));
    renderer.draw(counter);
}

void PomodoroApp::initConfigFromDisk() {
    configOptions = std::vector<BaseConfigOptions>();
    // Refresh Rate
    BaseConfigOptions defaultWorkTimeInSeconds;
    defaultWorkTimeInSeconds.label = "work_time";
    defaultWorkTimeInSeconds.type = BaseConfigOptionType::FREE_NUMBER;;
    defaultWorkTimeInSeconds.options = {};
    int workTimeInMinutes = stateMachine.getPomodoroConfig().workTimeInMinutes;
    defaultWorkTimeInSeconds.currentValue = std::to_string(workTimeInMinutes);
    defaultWorkTimeInSeconds.selected = false;
    defaultWorkTimeInSeconds.changed = false;
    defaultWorkTimeInSeconds.description = "The amount of time you will be working. In minutes";
    configOptions.push_back(defaultWorkTimeInSeconds);
    pomodoro_state_.setWorkTimeInSeconds(sf::seconds(workTimeInMinutes * 60));
    pomodoro_state_.setRemainingTime(pomodoro_state_.getWorkTimeInSeconds());

    // Default Tab
    BaseConfigOptions defaultPlayTimeInSeconds;
    defaultPlayTimeInSeconds.label = "play_time";
    defaultPlayTimeInSeconds.type = BaseConfigOptionType::FREE_NUMBER;
    defaultPlayTimeInSeconds.options = {};
    int playTimeInMinutes = stateMachine.getPomodoroConfig().playTimeInMinutes;
    defaultPlayTimeInSeconds.currentValue = std::to_string(playTimeInMinutes);
    defaultPlayTimeInSeconds.selected = false;
    defaultPlayTimeInSeconds.changed = false;
    defaultPlayTimeInSeconds.description = "The amount of time you will be on break. In minutes";
    configOptions.push_back(defaultPlayTimeInSeconds);
    pomodoro_state_.setPlayTimeInSeconds(sf::seconds(playTimeInMinutes * 60));

    BaseConfigOptions switchSound;
    switchSound.label = "switch_sound";
    switchSound.type = BaseConfigOptionType::TOGGLE;
    switchSound.options = {};
    switchSound.currentValue = std::to_string(stateMachine.getPomodoroConfig().switchSound);
    switchSound.selected = false;
    switchSound.changed = false;
    switchSound.description = "A tone will play every time the clock ends if enabled.";
    configOptions.push_back(switchSound);
}
