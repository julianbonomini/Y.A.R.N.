#include "weather_state.hpp"
#include <iomanip>
#include <sstream>
#include <ctime>

#include "../../common/logger.hpp"
#include "../../ui/utils/ui_helpers.hpp"


WeatherState::WeatherState() {
    // Optional default initialization
    data_ = {};
}

void WeatherState::updateFromJson(const nlohmann::json& json) {
    if (json.contains("main")) {
        float temp_kelvin = json["main"].value("temp", 0.0f);
        data_.temperature = UIHelpers::formatFloat(temp_kelvin - 273.15f, 1) + " C";

        float feels_like_kelvin = json["main"].value("feels_like", 0.0f);
        data_.feels_like = UIHelpers::formatFloat(feels_like_kelvin - 273.15f, 1) + " C";

        float humidity = json["main"].value("humidity", 0.0f);
        data_.humidity_pct = UIHelpers::formatFloat(humidity, 0) + "%";
    } else {
        data_.temperature = "!ERR";
    }

    if (json.contains("weather") && json["weather"].is_array() && !json["weather"].empty()) {
        const auto& weather = json["weather"][0];
        data_.weather_title = weather.value("main", "");
        data_.weather_description = weather.value("description", "");
    } else {
        data_.weather_title = "!ERR";
        data_.weather_description = "!ERR";
    }

    if (json.contains("wind")) {
        float wind_speed = json["wind"].value("speed", 0.0f);
        data_.wind_speed_meters_per_second = UIHelpers::formatFloat(wind_speed, 1) + "m/s";

        float wind_gust = json["wind"].value("gust", 0.0f);
        data_.wind_gust_meters_per_second = UIHelpers::formatFloat(wind_gust, 1) + "m/s";
    }

    if (json.contains("clouds")) {
        float clouds = json["clouds"].value("all", 0.0f);
        data_.cloudiness_pct = UIHelpers::formatFloat(clouds, 1) + "%";
    }

    if (json.contains("visibility")) {
        float visibility = json["visibility"].get<float>();
        data_.visibility_meters = UIHelpers::formatFloat(visibility, 0) + "m";
    }

    if (json.contains("rain") && json["rain"].contains("1h")) {
        float rain_1h_mm = json["rain"]["1h"].get<float>();
        data_.rain_1h_mm = UIHelpers::formatFloat(rain_1h_mm, 1) + "mm";
    } else {
        data_.rain_1h_mm = 0.0f;
    }

    // Format timestamp into readable string
    if (json.contains("dt")) {
        std::time_t timestamp = json["dt"].get<std::time_t>();
        std::tm *tm = std::localtime(&timestamp);
        std::ostringstream oss;
        oss << std::put_time(tm, "%H:%M");
        data_.last_weather_update = oss.str();
    } else {
        data_.last_weather_update = "!ERR";
    }
}

WeatherData WeatherState::getWeatherData() const {
    return data_;
}
