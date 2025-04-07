#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <string>

struct OsConfig {
    int refreshRate;
};

class StateMachine {
public:
    StateMachine(int default_tab);

    int getActiveTab() const;
    void setActiveTab(int newActiveTab);

    OsConfig& getOsConfig();
    const OsConfig& getOsConfig() const;

    // function to save/load configuration
    bool loadConfigFromDisk(const std::string& path);
    bool saveConfigToDisk(const std::string &path) const;

private:
    int activeTab;
    OsConfig osConfig;
};

#endif // STATE_MACHINE_HPP
