#include<iostream>


enum class BaseConfigOptionType {
    CYCLE,
    TOGGLE,
    FREE_NUMBER
};

struct BaseConfigOptions {
    std::string label; // The label/name of the setting (e.g., "Refresh Rate")
    BaseConfigOptionType type; // Type of control: "cycle", "checkbox", etc.
    std::vector<std::string> options; // Options for cycle or list (e.g., ["60", "120", "144"])
    std::string currentValue; // Current selected value, in string form

    // State for UI
    bool selected;
    bool changed;
};