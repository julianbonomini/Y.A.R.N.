#include<iostream>
#include "info_app.hpp"
#include <SFML/Graphics.hpp>
#include <string>

#include "../../core/execute/execute_utils.hpp"

InfoApp::InfoApp(const std::string &appName, sf::RenderTarget &renderer, const sf::Font &font)
    : App(appName, renderer, font) {
    infoData.push_back({"OS:", "Noop"});
    infoData.push_back({"VERSION:", "0.0.1"});
    infoData.push_back({"CPU:", ExecuteUtils::getCpuModel()});
    infoData.push_back({"CPU_USG:", "%"});
    infoData.push_back({"GPU:", ExecuteUtils::getGpuModel()});
    infoData.push_back({"RAM:", ExecuteUtils::getRam()});
    infoData.push_back({"USED STORAGE:", ExecuteUtils::getStorageUsage("/")});
    infoData.push_back({"APPS:", "NOOP"});
    infoData.push_back({"UPTIME:", ExecuteUtils::getUptime()});
    infoData.push_back({"NETWORK:", ExecuteUtils::getNetworkStatus()});
}

void InfoApp::handleEvent(const sf::Event::KeyPressed &keyPressed) {
}

void InfoApp::handleHelp() {
    if (helpOpen) {
        drawModalRectangle("HELP");
    }
}

void InfoApp::handleSettings() {
    if (settingsOpen) {
        drawModalRectangle("SETTINGS");
    }
}

void InfoApp::draw() {
    infoData[3] = {"CPU_USG:", ExecuteUtils::getCpuUsage()};
    float labelPositionX = TOP_LEFT_ANCHOR.x + Layout::PADDING;
    float valuePositionX = TOP_LEFT_ANCHOR.x + Layout::PADDING + Layout::LABEL_VALUE_SPACE;
    float verticalOffset = Layout::PADDING;

    // Iterate over all key-value pairs
    for (const auto &entry: infoData) {
        sf::Text labelText(font, entry.first);
        labelText.setPosition({labelPositionX, TOP_LEFT_ANCHOR.y + verticalOffset});
        labelText.setFillColor(Colors::BLACK);
        labelText.setCharacterSize(FontSizes::LABEL);

        sf::Text valueText(font, entry.second);
        valueText.setPosition({valuePositionX, TOP_LEFT_ANCHOR.y + verticalOffset});
        valueText.setFillColor(Colors::BLACK);
        valueText.setCharacterSize(FontSizes::LABEL);

        // Draw label and value
        renderer.draw(labelText);
        renderer.draw(valueText);

        // Increase the vertical offset for the next pair
        verticalOffset += Layout::TEXT_SPACING; // Adjust spacing between each pair
    }

    if (isSettingsOpen()) {
        handleSettings();
    }
    if (isHelpOpen()) {
        handleHelp();
    }
}
