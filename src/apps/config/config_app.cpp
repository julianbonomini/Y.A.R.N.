#include "config_app.hpp"

#include <iostream>
#include <sstream>

ConfigApp::ConfigApp(sf::RenderWindow &window, const sf::Font &font, StateMachine &stateMachine, const std::string &appName)
    : App(appName), window(window), font(font), stateMachine(stateMachine) {
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

void ConfigApp::drawHelpBox() {
    // --- Content area ---
    const float helpBoxWidth = Areas::MAIN_APP_WIDTH * 0.4;
    const float contentX = TOP_RIGHT.x - helpBoxWidth;
    const float contentY = TOP_RIGHT.y;

    // Draw box
    sf::RectangleShape contentArea(sf::Vector2f(helpBoxWidth, Areas::MAIN_APP_HEIGHT));
    contentArea.setPosition({contentX, contentY});
    contentArea.setFillColor(Colors::Background);
    contentArea.setOutlineColor(Colors::DarkerBackground); // light gray
    contentArea.setOutlineThickness(Lines::BOX_LINE_THICKNESS);
    window.draw(contentArea);

    // Help text
    std::vector<std::string> helpLines = {
        "<E>                   : Enter edit mode",
        "<ESC>                 : Cancel unsaved changes",
        "<ENTER>               : Save changes",
        "<UP|DOWN>             : Navigate options",
        "<LEFT|RIGHT>          : Change values"
    };

    float textX = contentX + Areas::PADDING;
    float textY = contentY + Areas::PADDING;

    for (const auto& line : helpLines) {
        sf::Text text(font, line, TextSizes::HELP);
        text.setFillColor(Colors::Text); // or maybe sf::Color::Green for terminal vibes
        text.setPosition({textX, textY});
        window.draw(text);
        textY += Areas::TEXT_SPACING; // spacing between lines
    }

}

void ConfigApp::draw() {
    float labelPositionX = TOP_LEFT.x + Areas::PADDING;
    float valuePositionX = TOP_LEFT.x + Areas::PADDING +  + Areas::LABEL_VALUE_SPACE;
    float verticalOffset = Areas::PADDING;

    drawHelpBox();

    // Loop through each config option dynamically
    for (size_t i = 0; i < configOptions.size(); ++i) {
        // Draw the label
        ConfigOption &currentOption = configOptions[i];
        sf::Text labelText(font, currentOption.label, TextSizes::LABEL);
        labelText.setFillColor(Colors::Text);
        labelText.setPosition({labelPositionX, TOP_LEFT.y + verticalOffset});
        window.draw(labelText);

        // Draw the value for this config option
        std::stringstream valueStream;
        valueStream << currentOption.currentValue;
        sf::Text valueText(font, valueStream.str(), TextSizes::VALUE);
        valueText.setFillColor(Colors::Text);
        valueText.setPosition({valuePositionX, TOP_LEFT.y + verticalOffset});
        // If selected, draw background rectangle behind value
        if (currentOption.selected) {
            sf::FloatRect bounds = valueText.getGlobalBounds();
            sf::RectangleShape highlightRect;
            highlightRect.setPosition({bounds.position.x - 5.f, bounds.position.y - 5.f});
            highlightRect.setSize({bounds.size.x + 10.f, bounds.size.y + 10.f});  // small padding
            highlightRect.setFillColor(Colors::DarkerBackground);
            window.draw(highlightRect);
        }
        window.draw(valueText);

        // Draw if it's active
        if (currentOption.selected) {
            // sf::RectangleShape editRecangle(sf::Vector2f(10.f, 10.f));
            // labelText.setPosition({labelPositionX, TOP_LEFT.y + verticalOffset});
            // labelText.setFillColor(Colors::Background);
            labelText.setOutlineColor(Colors::DarkerBackground);

            // debugRectangle.setPosition({window.getView().getCenter().x, window.getView().getCenter().y});
            // debugRectangle.setFillColor(sf::Color::Red);
            // debugRectangle.setOutlineThickness(Lines::BOX_LINE_THICKNESS);
            // window.draw(debugRectangle);
        }

        // Increment the vertical offset to position the next label/value pair
        verticalOffset += Areas::TEXT_SPACING;
    }


    // Draw if in edit mode:
    if (editModeEnabled) {
        sf::Text editModeEnabled(font, "EDIT_MODE", TextSizes::HELP);
        editModeEnabled.setFillColor(Colors::Text);
        editModeEnabled.setPosition({BOTTOM_LEFT.x + Areas::PADDING, BOTTOM_LEFT.y - Areas::PADDING});
        window.draw(editModeEnabled);
    }

    // Draw if in changes:
    if (unsavedChangesFlag) {
        const float helpBoxWidth = Areas::MAIN_APP_WIDTH * 0.4;
        sf::Text unsavedChanges(font, "UNSAVED_CHANGES", TextSizes::HELP);
        unsavedChanges.setFillColor(Colors::Text);
        unsavedChanges.setPosition({BOTTOM_RIGHT.x - helpBoxWidth - 100.f - Areas::PADDING, BOTTOM_LEFT.y - Areas::PADDING});
        window.draw(unsavedChanges);
    }
}

void ConfigApp::initConfigFromDisk() {
    configOptions = std::vector<ConfigOption>();
    // Refresh Rate
    ConfigOption refreshRateOption;
    refreshRateOption.label = "refresh_rate_hz";
    refreshRateOption.type = "dropdown";
    refreshRateOption.options = {"1", "30", "60"};
    refreshRateOption.currentValue = std::to_string(stateMachine.getOsConfig().refreshRate);
    refreshRateOption.selected = false;
    refreshRateOption.changed = false;
    configOptions.push_back(refreshRateOption);

    // Default Tab
    ConfigOption defaultTabOption;
    defaultTabOption.label = "default_tab";
    defaultTabOption.type = "dropdown";
    defaultTabOption.options = {"0", "1", "2", "3"};
    defaultTabOption.currentValue = std::to_string(stateMachine.getOsConfig().defaultTab);
    defaultTabOption.selected = false;
    defaultTabOption.changed = false;
    configOptions.push_back(defaultTabOption);
}
