#include "state_machine.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

StateMachine::StateMachine(int default_tab)
    : activeTab(default_tab)
{
    // Load the config file on startup
    loadConfigFromDisk("config/os_config.json");
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

bool StateMachine::loadConfigFromDisk(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "file not open" << std::endl;
        return false;
    }

    std::cout << path << std::endl;

    json j;
    try {
        file >> j;
        if (j.contains("refreshRate")) {
            std::cout << "so far super good, " << j["refreshRate"] << std::endl;

            osConfig.refreshRate = j["refreshRate"];
        }
    } catch (...) {
        return false;
    }

    return true;
}

bool StateMachine::saveConfigToDisk(const std::string& path) const {
    std::filesystem::create_directories("config"); // Ensure the config directory exists

    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }

    json j;
    j["refreshRate"] = osConfig.refreshRate;

    file << j.dump(4); // Pretty-print with indentation
    return true;
}
