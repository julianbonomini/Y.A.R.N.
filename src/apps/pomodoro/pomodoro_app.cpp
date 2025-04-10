#include<iostream>
#include "pomodoro_app.hpp"

#include <sstream>
#include <SFML/Graphics.hpp>
#include <string>
#include <valarray>

PomodoroApp::PomodoroApp(sf::RenderWindow &window, const sf::Font &font, StateMachine &stateMachine, const std::string &appName)
    : AppWithConfig(window, font, stateMachine, appName) {
    initConfigFromDisk();
}

void PomodoroApp::handleEvent(const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.scancode == sf::Keyboard::Scan::Space) {
        std::cout << "Swapping running state" << std::endl;
        startPauseTimer();
    }

    if (keyPressed.scancode == sf::Keyboard::Scan::R) {
        std::cout << "Reseting" << std::endl;
        resetSession();
    }
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

void PomodoroApp::updateClock() {
    if (isSessionRunning) {
        sf::Time now = timerClock.getElapsedTime();
        sf::Time delta = now - lastUpdate;

        // Track elapsed time to update remainingTime only once per second
        static sf::Time elapsedSinceLastUpdate = sf::Time::Zero;
        elapsedSinceLastUpdate += delta;

        if (elapsedSinceLastUpdate >= sf::seconds(1.0f)) {
            // Update remaining time once per second
            std::cout << "Remaining time: " << remainingTime.asSeconds() << " -> "
                      << (remainingTime - sf::seconds(1.0f)).asSeconds() << std::endl;

            remainingTime -= sf::seconds(1.0f);  // Subtract 1 second from remaining time
            elapsedSinceLastUpdate = sf::Time::Zero;  // Reset elapsed time tracker

            if (remainingTime <= sf::Time::Zero) {
                std::cout << "Time reached 0!" << std::endl;
                isWorkTime = !isWorkTime;  // Switch between work/break modes
                remainingTime = isWorkTime ? workTimeInSeconds : playTimeInSeconds;
                // Avoid restarting the timer here if it's already running
            }
        }

        // Make sure lastUpdate is updated after we've processed time.
        lastUpdate = now;
    }
}

void PomodoroApp::startPauseTimer() {
    if (!isSessionRunning) {
        isSessionRunning = true;
        timerClock.restart();
        timerClock.start();
        return;
    }
    if (timerClock.isRunning()) {
        timerClock.stop();
    } else {
        timerClock.start();
    }
}

void PomodoroApp::resetSession() {
    isSessionRunning = false;
    timerClock.restart();
    lastUpdate = sf::Time::Zero;
    remainingTime = workTimeInSeconds;
    isWorkTime = true;
}

void PomodoroApp::draw() {
    updateClock();  // Update the clock here to reflect changes
    drawWorkCounter();
    drawPlayCounter();
    drawControls();

    handleHelp();
    handleSettings();
}

void PomodoroApp::drawControls() {
    drawStartStopButton();
    drawResetButton();
}

void PomodoroApp::drawStartStopButton() {
    auto startStopBox = getGridBox(2, 0, 1, 1);
    sf::RectangleShape rect({startStopBox.size.x, startStopBox.size.y});
    rect.setPosition(startStopBox.position);
    rect.setFillColor(Colors::WHITE);
    rect.setOutlineColor(Colors::GRAY);
    rect.setOutlineThickness(Lines::LINE_THICKNESS);
    window.draw(rect);

    sf::Text text(font, "START/PAUSE");
    text.setFillColor(Colors::BLACK);
    text.setCharacterSize(TextSizes::LABEL);

    // Center text
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({textBounds.position.x + textBounds.size.x / 2, textBounds.position.y + textBounds.size.y / 2});
    text.setPosition(startStopBox.position + startStopBox.size / 2.0f);

    window.draw(text);
}

void PomodoroApp::drawResetButton() {
    auto box = getGridBox(2, 1, 1, 1);
    sf::RectangleShape rect({box.size.x, box.size.y});
    rect.setPosition(box.position);
    rect.setFillColor(Colors::WHITE);
    rect.setOutlineColor(Colors::GRAY);
    rect.setOutlineThickness(Lines::LINE_THICKNESS);
    window.draw(rect);
    sf::Text text(font, "RESET");
    text.setFillColor(Colors::BLACK);
    text.setCharacterSize(TextSizes::LABEL);
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({textBounds.position.x + textBounds.size.x / 2, textBounds.position.y + textBounds.size.y / 2});
    text.setPosition(box.position + box.size / 2.f);
    window.draw(text);
}

void PomodoroApp::drawWorkCounter() {
    auto box = getGridBox(0, 0, 2, 4);
    sf::RectangleShape rect({box.size.x, box.size.y});
    rect.setPosition(box.position);
    rect.setFillColor(isWorkTime ? Colors::GRAY : Colors::WHITE);
    rect.setOutlineColor(Colors::GRAY);
    rect.setOutlineThickness(Lines::LINE_THICKNESS);
    window.draw(rect);

    std::stringstream counterStream;
    sf::Time timeLeft = isSessionRunning && isWorkTime ? remainingTime : workTimeInSeconds;
    int mins = static_cast<int>(timeLeft.asSeconds()) / 60;
    int secs = static_cast<int>(timeLeft.asSeconds()) % 60;
    counterStream << std::setfill('0') << std::setw(2) << mins << ":" << std::setw(2) << secs;


    sf::Text label(font, "WORK TIMER");
    label.setFillColor(isWorkTime ? Colors::WHITE : Colors::BLACK);
    label.setCharacterSize(TextSizes::LABEL);
    label.setPosition({box.position + sf::Vector2f(Areas::PADDING, Areas::PADDING)});
    window.draw(label);

    sf::Text counter(font, counterStream.str());
    counter.setFillColor(isWorkTime ? Colors::WHITE : Colors::BLACK);
    counter.setCharacterSize(50);
    sf::FloatRect counterBounds = counter.getLocalBounds();
    counter.setOrigin({counterBounds.position.x + counterBounds.size.x / 2, counterBounds.position.y + counterBounds.size.y / 2});
    counter.setPosition(box.position + box.size / 2.0f);
    window.draw(counter);
}

void PomodoroApp::drawPlayCounter() {
    auto box = getGridBox(3, 0, 2, 4);
    sf::RectangleShape rect({box.size.x, box.size.y});
    rect.setPosition(box.position);
    rect.setFillColor(!isWorkTime ? Colors::GRAY : Colors::WHITE);
    rect.setOutlineColor(Colors::GRAY);
    rect.setOutlineThickness(Lines::LINE_THICKNESS);
    window.draw(rect);

    std::stringstream counterStream;
    sf::Time timeLeft = isSessionRunning && !isWorkTime ? remainingTime : playTimeInSeconds;
    int mins = static_cast<int>(timeLeft.asSeconds()) / 60;
    int secs = static_cast<int>(timeLeft.asSeconds()) % 60;
    counterStream << std::setfill('0') << std::setw(2) << mins << ":" << std::setw(2) << secs;

    sf::Text text(font, "BREAK TIMER");
    text.setFillColor(!isWorkTime ? Colors::WHITE : Colors::BLACK);
    text.setCharacterSize(TextSizes::LABEL);
    text.setPosition({box.position + sf::Vector2f(10, 10)});
    window.draw(text);

    sf::Text counter(font, counterStream.str());
    counter.setFillColor(!isWorkTime ? Colors::WHITE : Colors::BLACK);
    counter.setCharacterSize(50);
    sf::FloatRect counterBounds = counter.getLocalBounds();
    counter.setOrigin({counterBounds.position.x + counterBounds.size.x / 2, counterBounds.position.y + counterBounds.size.y / 2});
    counter.setPosition(box.position + box.size / 2.0f);
    window.draw(counter);
}

void PomodoroApp::initConfigFromDisk() {
    configOptions = std::vector<BaseConfigOptions>();
    // Refresh Rate
    BaseConfigOptions defaultWorkTimeInSeconds;
    defaultWorkTimeInSeconds.label = "default_work_time";
    defaultWorkTimeInSeconds.type = BaseConfigOptionType::FREE_NUMBER;;
    defaultWorkTimeInSeconds.options = {};
    int workInSeconds = stateMachine.getPomodoroConfig().defaultWorkTimeInSeconds;
    defaultWorkTimeInSeconds.currentValue = std::to_string(workInSeconds / 60);
    defaultWorkTimeInSeconds.selected = false;
    defaultWorkTimeInSeconds.changed = false;
    configOptions.push_back(defaultWorkTimeInSeconds);
    workTimeInSeconds = sf::seconds(workInSeconds);
    remainingTime = workTimeInSeconds;

    // Default Tab
    BaseConfigOptions defaultPlayTimeInSeconds;
    defaultPlayTimeInSeconds.label = "default_play_time";
    defaultPlayTimeInSeconds.type = BaseConfigOptionType::FREE_NUMBER;
    defaultPlayTimeInSeconds.options = {};
    int playInSeconds = stateMachine.getPomodoroConfig().defaultPlayTimeInSeconds;
    defaultPlayTimeInSeconds.currentValue = std::to_string(playInSeconds / 60);
    defaultPlayTimeInSeconds.selected = false;
    defaultPlayTimeInSeconds.changed = false;
    configOptions.push_back(defaultPlayTimeInSeconds);
    playTimeInSeconds = sf::seconds(playInSeconds);

}

void PomodoroApp::saveConfigToDisk() {
    stateMachine.savePomodoroConfigToDisk(configOptions);
}