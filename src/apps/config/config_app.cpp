#include "config_app.hpp"

#include <iostream>
#include <sstream>
#include <vector>

ConfigApp::ConfigApp(const std::string &appName, sf::RenderTarget &target, const sf::Font &font, StateMachine &stateMachine)
    : App(appName, target, font), stateMachine(stateMachine) {
    initConfigFromDisk();
}

void ConfigApp::moveDown() {
    for (size_t i = 0; i < configOptions.size(); ++i) {
        if (configOptions[i].selected) {
            configOptions[i].selected = false;
            size_t next = (i + 1) % configOptions.size(); // wrap around
            configOptions[next].selected = true;
            break;
        }
    }
}

void ConfigApp::moveUp() {
    for (size_t i = 0; i < configOptions.size(); ++i) {
        if (configOptions[i].selected) {
            configOptions[i].selected = false;
            size_t prev = (i == 0) ? configOptions.size() - 1 : i - 1;
            configOptions[prev].selected = true;
            break;
        }
    }
}

void ConfigApp::changeOptionRight() {
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

void ConfigApp::changeOptionLeft() {
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

void ConfigApp::handleEvent(const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.scancode == sf::Keyboard::Scan::E && !editModeEnabled) {
        editModeEnabled = true;
        configOptions[0].selected = true;
    }

    if (editModeEnabled) {
        if (keyPressed.scancode == sf::Keyboard::Scan::Down) {
            moveDown();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Right) {
            unsavedChangesFlag = true;
            changeOptionRight();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Left) {
            unsavedChangesFlag = true;
            changeOptionLeft();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Up) {
            moveUp();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Escape) {
            editModeEnabled = false;
            unsavedChangesFlag = false;
            initConfigFromDisk();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Enter) {
            stateMachine.saveOsConfigToDisk(configOptions);
            editModeEnabled = false;
            unsavedChangesFlag = false;
            initConfigFromDisk();
        }
    }
}

void ConfigApp::handleHelp() {
    if (helpOpen) {
        drawModalRectangle("HELP");
    }
}

void ConfigApp::handleSettings() {
    if (settingsOpen) {
        drawModalRectangle("SETTINGS");
    }
}

void ConfigApp::drawHelpBox() {
    // --- Content area ---
    auto contentAreaCoordinates = getGridBox(3, 0, 2, 4);
    sf::RectangleShape contentArea({contentAreaCoordinates.size.x, contentAreaCoordinates.size.y});
    contentArea.setPosition({contentAreaCoordinates.position.x, contentAreaCoordinates.position.y});
    contentArea.setFillColor(Colors::WHITE);
    contentArea.setOutlineColor(Colors::GRAY);
    contentArea.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(contentArea);

    // Help text
    std::vector<std::string> helpLines = {
        "<E>                   : Enter edit mode",
        "<ESC>                 : Cancel unsaved changes",
        "<ENTER>               : Save changes",
        "<UP|DOWN>             : Navigate options",
        "<LEFT|RIGHT>          : Change values"
    };

    float textX = contentAreaCoordinates.position.x + Layout::PADDING;
    float textY = contentAreaCoordinates.position.y + Layout::PADDING;

    for (const auto &line: helpLines) {
        sf::Text text(font, line);
        text.setCharacterSize(FontSizes::TITLE);
        text.setFillColor(Colors::BLACK);
        text.setPosition({textX, textY});
        renderer.draw(text);
        textY += Layout::TEXT_SPACING;
    }
}

void ConfigApp::draw() {
    float labelPositionX = TOP_LEFT_ANCHOR.x + Layout::PADDING;
    float valuePositionX = TOP_LEFT_ANCHOR.x + Layout::PADDING + +Layout::LABEL_VALUE_SPACE;
    float verticalOffset = Layout::PADDING;

    drawHelpBox();

    // Loop through each config option dynamically
    for (size_t i = 0; i < configOptions.size(); ++i) {
        // Draw the label
        float valueYPosition = TOP_LEFT_ANCHOR.y + verticalOffset;
        BaseConfigOptions &currentOption = configOptions[i];
        sf::Text labelText(font, currentOption.label, FontSizes::LABEL);
        labelText.setFillColor(Colors::BLACK);
        labelText.setPosition({labelPositionX, valueYPosition});
        renderer.draw(labelText);

        // Draw the value for this config option
        if (currentOption.type == BaseConfigOptionType::TOGGLE) {
            float circleOffset = Layout::PADDING / 2;
            // Draw the outline circle (outer part)
            sf::CircleShape checkboxOutline(5.f); // Larger radius for the outline
            // checkboxOutline.setFillColor(Colors::TRANSPARENT); // No fill for the outline
            checkboxOutline.setOutlineColor(Colors::BLACK); // Outline color
            checkboxOutline.setOutlineThickness(LineStyles::LINE_THICKNESS); // Outline thickness
            checkboxOutline.setPosition({valuePositionX, valueYPosition + circleOffset}); // Position the outline
            renderer.draw(checkboxOutline);

            // Draw the filled circle (inner part)
            if (currentOption.currentValue == "1") {
                sf::CircleShape checkboxFill(4.f); // Smaller radius for the fill (creates a gap between fill and outline)
                checkboxFill.setFillColor(Colors::BLACK); // Filled if value is "1"
                checkboxFill.setPosition({valuePositionX + 1.f, valueYPosition + circleOffset + 1.f}); // Adjust position for the gap
                renderer.draw(checkboxFill);
            }
        } else {
            std::stringstream valueStream;
            valueStream << currentOption.currentValue;
            sf::Text valueText(font, valueStream.str(), FontSizes::VALUE);
            valueText.setFillColor(Colors::BLACK);
            valueText.setPosition({valuePositionX, valueYPosition});
            renderer.draw(valueText);
        }

        if (currentOption.selected) {
            sf::FloatRect bounds = labelText.getGlobalBounds();
            sf::RectangleShape highlightRect;
            highlightRect.setPosition({bounds.position.x - 5.f, bounds.position.y - 5.f});
            highlightRect.setSize({bounds.size.x + 10.f, bounds.size.y + 10.f});  // small padding
            highlightRect.setFillColor(Colors::GRAY);
            renderer.draw(highlightRect);
            renderer.draw(labelText);
        }
        // Increment the vertical offset to position the next label/value pair
        verticalOffset += Layout::TEXT_SPACING;
    }


    // Draw if in edit mode:
    if (editModeEnabled) {
        sf::Text editModeEnabled(font, "EDIT_MODE", FontSizes::HELP);
        editModeEnabled.setFillColor(Colors::BLACK);
        editModeEnabled.setPosition({BOTTOM_LEFT_ANCHOR.x + Layout::PADDING, BOTTOM_LEFT_ANCHOR.y - Layout::PADDING});
        renderer.draw(editModeEnabled);
    }

    // Draw if in changes:
    if (unsavedChangesFlag) {
        const float helpBoxWidth = Layout::MAIN_APP_WIDTH * 0.4;
        sf::Text unsavedChanges(font, "UNSAVED_CHANGES", FontSizes::HELP);
        unsavedChanges.setFillColor(Colors::BLACK);
        unsavedChanges.setPosition({BOTTOM_RIGHT_ANCHOR.x - helpBoxWidth - 100.f - Layout::PADDING, BOTTOM_LEFT_ANCHOR.y - Layout::PADDING});
        renderer.draw(unsavedChanges);
    }

    handleHelp();
    handleSettings();
}

void ConfigApp::initConfigFromDisk() {
    configOptions = std::vector<BaseConfigOptions>();
    // Refresh Rate
    BaseConfigOptions refreshRateOption;
    refreshRateOption.label = "refresh_rate_hz";
    refreshRateOption.type = BaseConfigOptionType::CYCLE;;
    refreshRateOption.options = {"1", "30", "60"};
    refreshRateOption.currentValue = std::to_string(stateMachine.getOsConfig().refreshRate);
    refreshRateOption.selected = false;
    refreshRateOption.changed = false;
    configOptions.push_back(refreshRateOption);

    // Default Tab
    BaseConfigOptions defaultTabOption;
    defaultTabOption.label = "default_tab";
    defaultTabOption.type = BaseConfigOptionType::CYCLE;;
    defaultTabOption.options = {"0", "1", "2", "3"};
    defaultTabOption.currentValue = std::to_string(stateMachine.getOsConfig().defaultTab);
    defaultTabOption.selected = false;
    defaultTabOption.changed = false;
    configOptions.push_back(defaultTabOption);

    // Shader ON/OFF
    BaseConfigOptions shaderEnabled;
    shaderEnabled.label = "shader_enabled";
    shaderEnabled.type = BaseConfigOptionType::TOGGLE;;
    shaderEnabled.options = {};
    shaderEnabled.currentValue = std::to_string(stateMachine.getOsConfig().shaderEnabled);
    shaderEnabled.selected = false;
    shaderEnabled.changed = false;
    configOptions.push_back(shaderEnabled);

    // Shader
    std::vector<std::string> shaderFiles;
    std::string shaderDirectory = "src/ui/shaders";
    try {
        for (const auto& entry : std::filesystem::directory_iterator(shaderDirectory)) {
            if (std::filesystem::is_regular_file(entry)) {
                std::string filenameWithoutExtension = entry.path().stem().string();
                shaderFiles.push_back(filenameWithoutExtension);
            }
        }
    } catch (const std::exception& e) {
        app_log("Error reading shader directory: ", e.what());
    }
    BaseConfigOptions shader;
    shader.label = "shader";
    shader.type = BaseConfigOptionType::CYCLE;;
    shader.options = shaderFiles;
    shader.currentValue = stateMachine.getOsConfig().shader;
    shader.selected = false;
    shader.changed = false;
    configOptions.push_back(shader);

    // Flicker enabled
    BaseConfigOptions flickerEnabled;
    flickerEnabled.label = "flicker_enabled";
    flickerEnabled.type = BaseConfigOptionType::TOGGLE;;
    flickerEnabled.options = {};
    flickerEnabled.currentValue = std::to_string(stateMachine.getOsConfig().flickerEnabled);
    flickerEnabled.selected = false;
    flickerEnabled.changed = false;
    configOptions.push_back(flickerEnabled);

    // Shader
    BaseConfigOptions flickerIntensity;
    flickerIntensity.label = "flicker_intensity";
    flickerIntensity.type = BaseConfigOptionType::FREE_NUMBER;;
    flickerIntensity.options = {};
    flickerIntensity.currentValue = std::to_string(stateMachine.getOsConfig().flickerIntensity);
    flickerIntensity.selected = false;
    flickerIntensity.changed = false;
    configOptions.push_back(flickerIntensity);
}

void ConfigApp::saveConfigToDisk() {
    stateMachine.saveOsConfigToDisk(configOptions);
}
