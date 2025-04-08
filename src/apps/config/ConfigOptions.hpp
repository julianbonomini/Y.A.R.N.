struct ConfigOption {
    std::string label; // The label/name of the setting (e.g., "Refresh Rate")
    std::string type; // Type of control: "dropdown", "checkbox", etc.
    std::vector<std::string> options; // Options for dropdown or list (e.g., ["60", "120", "144"])
    std::string currentValue; // Current selected value, in string form

    // State for UI
    bool selected;
    bool changed;
};