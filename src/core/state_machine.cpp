#include "state_machine.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

StateMachine::StateMachine(int default_tab)
    : activeTab(default_tab) {
    // Load the config file on startup
    loadOsConfigFromDisk();
    setActiveTab(getOsConfig().defaultTab);
    std::cout << "Loading APPs config..." << std::endl;
    loadPomodoroAppConfigFromDisk();
}

int StateMachine::getActiveTab() const {
    return activeTab;
}

void StateMachine::setActiveTab(int newActiveTab) {
    activeTab = newActiveTab;
}

OsConfigFile &StateMachine::getOsConfig() {
    return osConfigFile;
}

PomodoroConfigFile &StateMachine::getPomodoroConfig() {
    return pomodoroConfigFile;
}

bool StateMachine::loadOsConfigFromDisk() {
    std::cout << "Loading OS config..." << std::endl;
    std::ifstream file("config/os_config.json");
    if (!file.is_open()) {
        std::cout << "file not open" << std::endl;
        return -1;
    }

    json j;
    try {
        file >> j;
        std::string refreshRateString = j["refresh_rate_hz"];
        std::string defaultTabString = j["default_tab"];
        osConfigFile.refreshRate = std::stoi(refreshRateString);
        osConfigFile.defaultTab = std::stoi(defaultTabString);
    } catch (...) {
        return -1;
    }

    return 1;
}

bool StateMachine::saveOsConfigToDisk(const std::vector<BaseConfigOptions> &configOptions) {
    std::ofstream file("config/os_config.json");
    if (!file.is_open()) {
        return false;
    }
    json j;

    for (const auto &option: configOptions) {
        j[option.label] = option.currentValue;
        if (option.label == "refresh_rate_hz") {
            osConfigFile.refreshRate = std::stoi(option.currentValue);
        } else if (option.label == "default_tab") {
            osConfigFile.defaultTab = std::stoi(option.currentValue);
        }
    }

    file << j.dump(4); // Pretty-print with indentation
    return true;
}

bool StateMachine::loadPomodoroAppConfigFromDisk() {
    std::cout << "Loading Pomodoro App config..." << std::endl;
    std::ifstream file("config/pomodoro_config.json");
    if (!file.is_open()) {
        std::cout << "file not open" << std::endl;
        return -1;
    }

    json j;
    try {
        file >> j;
        std::string defaultWorkTimeInSeconds = j["default_work_time_in_seconds"];
        std::string defaultPlayTimeInSeconds = j["default_play_time_in_seconds"];
        pomodoroConfigFile.defaultWorkTimeInSeconds = std::stoi(defaultWorkTimeInSeconds);
        pomodoroConfigFile.defaultPlayTimeInSeconds = std::stoi(defaultPlayTimeInSeconds);
    } catch (...) {
        return -1;
    }

    return 1;
}

bool StateMachine::savePomodoroConfigToDisk(const std::vector<BaseConfigOptions> &baseConfigOptions) {
    std::ofstream file("config/pomodoro_config.json");
    if (!file.is_open()) {
        return false;
    }
    json j;

    for (const auto &option: baseConfigOptions) {
        j[option.label] = option.currentValue;
        if (option.label == "default_work_session_length") {
            pomodoroConfigFile.defaultWorkTimeInSeconds = std::stoi(option.currentValue);
        } else if (option.label == "default_play_session_length") {
            pomodoroConfigFile.defaultPlayTimeInSeconds = std::stoi(option.currentValue);
        }
    }

    file << j.dump(4); // Pretty-print with indentation
    return true;
}
