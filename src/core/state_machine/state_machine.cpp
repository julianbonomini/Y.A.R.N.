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

bool StateMachine::healOsConfig() {
    std::cout << "Starting OS config heal..." << std::endl;
    std::ofstream file("config/os_config.json");
    if (!file.is_open()) {
        return false;
    }
    json j;

    j["refresh_rate_hz"] = "60";
    j["default_tab"] = "0";
    j["theme"] = "light";
    j["shader_enabled"] = "0";
    j["shader"] = "none";
    j["flicker_enabled"] = "0";
    j["flicker_intensity"] = "10";
    file << j.dump(4); // Pretty-print with indentation
    std::cout << "OS config has been overridden with defaults..." << std::endl;

    osConfigFile.refreshRate = 60;
    osConfigFile.defaultTab = 0;
    osConfigFile.shaderEnabled = 0;
    osConfigFile.shader = "none";
    osConfigFile.flickerEnabled = 0;
    osConfigFile.flickerIntensity = 10;
    osConfigFile.theme = "light";
    std::cout << "OS config obj was also updated..." << std::endl;
    return 1;
}


bool StateMachine::loadOsConfigFromDisk() {
    std::cout << "Loading OS config..." << std::endl;
    std::ifstream file("config/os_config.json");
    if (!file.is_open()) {
        std::cout << "file not open" << std::endl;
        return loadDefaultOsConfig();
    }

    json j;
    try {
        file >> j;
        osConfigFile.refreshRate = std::stoi(std::string(j["refresh_rate_hz"]));
        osConfigFile.defaultTab = std::stoi(std::string(j["default_tab"]));
        osConfigFile.theme = std::string(j["theme"]);
        osConfigFile.shaderEnabled = std::stoi(std::string(j["shader_enabled"]));
        osConfigFile.shader = std::string(j["shader"]);
        osConfigFile.flickerEnabled = std::stoi(std::string(j["flicker_enabled"]));
        osConfigFile.flickerIntensity = std::stoi(std::string(j["flicker_intensity"]));
    } catch (...) {
        std::cout << "FAILED TO LOAD OS CONFIG..." << std::endl;
        return loadDefaultOsConfig();
    }

    return 1;
}

bool StateMachine::loadDefaultOsConfig() {
    std::cout << "Loading Default OS config..." << std::endl;
    osConfigFile.refreshRate = 60;
    osConfigFile.defaultTab = 0;
    osConfigFile.shaderEnabled = 0;
    osConfigFile.shader = "none";
    osConfigFile.flickerEnabled = 0;
    osConfigFile.flickerIntensity = 10;
    osConfigFile.theme = "light";
    return 1;
}

bool StateMachine::saveOsConfigToDisk(const std::vector<BaseConfigOptions> &configOptions) {
    std::ofstream file("config/os_config.json");
    if (!file.is_open()) {
        return false;
    }
    json j;

    // TODO: here we could transform into proper type, currentValue is always a stirng
    for (const auto &option: configOptions) {
        j[option.label] = option.currentValue;
        if (option.label == "refresh_rate_hz") {
            osConfigFile.refreshRate = std::stoi(option.currentValue);
        }
        if (option.label == "default_tab") {
            osConfigFile.defaultTab = std::stoi(option.currentValue);
        }
        if (option.label == "shader_enabled") {
            osConfigFile.shaderEnabled = std::stoi(option.currentValue);
        }
        if (option.label == "shader") {
            osConfigFile.shader = option.currentValue;
        }
        if (option.label == "flicker_enabled") {
            osConfigFile.flickerEnabled = std::stoi(option.currentValue);
        }
        if (option.label == "flicker_intensity") {
            osConfigFile.flickerIntensity = std::stoi(option.currentValue);
        }
        if (option.label == "theme") {
            osConfigFile.theme = option.currentValue;
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
