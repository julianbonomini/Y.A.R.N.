#include<iostream>
#include "pomodoro_app.hpp"

#include <sstream>
#include <SFML/Graphics.hpp>
#include <string>

#include "../../ui/utils/ui_helpers.hpp"

PomodoroApp::PomodoroApp(sf::RenderTarget &renderer, const sf::Font &font, StateMachine &stateMachine, const std::string &appName)
    : AppWithConfig(renderer, font, stateMachine, appName) {
    initConfigFromDisk();
}

void PomodoroApp::handleEvent(const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.scancode == sf::Keyboard::Scan::Space) {
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
            isSessionRunning = false;
            isWorkTime = true;
            timerClock.restart();
            remainingTime = workTimeInSeconds;
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Escape && !unsavedChangesFlag) {
            closeWithoutChanges();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::C && !unsavedChangesFlag) {
            closeWithoutChanges();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Enter) {
            isSessionRunning = false;
            isWorkTime = true;
            timerClock.restart();
            saveAndClose(AppConfigTypes::POMODORO, configOptions);
            remainingTime = workTimeInSeconds;
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

void PomodoroApp::updateClock() {
    if (isSessionRunning) {
        sf::Time now = timerClock.getElapsedTime();
        sf::Time delta = now - lastUpdate;

        // Track elapsed time to update remainingTime only once per second
        static sf::Time elapsedSinceLastUpdate = sf::Time::Zero;
        elapsedSinceLastUpdate += delta;

        if (elapsedSinceLastUpdate >= sf::seconds(1.0f)) {
            // Update remaining time once per second
            remainingTime -= sf::seconds(1.0f); // Subtract 1 second from remaining time
            elapsedSinceLastUpdate = sf::Time::Zero; // Reset elapsed time tracker

            if (remainingTime <= sf::Time::Zero) {
                isWorkTime = !isWorkTime; // Switch between work/break modes
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
    updateClock(); // Update the clock here to reflect changes
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
    rect.setFillColor(Colors::WHITE);
    rect.setOutlineColor(Colors::GRAY);
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);

    sf::Text text(font, "START/PAUSE");
    text.setFillColor(Colors::BLACK);
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
    rect.setFillColor(Colors::WHITE);
    rect.setOutlineColor(Colors::GRAY);
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);

    sf::Text text(font, "RESET");
    text.setFillColor(Colors::BLACK);
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
    rect.setFillColor(isWorkTime ? Colors::GRAY : Colors::WHITE);
    rect.setOutlineColor(Colors::GRAY);
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);

    std::stringstream counterStream;
    sf::Time timeLeft = isSessionRunning && isWorkTime ? remainingTime : workTimeInSeconds;
    int mins = static_cast<int>(timeLeft.asSeconds()) / 60;
    int secs = static_cast<int>(timeLeft.asSeconds()) % 60;
    counterStream << std::setfill('0') << std::setw(2) << mins << ":" << std::setw(2) << secs;


    sf::Text label(font, "WORK TIMER");
    label.setFillColor(isWorkTime ? Colors::WHITE : Colors::BLACK);
    label.setCharacterSize(FontSizes::LABEL);
    sf::Vector2f labelPos = UIHelpers::snapToGrid({box.position + sf::Vector2f(Layout::PADDING, Layout::PADDING)});
    label.setPosition(labelPos);
    renderer.draw(label);

    sf::Text counter(font, counterStream.str());
    counter.setFillColor(isWorkTime ? Colors::WHITE : Colors::BLACK);
    counter.setCharacterSize(50);
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
    rect.setFillColor(!isWorkTime ? Colors::GRAY : Colors::WHITE);
    rect.setOutlineColor(Colors::GRAY);
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);

    std::stringstream counterStream;
    sf::Time timeLeft = isSessionRunning && !isWorkTime ? remainingTime : playTimeInSeconds;
    int mins = static_cast<int>(timeLeft.asSeconds()) / 60;
    int secs = static_cast<int>(timeLeft.asSeconds()) % 60;
    counterStream << std::setfill('0') << std::setw(2) << mins << ":" << std::setw(2) << secs;

    sf::Text text(font, "BREAK TIMER");
    text.setFillColor(!isWorkTime ? Colors::WHITE : Colors::BLACK);
    text.setCharacterSize(FontSizes::LABEL);
    text.setPosition({box.position + sf::Vector2f(Layout::PADDING, Layout::PADDING)});
    renderer.draw(text);

    sf::Text counter(font, counterStream.str());
    counter.setFillColor(!isWorkTime ? Colors::WHITE : Colors::BLACK);
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
    defaultWorkTimeInSeconds.label = "default_work_time";
    defaultWorkTimeInSeconds.type = BaseConfigOptionType::FREE_NUMBER;;
    defaultWorkTimeInSeconds.options = {};
    int workTimeInMinutes = stateMachine.getPomodoroConfig().defaultWorkTimeInMinutes;\
    defaultWorkTimeInSeconds.currentValue = std::to_string(workTimeInMinutes);
    defaultWorkTimeInSeconds.selected = false;
    defaultWorkTimeInSeconds.changed = false;
    configOptions.push_back(defaultWorkTimeInSeconds);
    workTimeInSeconds = sf::seconds(workTimeInMinutes * 60);
    remainingTime = workTimeInSeconds;

    // Default Tab
    BaseConfigOptions defaultPlayTimeInSeconds;
    defaultPlayTimeInSeconds.label = "default_play_time";
    defaultPlayTimeInSeconds.type = BaseConfigOptionType::FREE_NUMBER;
    defaultPlayTimeInSeconds.options = {};
    int playTimeInMinutes = stateMachine.getPomodoroConfig().defaultPlayTimeInMinutes;
    defaultPlayTimeInSeconds.currentValue = std::to_string(playTimeInMinutes);
    defaultPlayTimeInSeconds.selected = false;
    defaultPlayTimeInSeconds.changed = false;
    configOptions.push_back(defaultPlayTimeInSeconds);
    playTimeInSeconds = sf::seconds(playTimeInMinutes * 60);
}
