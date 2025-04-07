#include "app_space.hpp"
#include <SFML/Graphics.hpp>
#include "../theme.hpp"

float padding = 8.f;
float tabHeight = 30.f;
float topOffset = 40.f + padding;

void AppSpace::draw(sf::RenderWindow& window, const sf::Font& font, const std::vector<std::unique_ptr<App>>& apps, int activeTab) {
    // --- Tabs ---
    drawTabs(window, font, apps, activeTab);

    // --- App Area ---
    drawAppArea(window, apps, activeTab);  // Pass activeTab to the app area drawing function
}

void AppSpace::drawTabs(sf::RenderWindow& window, const sf::Font& font, const std::vector<std::unique_ptr<App>>& apps, int activeTab) {
    float tabWidth = 120.f;
    float textOffsetY = 6.f;

    for (size_t i = 0; i < apps.size(); ++i) {
        float x = padding + i * (tabWidth + padding);

        sf::RectangleShape tab(sf::Vector2f(tabWidth, tabHeight));
        tab.setPosition({x, topOffset});

        // Retro tab coloring: Active tab has a darker background
        sf::Color bg = (i == activeTab) ? Theme::DarkerBackground : Theme::Background;
        tab.setFillColor(bg);
        window.draw(tab);

        sf::Text label(font, apps[i].get()->appName, 12);
        label.setFillColor(Theme::Text);
        label.setPosition({x + padding, topOffset + textOffsetY});
        window.draw(label);
    }
}

void AppSpace::drawAppArea(sf::RenderWindow& window, const std::vector<std::unique_ptr<App>>& apps, int activeTab) {
    // --- Content area ---
    float contentX = padding;
    float contentY = topOffset + tabHeight + padding;
    float contentWidth = window.getSize().x - 2 * padding;
    float contentHeight = window.getSize().y - contentY - padding;

    sf::RectangleShape contentArea(sf::Vector2f(contentWidth, contentHeight));
    contentArea.setPosition({contentX, contentY});
    contentArea.setFillColor(Theme::Background);
    contentArea.setOutlineColor(Theme::DarkerBackground);  // Light gray border
    contentArea.setOutlineThickness(2.f);

    window.draw(contentArea);

    // --- Draw the active app inside the content area ---
    if (activeTab >= 0 && activeTab < apps.size()) {
        // You can now call `draw` for the specific app that should be active.
        // For example, call your InfoApp's draw method or whatever app is in the activeTab.
        apps[activeTab].get()->draw();
    }
}
