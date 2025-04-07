#include "app_space.hpp"
#include <SFML/Graphics.hpp>
#include "../theme.hpp"

void AppSpace::draw(sf::RenderWindow& window, const sf::Font& font, const std::vector<std::string>& tabs, int activeTab) {
    float padding = 8.f;
    float tabWidth = 120.f;
    float tabHeight = 30.f;
    float topOffset = 40.f + padding;  // Toolbar height + padding
    float textOffsetY = 6.f;

    // --- Tabs ---
    for (size_t i = 0; i < tabs.size(); ++i) {
        float x = padding + i * (tabWidth + padding);

        sf::RectangleShape tab(sf::Vector2f(tabWidth, tabHeight));
        tab.setPosition({x, topOffset});

        sf::Color bg = (i == activeTab) ? Theme::DarkerBackground : Theme::Background;
        tab.setFillColor(bg);
        window.draw(tab);

        sf::Text label(font, tabs[i], 12);
        label.setFillColor(Theme::Text);
        label.setPosition({x + padding, topOffset + textOffsetY});
        window.draw(label);
    }

    // --- Content area ---
    float contentX = padding;
    float contentY = topOffset + tabHeight + padding;
    float contentWidth = window.getSize().x - 2 * padding;
    float contentHeight = window.getSize().y - contentY - padding;

    sf::RectangleShape contentArea(sf::Vector2f(contentWidth, contentHeight));
    contentArea.setPosition({contentX, contentY});
    contentArea.setFillColor(Theme::Background);
    contentArea.setOutlineColor(Theme::DarkerBackground);  // light gray
    contentArea.setOutlineThickness(2.f);

    window.draw(contentArea);
}
