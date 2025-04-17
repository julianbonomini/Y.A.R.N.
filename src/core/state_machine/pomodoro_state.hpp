#ifndef POMODORO_STATE_H
#define POMODORO_STATE_H

#include <SFML/Graphics.hpp>


struct PomodoroData {
    sf::Time workTimeInSeconds = sf::Time::Zero;
    sf::Time playTimeInSeconds = sf::Time::Zero;
    sf::Time lastUpdate = sf::Time::Zero;
    sf::Time remainingTime = sf::Time::Zero;
    sf::Clock timerClock;

    bool isSessionRunning = false;
    bool isWorkTime = true;
};

class PomodoroState {
public:
    PomodoroState();

    sf::Time getWorkTimeInSeconds();

    sf::Time getPlayTimeInSeconds();

    sf::Time getLastUpdate();

    sf::Time getRemainingTime();

    sf::Clock getTimerClock();

    bool getIsSessionRunning();

    bool getIsWorkTime();

    void setWorkTimeInSeconds(sf::Time workTimeInSeconds);

    void setPlayTimeInSeconds(sf::Time playTimeInSeconds);

    void setLastUpdate(sf::Time lastUpdate);

    void setRemainingTime(sf::Time remainingTime);

    void setTimerClock(sf::Clock timerClock);

    void setIsSessionRunning(bool isSessionRunning);

    void setIsWorkTime(bool isWorkTime);

    void restartTimerClock();

    void startTimerClock();

    void startOrPauseClock();

    void switchIsWorkTime();

private:
    PomodoroData data_;
};

#endif //POMODORO_STATE_H
