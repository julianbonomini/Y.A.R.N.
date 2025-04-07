#include "state_machine.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

StateMachine::StateMachine(int default_tab)
    : activeTab(default_tab)
{
    // Load the config file on startup
    std::cout << "Loading OS config..." << std::endl;
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

OsConfig& StateMachine::getOsConfig() {
    return osConfig;
}

bool StateMachine::loadOsConfigFromDisk() {
    std::ifstream file("config/os_config.json");
    if (!file.is_open()) {
        std::cout << "file not open" << std::endl;
        return -1;
    }

    json j;
    try {
        file >> j;
        if (j.contains("refreshRate")) {
            osConfig.refreshRate = j["refreshRate"];
            osConfig.defaultTab = j["defaultTab"];
        }
    } catch (...) {
        return -1;
    }

    return 1;
}

bool StateMachine::saveOsConfigToDisk() const {
    std::filesystem::create_directories("config"); // Ensure the config directory exists

    std::ofstream file("config/os_config.json");
    if (!file.is_open()) {
        return false;
    }

    json j;
    j["refreshRate"] = osConfig.refreshRate;
    j["defaultTab"] = osConfig.defaultTab;

    file << j.dump(4); // Pretty-print with indentation
    return true;
}
