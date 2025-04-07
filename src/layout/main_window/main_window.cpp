#include "main_window.hpp"
#include <SFML/Graphics.hpp>
#include "../../globals/theme.hpp"

float padding = 8.f;
float tabHeight = 30.f;
float topOffset = 40.f + padding;

MainWindow::MainWindow(sf::RenderWindow& window, const sf::Font& font)
    : window(window), font(font) {
    // Constructor logic can go here (optional)
}

void MainWindow::draw(const std::vector<std::unique_ptr<App>>& apps, int activeTab) {
    // --- Tabs ---
    drawTabs(apps, activeTab);

    // --- Draw App Area (Constrain drawing to app area) ---
    drawAppArea();

    drawActiveApp(apps[activeTab].get());

    // --- Reset the view to default for the rest of the UI ---
    window.setView(window.getDefaultView());  // Reset to default view after app content
}

void MainWindow::drawTabs(const std::vector<std::unique_ptr<App>>& apps, int activeTab) {
    float tabWidth = 120.f;
    float textOffsetY = 6.f;
    for (size_t i = 0; i < apps.size(); ++i) {
        float x = padding + i * (tabWidth + padding);

        sf::RectangleShape tab(sf::Vector2f(tabWidth, tabHeight));
        tab.setPosition({x, topOffset});

        sf::Color bg = (i == activeTab) ? Theme::DarkerBackground : Theme::Background;
        tab.setFillColor(bg);
        window.draw(tab);

        sf::Text label(font, apps[i].get()->appName, 12);
        label.setFillColor(Theme::Text);
        label.setPosition({x + padding, topOffset + textOffsetY});
        window.draw(label);
    }
}

void MainWindow::drawAppArea() {
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

void MainWindow::drawActiveApp(App * activeApp) {
    // --- Set view for the active app's space ---
    // --- Set view for the active app's space ---
    float contentX = padding;
    float contentY = topOffset + tabHeight + padding;
    float contentWidth = Theme::WINDOW_WIDTH - ( 2 * padding );
    float contentHeight = Theme::WINDOW_WIDTH - ( 2 * padding ) - contentY;

    // Create a view centered at (contentX + contentWidth / 2, contentY + contentHeight / 2)
    // with size (contentWidth, contentHeight)
    sf::Vector2f center(Theme::WINDOW_WIDTH / 2, Theme::WINDOW_HEIGHT / 2);
    sf::Vector2f size(contentWidth, contentHeight);
    // Create the view with the center and size
    sf::View appView(center, size);
    appView.setCenter(center);
    appView.setSize(size);
    // Set the view for the active app's space
    window.setView(appView);
    // Draw the active app in the constrained area
    activeApp->draw();  // Draw your app (InfoApp, etc.)
}