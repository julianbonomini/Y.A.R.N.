#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP
#include <vector>
#include "../../apps/common/BaseConfigOptions.hpp"

enum class AppConfigTypes {
    POMODORO,
    MARKET,
    WEATHER,
};

struct OsConfigFile {
    int refreshRate;
    int defaultTab;
    std::string theme;
    int shaderEnabled;
    std::string shader;
    bool flickerEnabled;
    int flickerIntensity;
};

struct PomodoroConfigFile {
    int defaultWorkTimeInMinutes;
    int defaultPlayTimeInMinutes;
};

struct MarketConfigFile {
    int defaultRefreshIntervalInMinutes;
};

class StateMachine {
public:
    StateMachine(int default_tab);

    int getActiveTab() const;

    void setActiveTab(int newActiveTab);

    OsConfigFile &getOsConfig();

    PomodoroConfigFile &getPomodoroConfig();

    MarketConfigFile &getMarketConfig();

    bool healOsConfig();

    bool saveOsConfigToDisk(const std::vector<BaseConfigOptions> &configOptions);

    bool saveAppConfigToDisk(AppConfigTypes appConfigType, const std::vector<BaseConfigOptions>& configOptions);

private:
    int activeTab;
    OsConfigFile osConfigFile;
    PomodoroConfigFile pomodoroConfigFile;
    MarketConfigFile marketConfigFile;

    bool loadOsConfigFromDisk();

    bool loadDefaultOsConfig();

    bool loadPomodoroAppConfigFromDisk();

    bool savePomodoroConfigToDisk(const std::vector<BaseConfigOptions> &baseConfigOptions);

    bool loadMarketAppConfigFromDisk();

    bool saveMarketConfigToDisk(const std::vector<BaseConfigOptions> &baseConfigOptions);
};

#endif // STATE_MACHINE_HPP
