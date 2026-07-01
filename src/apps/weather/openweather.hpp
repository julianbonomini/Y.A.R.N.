#ifndef OPENWEATHER_H
#define OPENWEATHER_H

#include <nlohmann/json.hpp>

#include "../../common/logger.hpp"
#include "../../core/http/http.hpp"


class OpenWeather {
public:
    static nlohmann::json getWeather(std::string city) {
        std::string api_key = EnvConfig::load_env_config()["OPENWEATHER_API_KEY"];
        std::map<std::string, std::string> query_params = {
            {"q", city},
            {"appid", api_key}
        };
        std::map<std::string, std::string> headers = {};
        std::string host = "api.openweathermap.org";
        std::string path = "/data/2.5/weather";

        auto response = Http::GET(host, path, query_params, headers);
        if (response) {
            auto data = response.value();
            return data;
        } else {
            Logger::error("Error: Could not get weather data!");
            nlohmann::json emptyData = {};
            return emptyData;
        }
    };

    // paid
    static nlohmann::json getDailyForecast() {
        std::string api_key = "whaaa"; // Replace with your OpenWeatherMap API key
        std::string city = "Amsterdam";
        std::map<std::string, std::string> query_params = {
            {"q", city},
            {"cnt", "7"},
            {"appid", api_key}
        };
        std::map<std::string, std::string> headers = {};
        std::string host = "api.openweathermap.org";
        std::string path = "/data/2.5/forecast/daily";

        auto response = Http::GET(host, path, query_params, headers);
        if (response) {
            auto data = response.value();
            Logger::info("forecast", data);
            return data;
        } else {
            Logger::error("Error: Could not get weather data!");
            nlohmann::json emptyData = {};
            return emptyData;
        }
    };
};




#endif //OPENWEATHER_H
