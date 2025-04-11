#include "app_with_config.hpp"
#include <sstream>


sf::FloatRect AppWithConfig::drawSettings() const {
    sf::RectangleShape modalRectangle({Layout::MODAL_WIDTH, Layout::MODAL_HEIGHT});
    modalRectangle.setPosition({TOP_LEFT_MODAL_ANCHOR.x, TOP_LEFT_MODAL_ANCHOR.y});
    modalRectangle.setFillColor(Colors::WHITE);
    modalRectangle.setOutlineColor(Colors::GRAY);
    modalRectangle.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(modalRectangle);
    sf::FloatRect bounds = modalRectangle.getGlobalBounds();

    sf::Text title(font, appName + " SETTINGS");
    title.setCharacterSize(FontSizes::TITLE);
    title.setFillColor(Colors::BLACK);
    title.setPosition({TOP_LEFT_MODAL_ANCHOR.x + Layout::PADDING, TOP_LEFT_MODAL_ANCHOR.y + Layout::PADDING});
    renderer.draw(title);

    return bounds;
}

void AppWithConfig::drawAppConfigOptions(const sf::FloatRect bounds) {
    float labelPositionX = bounds.position.x + Layout::PADDING * 2;
    float valuePositionX = bounds.position.x + Layout::PADDING * 2 + +Layout::LABEL_VALUE_SPACE;
    float verticalOffset = Layout::PADDING;
    // Loop through each config option dynamically
    for (size_t i = 0; i < configOptions.size(); ++i) {
        // Draw the label
        BaseConfigOptions &currentOption = configOptions[i];
        sf::Text labelText(font, currentOption.label, FontSizes::LABEL);
        labelText.setFillColor(Colors::BLACK);
        labelText.setPosition({labelPositionX, TOP_LEFT_ANCHOR.y + verticalOffset});
        renderer.draw(labelText);

        // Draw the value for this config option
        std::stringstream valueStream;
        valueStream << currentOption.currentValue;
        sf::Text valueText(font, valueStream.str(), FontSizes::VALUE);
        valueText.setFillColor(Colors::BLACK);
        valueText.setPosition({valuePositionX, TOP_LEFT_ANCHOR.y + verticalOffset});
        // If selected, draw background rectangle behind value
        if (currentOption.selected) {
            sf::FloatRect bounds = valueText.getGlobalBounds();
            sf::RectangleShape highlightRect;
            highlightRect.setPosition({bounds.position.x - 5.f, bounds.position.y - 5.f});
            highlightRect.setSize({bounds.size.x + 10.f, bounds.size.y + 10.f}); // small padding
            highlightRect.setFillColor(Colors::GRAY);
            renderer.draw(highlightRect);
        }
        renderer.draw(valueText);

        // Draw if it's active
        if (currentOption.selected) {
            labelText.setOutlineColor(Colors::GRAY);
        }

        // Increment the vertical offset to position the next label/value pair
        verticalOffset += Layout::TEXT_SPACING;
    }

    // Draw if in changes:
    if (unsavedChangesFlag) {
        sf::Text unsavedChanges(font, "UNSAVED_CHANGES", FontSizes::HELP);
        unsavedChanges.setFillColor(Colors::BLACK);
        unsavedChanges.setPosition({labelPositionX, bounds.position.y + bounds.size.y - unsavedChanges.getGlobalBounds().size.y - Layout::PADDING});
        renderer.draw(unsavedChanges);
    }
}

void AppWithConfig::moveDown() {
    for (size_t i = 0; i < configOptions.size(); ++i) {
        if (configOptions[i].selected) {
            configOptions[i].selected = false;
            size_t next = (i + 1) % configOptions.size(); // wrap around
            configOptions[next].selected = true;
            break;
        }
    }
}

void AppWithConfig::moveUp() {
    for (size_t i = 0; i < configOptions.size(); ++i) {
        if (configOptions[i].selected) {
            configOptions[i].selected = false;
            size_t prev = (i == 0) ? configOptions.size() - 1 : i - 1;
            configOptions[prev].selected = true;
            break;
        }
    }
}

void AppWithConfig::changeOptionRight() {
    unsavedChangesFlag = true;
    for (auto &option: configOptions) {
        if (option.selected) {
            switch (option.type) {
                case BaseConfigOptionType::CYCLE: {
                    if (!option.options.empty()) {
                        auto it = std::find(option.options.begin(), option.options.end(), option.currentValue);
                        if (it != option.options.end()) {
                            size_t index = std::distance(option.options.begin(), it);
                            index = (index + 1) % option.options.size(); // wrap around
                            option.currentValue = option.options[index];
                            option.changed = true;
                        }
                    }
                    break;
                }
                case BaseConfigOptionType::FREE_NUMBER: {
                    int currValueInt = std::stoi(option.currentValue);
                    if (currValueInt >= 99) {
                        break;
                    }
                    option.currentValue = std::to_string(currValueInt + 1);
                    option.changed = true;
                    break;
                }
                case BaseConfigOptionType::CHECKBOX: {
                    // You can define your own logic here.
                    // For example, maybe prompt user input or increment a numeric field.
                    std::cout << "Checkbox not implemented" << std::endl;
                }
                default:
                    break;
            }
        }
    }
}

void AppWithConfig::changeOptionLeft() {
    unsavedChangesFlag = true;
    for (auto &option: configOptions) {
        if (option.selected) {
            switch (option.type) {
                case BaseConfigOptionType::CYCLE: {
                    if (!option.options.empty()) {
                        auto it = std::find(option.options.begin(), option.options.end(), option.currentValue);
                        if (it != option.options.end()) {
                            size_t index = std::distance(option.options.begin(), it);
                            index = (index == 0) ? option.options.size() - 1 : index - 1; // wrap around
                            option.currentValue = option.options[index];
                            option.changed = true;
                        }
                    }
                    break;
                }
                case BaseConfigOptionType::FREE_NUMBER: {
                    int currValueInt = std::stoi(option.currentValue);
                    if (currValueInt <= 1) {
                        break;
                    }
                    option.currentValue = std::to_string(currValueInt - 1);
                    option.changed = true;
                    break;
                }
                case BaseConfigOptionType::CHECKBOX: {
                    // You can define your own logic here.
                    // For example, maybe prompt user input or increment a numeric field.
                    std::cout << "Checkbox not implemented" << std::endl;
                }
                default:
                    break;
            }
        }
    }
}

void AppWithConfig::closeWithoutChanges() {
    app_log("Closing config - no changes");
    setSettingsOpen(false);
    setHasOpenModal(false);
    unsavedChangesFlag = false;
}

void AppWithConfig::closeWithUnsavedChanges() {
    app_log("Closing config w/unsaved changes - reset from disk");
    setSettingsOpen(false);
    setHasOpenModal(false);
    unsavedChangesFlag = false;
    initConfigFromDisk();
}

void AppWithConfig::saveAndClose(AppConfigTypes appConfigType, const std::vector<BaseConfigOptions> &toBeSaved) {
    app_log("Saving config to disk - reset from disk");
    stateMachine.saveAppConfigToDisk(appConfigType, toBeSaved);
    unsavedChangesFlag = false;
    initConfigFromDisk();
    setSettingsOpen(false);
    setHasOpenModal(false);
}
