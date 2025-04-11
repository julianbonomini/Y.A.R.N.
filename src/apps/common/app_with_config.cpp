#include "app_with_config.hpp"
#include <sstream>

#include "../../ui/themes/theme_manager.hpp"


sf::FloatRect AppWithConfig::drawSettings() {
    sf::FloatRect box = getGridBox(0, 0, 5, 4);
    sf::RectangleShape modalRectangle({box.size.x, box.size.y});
    modalRectangle.setPosition({box.position.x, box.position.y});
    modalRectangle.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    modalRectangle.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    modalRectangle.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(modalRectangle);

    sf::Text title(font, appName + " SETTINGS");
    title.setCharacterSize(FontSizes::TITLE);
    title.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    title.setPosition({box.position.x + Layout::PADDING, box.position.y + Layout::PADDING});
    renderer.draw(title);

    return box;
}

void AppWithConfig::drawAppConfigOptions(const sf::FloatRect bounds) {
    float labelPositionX = bounds.position.x + Layout::PADDING * 2;
    float valuePositionX = bounds.position.x + Layout::PADDING * 2 + +Layout::LABEL_VALUE_SPACE;
    float verticalOffset = FontSizes::TITLE + Layout::PADDING * 2;

    // Loop through each config option dynamically
    for (size_t i = 0; i < configOptions.size(); ++i) {
        // Draw the label
        float valueYPosition = TOP_LEFT_ANCHOR.y + verticalOffset;
        BaseConfigOptions &currentOption = configOptions[i];
        sf::Text labelText(font, currentOption.label, FontSizes::LABEL);
        labelText.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
        labelText.setPosition({labelPositionX, valueYPosition});
        renderer.draw(labelText);

        // Draw the value for this config option
        if (currentOption.type == BaseConfigOptionType::TOGGLE) {
            float circleOffset = Layout::PADDING / 2;
            // Draw the outline circle (outer part)
            sf::CircleShape checkboxOutline(5.f); // Larger radius for the outline
            checkboxOutline.setFillColor(ThemeManager::instance().getCurrentTheme().background());
            checkboxOutline.setOutlineColor(ThemeManager::instance().getCurrentTheme().primary()); // Outline color
            checkboxOutline.setOutlineThickness(LineStyles::LINE_THICKNESS); // Outline thickness
            checkboxOutline.setPosition({valuePositionX, valueYPosition + circleOffset}); // Position the outline
            renderer.draw(checkboxOutline);

            // Draw the filled circle (inner part)
            if (currentOption.currentValue == "1") {
                sf::CircleShape checkboxFill(4.f); // Smaller radius for the fill (creates a gap between fill and outline)
                checkboxFill.setFillColor(ThemeManager::instance().getCurrentTheme().primary()); // Filled if value is "1"
                checkboxFill.setPosition({valuePositionX + 1.f, valueYPosition + circleOffset + 1.f}); // Adjust position for the gap
                renderer.draw(checkboxFill);
            }
        } else {
            std::stringstream valueStream;
            valueStream << currentOption.currentValue;
            sf::Text valueText(font, valueStream.str(), FontSizes::VALUE);
            valueText.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
            valueText.setPosition({valuePositionX, valueYPosition});
            renderer.draw(valueText);
        }

        if (currentOption.selected) {
            sf::FloatRect bounds = labelText.getGlobalBounds();
            sf::RectangleShape highlightRect;
            highlightRect.setPosition({bounds.position.x - 5.f, bounds.position.y - 5.f});
            highlightRect.setSize({bounds.size.x + 10.f, bounds.size.y + 10.f});  // small padding
            highlightRect.setFillColor(ThemeManager::instance().getCurrentTheme().highlight());
            renderer.draw(highlightRect);
            renderer.draw(labelText);
        }
        // Increment the vertical offset to position the next label/value pair
        verticalOffset += Layout::TEXT_SPACING;
    }

    drawCurrOptionHelpBox();

    // Draw if in changes:
    if (unsavedChangesFlag) {
        const float helpBoxWidth = Layout::MAIN_APP_WIDTH * 0.4;
        sf::Text unsavedChanges(font, "UNSAVED_CHANGES", FontSizes::HELP);
        unsavedChanges.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
        unsavedChanges.setPosition({BOTTOM_RIGHT_ANCHOR.x - helpBoxWidth - 100.f - Layout::PADDING, BOTTOM_LEFT_ANCHOR.y - Layout::PADDING});
        renderer.draw(unsavedChanges);
    }
}

void AppWithConfig::drawCurrOptionHelpBox() {
    auto contentAreaCoordinates = getGridBox(3, 0, 2, 2);
    std::string selectedOptionHelp = "";
    for (size_t i = 0; i < configOptions.size(); ++i) {
        if (configOptions[i].selected) {
            selectedOptionHelp = wrapText(configOptions[i].description, contentAreaCoordinates.size.x - Layout::PADDING * 2, FontSizes::DESCRIPTION);
            break;
        }
        selectedOptionHelp = "";
    }

    if (selectedOptionHelp == "") {
        return;
    }
    // --- Content area ---
    sf::RectangleShape contentArea({contentAreaCoordinates.size.x, contentAreaCoordinates.size.y});
    contentArea.setPosition({contentAreaCoordinates.position.x, contentAreaCoordinates.position.y});
    contentArea.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    contentArea.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    contentArea.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(contentArea);

    float textX = contentAreaCoordinates.position.x + Layout::PADDING;
    float textY = contentAreaCoordinates.position.y + Layout::PADDING;

    sf::Text title(font, "SETTING HELP");
    title.setCharacterSize(FontSizes::TITLE);
    title.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    title.setPosition({textX, textY});
    renderer.draw(title);

    sf::Text help(font, selectedOptionHelp);
    help.setCharacterSize(FontSizes::DESCRIPTION);
    help.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    help.setPosition({textX, title.getGlobalBounds().position.y + title.getGlobalBounds().size.y + Layout::PADDING});
    renderer.draw(help);

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
                case BaseConfigOptionType::TOGGLE: {
                    option.currentValue = std::stoi(option.currentValue) == 0 ? "1" : "0";
                    option.changed = true;
                    break;
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
                case BaseConfigOptionType::TOGGLE: {
                    option.currentValue = std::stoi(option.currentValue) == 0 ? "1" : "0";
                    option.changed = true;
                    break;
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
