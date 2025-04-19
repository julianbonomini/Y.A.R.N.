#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP
#include <vector>
#include "../../apps/common/BaseConfigOptions.hpp"
#include <unordered_map>

#include "tabs.hpp"

enum class AppConfigTypes {
    POMODORO,
    MARKET,
    WEATHER,
};

struct OsConfigFile {
    int refreshRate;
    Tab defaultTab;
    std::string theme;
    int shaderEnabled;
    std::string shader;
    bool flickerEnabled;
    int flickerIntensity;
    bool cycleTabsEnabled;
    int cycleTabTimeInSeconds;
};

struct PomodoroConfigFile {
    int workTimeInMinutes;
    int playTimeInMinutes;
    bool switchSound;
};

struct MarketConfigFile {
    int defaultRefreshIntervalInMinutes;
};

struct WeatherConfigFile {
    int refreshIntervalInMinutes;
    std::string city;
};

class StateMachine {
public:
    StateMachine(Tab default_tab, std::unordered_map<std::string, std::string> envConfig);

    Tab getActiveTab() const;

    void nextTab();

    void previousTab();

    void setActiveTab(Tab newActiveTab);

    OsConfigFile &getOsConfig();

    PomodoroConfigFile &getPomodoroConfig();

    MarketConfigFile &getMarketConfig();

    WeatherConfigFile &getWeatherConfig();

    bool healOsConfig();

    bool saveOsConfigToDisk(const std::vector<BaseConfigOptions> &configOptions);

    bool saveAppConfigToDisk(AppConfigTypes appConfigType, const std::vector<BaseConfigOptions> &configOptions);

private:
    Tab activeTab;
    std::unordered_map<std::string, std::string> envConfig;
    OsConfigFile osConfigFile;
    PomodoroConfigFile pomodoroConfigFile;
    MarketConfigFile marketConfigFile;
    WeatherConfigFile weatherConfigFile;

    bool loadOsConfigFromDisk();

    bool loadDefaultOsConfig();

    bool loadPomodoroAppConfigFromDisk();

    bool savePomodoroConfigToDisk(const std::vector<BaseConfigOptions> &baseConfigOptions);

    bool loadMarketAppConfigFromDisk();

    bool saveMarketConfigToDisk(const std::vector<BaseConfigOptions> &baseConfigOptions);

    bool loadWeatherAppConfigFromDisk();

    bool saveWeatherConfigToDisk(const std::vector<BaseConfigOptions> &baseConfigOptions);
};

#endif // STATE_MACHINE_HPP
