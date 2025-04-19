#ifndef POMODORO_STATE_H
#define POMODORO_STATE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "state_machine.hpp"
#include "../../common/logger.hpp"
#include "../execute/execute_utils.hpp"


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

    static void updatePomodoroClockIfRunning(PomodoroState &pomodoroState, StateMachine &stateMachine) {
    if (pomodoroState.getIsSessionRunning()) {
        sf::Time now = pomodoroState.getTimerClock().getElapsedTime();
        sf::Time delta = now - pomodoroState.getLastUpdate();

        // Track elapsed time to update remainingTime only once per second
        static sf::Time elapsedSinceLastUpdate = sf::Time::Zero;
        elapsedSinceLastUpdate += delta;

        if (elapsedSinceLastUpdate >= sf::seconds(1.0f)) {
            // Update remaining time once per second
            pomodoroState.setRemainingTime(pomodoroState.getRemainingTime() - sf::seconds(1.0f));
            // Subtract 1 second from remaining time
            elapsedSinceLastUpdate = sf::Time::Zero; // Reset elapsed time tracker

            if (pomodoroState.getRemainingTime() <= sf::Time::Zero) {
                // Bring focus to pomodoro clock when changing
                stateMachine.setActiveTab(Tab::PMD);

                bool withSound = stateMachine.getPomodoroConfig().switchSound;
                sf::SoundBuffer buffer;
                if (withSound && buffer.loadFromFile(ExecuteUtils::getResourcePath("assets/sounds/tone.wav"))) {
                    Logger::debug("Should play pomodoro sound. App will freeze for a second.");
                    sf::Sound sound(buffer);
                    sound.play();
                    // Wait while the sound is still playing
                    while (sound.getStatus() == sf::Sound::Status::Playing) {
                        sleep(sf::milliseconds(100)); // Sleep to avoid CPU spin
                    }
                }
                pomodoroState.switchIsWorkTime();
                pomodoroState.setRemainingTime(pomodoroState.getIsWorkTime()
                                                   ? pomodoroState.getWorkTimeInSeconds()
                                                   : pomodoroState.getPlayTimeInSeconds());
                // Avoid restarting the timer here if it's already running
            }
        }

        // Make sure lastUpdate is updated after we've processed time.
        pomodoroState.setLastUpdate(now);
    }
}

private:
    PomodoroData data_;
};

#endif //POMODORO_STATE_H
