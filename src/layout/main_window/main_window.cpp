#include "main_window.hpp"
#include <SFML/Graphics.hpp>
#include "../../globals/theme.hpp"


MainWindow::MainWindow(sf::RenderWindow &window, const sf::Font &font)
    : window(window), font(font) {
    // Constructor logic can go here (optional)
}

void MainWindow::draw(const std::vector<std::unique_ptr<App> > &apps, int activeTab) {
    // --- Tabs ---
    drawTabs(apps, activeTab);

    // --- Draw App Area (Constrain drawing to app area) ---
    drawAppArea();

    drawActiveApp(apps[activeTab].get());

    // --- Reset the view to default for the rest of the UI ---
    window.setView(window.getDefaultView()); // Reset to default view after app content
}

void MainWindow::drawTabs(const std::vector<std::unique_ptr<App> > &apps, int activeTab) {
    float textOffsetY = 6.f;
    for (size_t i = 0; i < apps.size(); ++i) {
        float x = Areas::PADDING + i * (Areas::TAB_WIDTH + Areas::PADDING);

        sf::RectangleShape tab(sf::Vector2f(Areas::TAB_WIDTH, Areas::TAB_HEIGHT));
        tab.setPosition({x, Areas::TOOLBAR_OFFSET});

        sf::Color bg = i == activeTab ? Colors::DarkerBackground : Colors::Background;
        tab.setFillColor(bg);
        window.draw(tab);

        sf::Text label(font, apps[i].get()->appName, TextSizes::TAB);
        label.setFillColor(Colors::Text);
        label.setPosition({x + Areas::PADDING, Areas::TOOLBAR_OFFSET + textOffsetY});
        window.draw(label);
    }
}

void MainWindow::drawAppArea() {
    // --- Content area ---
    float contentX = Areas::PADDING;
    float contentY = Areas::TOOLBAR_OFFSET + Areas::TAB_HEIGHT + Areas::PADDING;

    sf::RectangleShape contentArea(sf::Vector2f(Areas::MAIN_APP_WIDTH, Areas::MAIN_APP_HEIGHT));
    contentArea.setPosition({contentX, contentY});
    contentArea.setFillColor(Colors::Background);
    contentArea.setOutlineColor(Colors::DarkerBackground); // light gray
    contentArea.setOutlineThickness(Lines::BOX_LINE_THICKNESS);

    window.draw(contentArea);
}

void MainWindow::drawActiveApp(App *activeApp) {
    // Draw the app into that view
    activeApp->draw();
}
