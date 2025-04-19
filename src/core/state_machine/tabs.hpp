#ifndef TABS_H
#define TABS_H

#include<iostream>
#include <array>
#include <vector>

// This needs to match the order in TabNames
enum class Tab {
    MKT,
    PMD,
    WTH,
    INF,
    CNF,
    COUNT // Utility to track the number of tabs
};

// Manual mapping but safe
constexpr std::array<const char*, static_cast<size_t>(Tab::COUNT)> TabNames = {
    "MARKET", "POMODORO", "WEATHER", "INFO", "CONFIG" // Used in UI and config files
};

class Tabs {
public:

    static int totalTabs() {
        return static_cast<int>(Tab::COUNT);
    }

    static int tabToIndex(Tab tab) {
        return static_cast<int>(tab);
    }

    static std::vector<std::string> getAllTabNames() {
        return std::vector<std::string>(TabNames.begin(), TabNames.end());
    }

    static const std::string tabToString(Tab tab) {
        const std::vector<std::string> &tabNames = getAllTabNames();
        return tabNames[tabToIndex(tab)];
    }

    static Tab stringToTab(const std::string &name) {
        static const std::unordered_map<std::string, Tab> nameToTab = {
            {"MARKET", Tab::MKT},
            {"WEATHER", Tab::WTH},
            {"POMODORO", Tab::PMD},
            {"INFO", Tab::INF},
            {"CONFIG", Tab::CNF}
        };
        return nameToTab.at(name); // May throw if not found
    }

    static Tab nextTab(Tab currentTab) {
        // (stateMachine.getActiveTab() - 1 + apps.size()) % apps.size();
        int currentIndex = static_cast<int>(currentTab);
        return static_cast<Tab>((currentIndex + 1 + totalTabs() ) % totalTabs());
    }

    static Tab previousTab(Tab currentTab) {
        int currentIndex = static_cast<int>(currentTab);
        return static_cast<Tab>((currentIndex - 1 + totalTabs() ) % totalTabs());
    }

};
#endif //TABS_H
