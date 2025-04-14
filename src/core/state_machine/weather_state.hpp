#ifndef WEATHER_STATE_H
#define WEATHER_STATE_H
#include <nlohmann/json.hpp>
#include <string>

struct WeatherData {
  std::string temperature;
  std::string feels_like;
  std::string humidity_pct;
  std::string weather_title;         // e.g., "Rain"
  std::string weather_description;   // e.g., "moderate rain"
  std::string wind_speed_meters_per_second;
  std::string last_weather_update;   // datetime string of weather update, not api call
  std::string cloudiness_pct;
  std::string rain_1h_mm;
};

class WeatherState {
public:
  WeatherState();

  // Parses and stores data from a JSON object
  void updateFromJson(const nlohmann::json& json);

  // Returns the latest stored weather data
  WeatherData getWeatherData() const;

private:
  WeatherData data_;
};

#endif // WEATHER_STATE_H
