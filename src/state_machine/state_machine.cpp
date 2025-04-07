#include "state_machine.hpp"

// Constructor initializing the activeTab to 0 (first tab).
StateMachine::StateMachine(int default_tab) : activeTab(default_tab) {}

// Getter for activeTab.
int StateMachine::getActiveTab() const {
    return activeTab;
}

// Setter for activeTab.
void StateMachine::setActiveTab(int newActiveTab) {
    activeTab = newActiveTab;
}
