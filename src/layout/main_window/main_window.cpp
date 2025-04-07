#include "main_window.hpp"

#include <SFML/Graphics.hpp>

#include "../../core/state_machine.hpp"
#include "../../globals/theme.hpp"


MainWindow::MainWindow(sf::RenderWindow &window, const sf::Font &font, StateMachine &stateMachine)
    : window(window), font(font), stateMachine(stateMachine) {
    // Constructor logic can go here (optional)
}

void MainWindow::draw(const std::vector<std::unique_ptr<App>>& apps) {
    // --- Tabs ---
    drawTabs(apps);

    // --- Draw App Area (Constrain drawing to app area) ---
    drawAppArea();

    drawActiveApp(apps[stateMachine.getActiveTab()].get());

    // --- Reset the view to default for the rest of the UI ---
    window.setView(window.getDefaultView());  // Reset to default view after app content
}

void MainWindow::drawTabs(const std::vector<std::unique_ptr<App>>& apps) {
    float textOffsetY = 6.f;
    for (size_t i = 0; i < apps.size(); ++i) {
        float x = Theme::PADDING + i * (Theme::TAB_WIDTH + Theme::PADDING);

        sf::RectangleShape tab(sf::Vector2f(Theme::TAB_WIDTH, Theme::TAB_HEIGHT));
        tab.setPosition({x, Theme::TOOLBAR_OFFSET});

        sf::Color bg = i == stateMachine.getActiveTab() ? Colors::DarkerBackground : Colors::Background;
        tab.setFillColor(bg);
        window.draw(tab);

        sf::Text label(font, apps[i].get()->appName, TextSizes::TITLE);
        label.setFillColor(Colors::Text);
        label.setPosition({x + Theme::PADDING, Theme::TOOLBAR_OFFSET + textOffsetY});
        window.draw(label);
    }
}

void MainWindow::drawAppArea() {
    // --- Content area ---
    float contentX = Theme::PADDING;
    float contentY = Theme::TOOLBAR_OFFSET + Theme::TAB_HEIGHT + Theme::PADDING;
    float contentWidth = Theme::WINDOW_WIDTH - 2 * Theme::PADDING;
    float contentHeight = Theme::WINDOW_HEIGHT - contentY - Theme::PADDING;

    sf::RectangleShape contentArea(sf::Vector2f(contentWidth, contentHeight));
    contentArea.setPosition({contentX, contentY});
    contentArea.setFillColor(Colors::Background);
    contentArea.setOutlineColor(Colors::DarkerBackground);  // light gray
    contentArea.setOutlineThickness(2.f);

    window.draw(contentArea);
}

    void MainWindow::drawActiveApp(App* activeApp) {
    // Draw the app into that view
    activeApp->draw();
}