#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>
#include <unordered_map>

class EnvConfig {
public:
    static std::unordered_map<std::string, std::string> load_env_config();

};

#endif //CONFIG_H
