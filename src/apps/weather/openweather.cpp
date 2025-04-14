#include "openweather.hpp"

#include<iostream>
#include <map>

#include "../../common/logger.hpp"
#include "../../core/http/http.hpp"


std::optional<nlohmann::json> OpenWeather::getWeather() {
    std::string api_key = "e8409838ac1968c4c525799b5ac34da0"; // Replace with your OpenWeatherMap API key
    std::string city = "Amsterdam";
    std::map<std::string, std::string> query_params = {
        {"q", city},
        {"appid", api_key}
    };
    std::map<std::string, std::string> headers = {};
    std::string host = "api.openweathermap.org";
    std::string path = "/data/2.5/weather";
    std::string scheme = "https";

    auto response = Http::GET(host, path, query_params, headers, scheme);
    if (response) {
        auto data = response.value();
        return data;
    } else {
        Logger::error("Error: Could not get weather data!");
        return {};
    }
}
