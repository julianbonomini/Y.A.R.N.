#include "pomodoro_state.hpp"

#include "../../common/logger.hpp"


PomodoroState::PomodoroState() {
    // Optional default initialization
    data_ = {};
}

sf::Time PomodoroState::getWorkTimeInSeconds() {
    return data_.workTimeInSeconds;
};

sf::Time PomodoroState::getPlayTimeInSeconds() {
    return data_.playTimeInSeconds;
};

sf::Time PomodoroState::getLastUpdate() {
    return data_.lastUpdate;
};

sf::Time PomodoroState::getRemainingTime() {
    return data_.remainingTime;
};

sf::Clock PomodoroState::getTimerClock() {
    return data_.timerClock;
};

bool PomodoroState::getIsSessionRunning() {
    return data_.isSessionRunning;
};

bool PomodoroState::getIsWorkTime() {
    return data_.isWorkTime;
};

void PomodoroState::setWorkTimeInSeconds(sf::Time workTimeInSeconds) {
    data_.workTimeInSeconds = workTimeInSeconds;
};

void PomodoroState::setPlayTimeInSeconds(sf::Time playTimeInSeconds) {
    data_.playTimeInSeconds = playTimeInSeconds;
};

void PomodoroState::setLastUpdate(sf::Time lastUpdate) {
    data_.lastUpdate = lastUpdate;
};

void PomodoroState::setRemainingTime(sf::Time remainingTime) {
    data_.remainingTime = remainingTime;
};

void PomodoroState::setTimerClock(sf::Clock timerClock) {
    data_.timerClock = timerClock;
};

void PomodoroState::setIsSessionRunning(bool isSessionRunning) {
    data_.isSessionRunning = isSessionRunning;
};

void PomodoroState::setIsWorkTime(bool isWorkTime) {
    data_.isWorkTime = isWorkTime;
};

void PomodoroState::restartTimerClock() {
    data_.timerClock.restart();
};

void PomodoroState::startTimerClock() {
    data_.timerClock.start();
};

void PomodoroState::startOrPauseClock() {
    if (data_.timerClock.isRunning()) {
        Logger::debug("4");
        data_.timerClock.stop();
    } else {
        Logger::debug("5");
        data_.timerClock.start();
    }
};

void PomodoroState::switchIsWorkTime() {
    data_.isWorkTime = !data_.isWorkTime;
};

