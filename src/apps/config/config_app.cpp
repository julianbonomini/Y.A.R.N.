#include "config_app.hpp"

#include <iostream>
#include <sstream>

ConfigApp::ConfigApp(sf::RenderTarget &target, const sf::Font &font, StateMachine &stateMachine, const std::string &appName)
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
    for (auto& option : configOptions) {
        if (option.selected && !option.options.empty()) {
            auto it = std::find(option.options.begin(), option.options.end(), option.currentValue);
            if (it != option.options.end()) {
                size_t index = std::distance(option.options.begin(), it);
                index = (index + 1) % option.options.size(); // wrap around
                option.currentValue = option.options[index];
                option.changed = true;
            }
            break;
        }
    }
}

void ConfigApp::changeOptionLeft() {
    for (auto& option : configOptions) {
        if (option.selected && !option.options.empty()) {
            auto it = std::find(option.options.begin(), option.options.end(), option.currentValue);
            if (it != option.options.end()) {
                size_t index = std::distance(option.options.begin(), it);
                index = (index == 0) ? option.options.size() - 1 : index - 1; // wrap around
                option.currentValue = option.options[index];
                option.changed = true;
            }
            break;
        }
    }
}

void ConfigApp::handleEvent(const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.scancode == sf::Keyboard::Scan::E) {
        editModeEnabled = true;
        unsavedChangesFlag = true;
        configOptions[0].selected = true;
    } else if (keyPressed.scancode == sf::Keyboard::Scan::Down && editModeEnabled) {
        moveDown();
    } else if (keyPressed.scancode == sf::Keyboard::Scan::Right && editModeEnabled) {
        changeOptionRight();
    } else if (keyPressed.scancode == sf::Keyboard::Scan::Left && editModeEnabled) {
        changeOptionLeft();
    } else if (keyPressed.scancode == sf::Keyboard::Scan::Up && editModeEnabled) {
        moveUp();
    } else if (keyPressed.scancode == sf::Keyboard::Scan::Escape && editModeEnabled) {
        editModeEnabled = false;
        unsavedChangesFlag = false;
        initConfigFromDisk();
    } else if (keyPressed.scancode == sf::Keyboard::Scan::Enter && editModeEnabled) {
        stateMachine.saveOsConfigToDisk(configOptions);
        editModeEnabled = false;
        unsavedChangesFlag = false;
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

    for (const auto& line : helpLines) {
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
    float valuePositionX = TOP_LEFT_ANCHOR.x + Layout::PADDING +  + Layout::LABEL_VALUE_SPACE;
    float verticalOffset = Layout::PADDING;

    drawHelpBox();

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
            highlightRect.setSize({bounds.size.x + 10.f, bounds.size.y + 10.f});  // small padding
            highlightRect.setFillColor(Colors::GRAY);
            renderer.draw(highlightRect);
        }
        renderer.draw(valueText);

        // Draw if it's active
        if (currentOption.selected) {
            // sf::RectangleShape editRecangle(sf::Vector2f(10.f, 10.f));
            // labelText.setPosition({labelPositionX, TOP_LEFT.y + verticalOffset});
            // labelText.setFillColor(Colors::Background);
            labelText.setOutlineColor(Colors::GRAY);

            // debugRectangle.setPosition({renderer.getView().getCenter().x, renderer.getView().getCenter().y});
            // debugRectangle.setFillColor(sf::Color::Red);
            // debugRectangle.setOutlineThickness(Lines::BOX_LINE_THICKNESS);
            // renderer.draw(debugRectangle);
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
}

void ConfigApp::saveConfigToDisk() {
    stateMachine.saveOsConfigToDisk(configOptions);
}
