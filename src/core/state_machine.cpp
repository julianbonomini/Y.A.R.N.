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
    std::cout << "Loading APP config..." << std::endl;
}

int StateMachine::getActiveTab() const {
    return activeTab;
}

void StateMachine::setActiveTab(int newActiveTab) {
    activeTab = newActiveTab;
}

OsConfig &StateMachine::getOsConfig() {
    return osConfig;
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
        osConfig.refreshRate = std::stoi(refreshRateString);
        osConfig.defaultTab = std::stoi(defaultTabString);
    } catch (...) {
        return -1;
    }

    return 1;
}

bool StateMachine::saveOsConfigToDisk(const std::vector<ConfigOption> &configOptions) {
    std::ofstream file("config/os_config.json");
    if (!file.is_open()) {
        return false;
    }
    json j;

    for (const auto &option: configOptions) {
        j[option.label] = option.currentValue;
        if (option.label == "refresh_rate_hz") {
            osConfig.refreshRate = std::stoi(option.currentValue);
        } else if (option.label == "default_tab") {
            osConfig.defaultTab = std::stoi(option.currentValue);
        }
    }

    file << j.dump(4); // Pretty-print with indentation
    return true;
}
