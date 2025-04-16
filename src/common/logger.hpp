#ifndef STATIC_HELPERS_H
#define STATIC_HELPERS_H

#include<iostream>
#include<sstream>

#include "../core/env/config.hpp"

class Logger {
public:
    template<typename... Args>
    static void info(Args&&... args) {
        std::ostringstream oss;
        oss << "INFO: ";
        ((oss << args << " "), ...);
        std::cout << oss.str() << std::endl;
    }

    template<typename... Args>
    static void warning(Args&&... args) {
        attention_grabber();
        std::ostringstream oss;
        oss << "!WRN: ";
        ((oss << args << " "), ...);
        std::cout << oss.str() << std::endl;
        attention_grabber();

    }

    template<typename... Args>
    static void error(Args&&... args) {
        std::ostringstream oss;
        oss << "!ERR: ";
        ((oss << args << " "), ...);
        std::cerr << oss.str() << std::endl;
    }

    template<typename... Args>
    static void debug(Args&&... args) {
        std::unordered_map<std::string, std::string> envConfig = EnvConfig::load_env_config();
        std::string logLevel = envConfig["LOG_LEVEL"];
        if (logLevel == "DEBUG") {
            std::ostringstream oss;
            oss << "DEBUG: ";
            ((oss << args << " "), ...);
            std::cerr << oss.str() << std::endl;
        }
    }

    static void done_separator() {
        std::cout << "***************DONE*****************" << std::endl << std::endl << std::endl;
    }

    static void separator() {
        std::cout << "*************************************" << std::endl << std::endl << std::endl;
    }

    static void line() {
        std::cout << "-------------------------------------" << std::endl << std::endl << std::endl;
    }

private:
    static void attention_grabber() {
        std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    }
};



#endif //STATIC_HELPERS_H
