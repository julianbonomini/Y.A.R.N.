#include "state_machine.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

#include "../../common/logger.hpp"
#include "../execute/execute_utils.hpp"

using json = nlohmann::json;

StateMachine::StateMachine(int default_tab, std::unordered_map<std::string, std::string> envConfig)
    : activeTab(default_tab), envConfig(envConfig) {
    // Load the config file on startup
    loadOsConfigFromDisk();
    setActiveTab(getOsConfig().defaultTab);
    Logger::info("STATE_MACHINE", "Loading APPs config...");
    loadPomodoroAppConfigFromDisk();
    loadMarketAppConfigFromDisk();
    loadWeatherAppConfigFromDisk();
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

WeatherConfigFile &StateMachine::getWeatherConfig() {
    return weatherConfigFile;
}

bool StateMachine::healOsConfig() {
    Logger::info("STATE_MACHINE", "Starting OS config heal...");

    std::ofstream file(ExecuteUtils::getResourcePath("assets/config/os_config.json"));
    if (!file.is_open()) {
        return false;
    }
    json j;

    j["refresh_rate_hz"] = "60";
    j["default_tab"] = "0";
    j["theme"] = "eink";
    j["shader_enabled"] = "0";
    j["shader"] = "none";
    j["flicker_enabled"] = "0";
    j["flicker_intensity"] = "10";
    file << j.dump(4); // Pretty-print with indentation
    Logger::info("STATE_MACHINE", "OS config has been overridden with defaults..");


    osConfigFile.refreshRate = 60;
    osConfigFile.defaultTab = 0;
    osConfigFile.shaderEnabled = 0;
    osConfigFile.shader = "none";
    osConfigFile.flickerEnabled = 0;
    osConfigFile.flickerIntensity = 10;
    osConfigFile.theme = "eink";
    Logger::info("STATE_MACHINE", "OS config obj was also updated...");
    return 1;
}


bool StateMachine::loadOsConfigFromDisk() {
    Logger::info("STATE_MACHINE", "Loading OS config...");
    std::ifstream file(ExecuteUtils::getResourcePath("assets/config/os_config.json"));
    if (!file.is_open()) {
        Logger::error("STATE_MACHINE", "Could not open file for os config");
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
        Logger::error("STATE_MACHINE", "FAILED TO LOAD OS CONFIG..");
        return loadDefaultOsConfig();
    }

    return 1;
}

bool StateMachine::loadDefaultOsConfig() {
    Logger::info("STATE_MACHINE", "Loading Default OS config...");

    osConfigFile.refreshRate = 60;
    osConfigFile.defaultTab = 0;
    osConfigFile.shaderEnabled = 0;
    osConfigFile.shader = "none";
    osConfigFile.flickerEnabled = 0;
    osConfigFile.flickerIntensity = 10;
    osConfigFile.theme = "eink";
    return 1;
}

bool StateMachine::saveOsConfigToDisk(const std::vector<BaseConfigOptions> &configOptions) {
    std::ofstream file(ExecuteUtils::getResourcePath("assets/config/os_config.json"));
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
            return saveMarketConfigToDisk(configOptions);
        }
        case AppConfigTypes::WEATHER: {
            return saveWeatherConfigToDisk(configOptions);
        }
    }
}

bool StateMachine::loadPomodoroAppConfigFromDisk() {
    Logger::info("STATE_MACHINE", "Loading Pomodoro App config...");
    std::ifstream file(ExecuteUtils::getResourcePath("assets/config/pomodoro_config.json"));
    if (!file.is_open()) {
        Logger::error("STATE_MACHINE", "Could not load Pomodoro App config...");


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
    std::ofstream file(ExecuteUtils::getResourcePath("assets/config/pomodoro_config.json"));
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
    Logger::info("STATE_MACHINE", "Loading Market App config...");
    std::ifstream file(ExecuteUtils::getResourcePath("assets/config/market_config.json"));
    if (!file.is_open()) {
        Logger::error("STATE_MACHINE", "Could not load Market App config...");
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
    std::ofstream file(ExecuteUtils::getResourcePath("assets/config/market_config.json"));
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

bool StateMachine::loadWeatherAppConfigFromDisk() {
    Logger::info("STATE_MACHINE", "Loading Weather App config...");
    std::ifstream file(ExecuteUtils::getResourcePath("assets/config/weather_config.json"));
    if (!file.is_open()) {
        Logger::error("STATE_MACHINE", "Could not load Weather App config...");
        return -1;
    }

    json j;
    try {
        file >> j;
        std::string refreshIntervalMinutes = j["refresh_interval_minutes"];
        std::string city = j["city"];
        weatherConfigFile.refreshIntervalInMinutes = std::stoi(refreshIntervalMinutes);
        weatherConfigFile.city = city;
    } catch (...) {
        return -1;
    }

    return 1;
}

bool StateMachine::saveWeatherConfigToDisk(const std::vector<BaseConfigOptions> &baseConfigOptions) {
    std::ofstream file(ExecuteUtils::getResourcePath("assets/config/pomodoro_config.json"));
    if (!file.is_open()) {
        return false;
    }
    json j;

    for (const auto &option: baseConfigOptions) {
        j[option.label] = option.currentValue;
        if (option.label == "refresh_interval_minutes") {
            weatherConfigFile.refreshIntervalInMinutes = std::stoi(option.currentValue);
        } else if (option.label == "city") {
            weatherConfigFile.city = option.currentValue;
        }
    }

    file << j.dump(4); // Pretty-print with indentation
    return true;
}
