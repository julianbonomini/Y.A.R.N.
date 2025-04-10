#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP
#include <vector>
#include "../apps/common/BaseConfigOptions.hpp"

enum class AppConfigTypes {
    POMODORO,
    MARKET,
    WEATHER,
};

struct OsConfigFile {
    int refreshRate;
    int defaultTab;
};

struct PomodoroConfigFile {
    int defaultWorkTimeInMinutes;
    int defaultPlayTimeInMinutes;
};

class StateMachine {
public:
    StateMachine(int default_tab);

    int getActiveTab() const;

    void setActiveTab(int newActiveTab);

    OsConfigFile &getOsConfig();

    PomodoroConfigFile &getPomodoroConfig();

    const OsConfigFile &getOsConfig() const;

    bool saveOsConfigToDisk(const std::vector<BaseConfigOptions> &configOptions);

    bool saveAppConfigToDisk(AppConfigTypes appConfigType, const std::vector<BaseConfigOptions>& configOptions);

private:
    int activeTab;
    OsConfigFile osConfigFile;
    PomodoroConfigFile pomodoroConfigFile;

    // function to save/load os configuration
    bool loadOsConfigFromDisk();

    bool loadPomodoroAppConfigFromDisk();

    bool savePomodoroConfigToDisk(const std::vector<BaseConfigOptions> &baseConfigOptions);
};

#endif // STATE_MACHINE_HPP
