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
    loadMarketAppConfigFromDisk();
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

MarketConfigFile &StateMachine::getMarketConfig() {
    return marketConfigFile;
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
        std::string shaderToggleString = j["shader_toggle"];
        std::string shaderString = j["shader"];
        std::string flickerToggleString = j["flicker_toggle"];
        std::string flickerIntenityString = j["flicker_intensity"];
        osConfigFile.refreshRate = std::stoi(refreshRateString);
        osConfigFile.defaultTab = std::stoi(defaultTabString);
        osConfigFile.shaderToggle = std::stoi(shaderToggleString);
        osConfigFile.shader = shaderString;
        osConfigFile.flickerToggle = std::stoi(flickerToggleString);
        osConfigFile.flickerIntensity = std::stoi(flickerToggleString);
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
        }
        if (option.label == "default_tab") {
            osConfigFile.defaultTab = std::stoi(option.currentValue);
        }
        if (option.label == "shader_toggle") {
            osConfigFile.defaultTab = std::stoi(option.currentValue);
        }
        if (option.label == "shader") {
            osConfigFile.defaultTab = std::stoi(option.currentValue);
        }
        if (option.label == "flicker_toggle") {
            osConfigFile.defaultTab = std::stoi(option.currentValue);
        }
        if (option.label == "flicker_intensity") {
            osConfigFile.defaultTab = std::stoi(option.currentValue);
        }
    }

    file << j.dump(4); // Pretty-print with indentation
    return true;
}

bool StateMachine::saveAppConfigToDisk(const AppConfigTypes appConfigType, const std::vector<BaseConfigOptions> &configOptions) {
    switch (appConfigType) {
        case AppConfigTypes::POMODORO: {
            return savePomodoroConfigToDisk(configOptions);
        }
        case AppConfigTypes::MARKET: {
            std::cout << "MARKET app config not implemented" << std::endl;
            return true;
        }
        case AppConfigTypes::WEATHER: {
            std::cout << "WEATHER app config not implemented" << std::endl;
            return true;
        }
    }
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
        std::string defaultWorkTimeInSeconds = j["default_work_time"];
        std::string defaultPlayTimeInSeconds = j["default_play_time"];
        pomodoroConfigFile.defaultWorkTimeInMinutes = std::stoi(defaultWorkTimeInSeconds);
        pomodoroConfigFile.defaultPlayTimeInMinutes = std::stoi(defaultPlayTimeInSeconds);
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
        if (option.label == "default_work_time") {
            pomodoroConfigFile.defaultWorkTimeInMinutes = std::stoi(option.currentValue);
        } else if (option.label == "default_play_time") {
            pomodoroConfigFile.defaultPlayTimeInMinutes = std::stoi(option.currentValue);
        }
    }

    file << j.dump(4); // Pretty-print with indentation
    return true;
}

bool StateMachine::loadMarketAppConfigFromDisk() {
    std::cout << "Loading Market App config..." << std::endl;
    std::ifstream file("config/market_config.json");
    if (!file.is_open()) {
        std::cout << "file not open" << std::endl;
        return -1;
    }

    json j;
    try {
        file >> j;
        std::string defaultRefreshInterval = j["refresh_interval"];
        marketConfigFile.defaultRefreshIntervalInMinutes = std::stoi(defaultRefreshInterval);
    } catch (...) {
        return -1;
    }

    return 1;
}

bool StateMachine::saveMarketConfigToDisk(const std::vector<BaseConfigOptions> &baseConfigOptions) {
    std::ofstream file("config/market_config.json");
    if (!file.is_open()) {
        return false;
    }
    json j;

    for (const auto &option: baseConfigOptions) {
        j[option.label] = option.currentValue;
        if (option.label == "refresh_interval") {
            marketConfigFile.defaultRefreshIntervalInMinutes = std::stoi(option.currentValue);
        }
    }

    file << j.dump(4); // Pretty-print with indentation
    return true;
}
