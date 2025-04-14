#ifndef OPENWEATHER_H
#define OPENWEATHER_H

#include <nlohmann/json.hpp>


class OpenWeather {
public:
    static std::optional<nlohmann::json> getWeather();
};




#endif //OPENWEATHER_H
