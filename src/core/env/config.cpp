#include "config.hpp"

#include <fstream>
#include <sstream>
#include "../../common/logger.hpp"
#include "../execute/execute_utils.hpp"

std::unordered_map<std::string, std::string> EnvConfig::load_env_config() {
    std::unordered_map<std::string, std::string> config;
    std::string configFile = ".config";
    Logger::info("MY ENV FILE PATH: ", ExecuteUtils::getEnvConfPath(configFile));
    std::ifstream file(ExecuteUtils::getEnvConfPath(configFile));
    if (!file.is_open()) {
        std::cerr << "Could not open config file: " << configFile << std::endl;
        return config;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines or comments
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            config[key] = value;
        }
    }

    return config;
}