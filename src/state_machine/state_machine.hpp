#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <iostream>

class StateMachine {
public:
    // Constructor to initialize the activeTab value.
    StateMachine(int default_tab);

    // Get the current active tab.
    int getActiveTab() const;

    // Set the active tab to a new value.
    void setActiveTab(int newActiveTab);

private:
    int activeTab; // Tracks the current active tab.
};

#endif // STATE_MACHINE_HPP
