#include "main_window.hpp"
#include <SFML/Graphics.hpp>
#include "../../globals/ui_globals.hpp"
#include "../../ui/utils/ui_helpers.hpp"


MainWindow::MainWindow(sf::RenderTarget &renderer, const sf::Font &font)
    : renderer(renderer), font(font) {
    // Constructor logic can go here (optional)
}

void MainWindow::draw(const std::vector<std::unique_ptr<App> > &apps, int activeTab) {
    // --- Draw App Area (Constrain drawing to app area) ---
    drawAppArea();

    // --- Tabs ---
    // Needs to be after App Area because it has to cover it
    drawTabs(apps, activeTab);

    drawActiveApp(apps[activeTab].get());
}

void MainWindow::drawTabs(const std::vector<std::unique_ptr<App> > &apps, int activeTab) {
    float textOffsetY = 6.f;
    for (size_t i = 0; i < apps.size(); ++i) {
        float startingXPosition = Layout::PADDING + i * (Layout::TAB_WIDTH + Layout::PADDING);

        sf::ConvexShape tabOutline;
        tabOutline.setPointCount(6);

        float curve = 8.f;

        tabOutline.setPoint(0, {startingXPosition, Layout::TOOLBAR_OFFSET + Layout::TAB_HEIGHT + textOffsetY}); // bottom left
        tabOutline.setPoint(1, {startingXPosition, Layout::TOOLBAR_OFFSET + curve}); // left curve start
        tabOutline.setPoint(2, {startingXPosition + curve, Layout::TOOLBAR_OFFSET}); // top left curve
        tabOutline.setPoint(3, {startingXPosition + Layout::TAB_WIDTH - curve, Layout::TOOLBAR_OFFSET}); // top right curve
        tabOutline.setPoint(4, {startingXPosition + Layout::TAB_WIDTH, Layout::TOOLBAR_OFFSET + curve}); // right curve end
        tabOutline.setPoint(5, {startingXPosition + Layout::TAB_WIDTH, Layout::TOOLBAR_OFFSET + Layout::TAB_HEIGHT + textOffsetY}); // bottom right
        tabOutline.setOutlineThickness(LineStyles::LINE_THICKNESS);
        tabOutline.setOutlineColor(Colors::SECONDARY);
        // tabOutline.setFillColor(Colors::GRAY);


        if (i == activeTab) {
            renderer.draw(tabOutline);
            sf::RectangleShape coverAreaLine(sf::Vector2f(Layout::TAB_WIDTH, LineStyles::LINE_THICKNESS));
            coverAreaLine.setPosition({startingXPosition, Layout::TOOLBAR_OFFSET + Layout::TAB_HEIGHT + Layout::PADDING - 2.f});
            coverAreaLine.setFillColor(Colors::BACKGROUND);
            renderer.draw(coverAreaLine);
        }

        sf::Text label(font, apps[i].get()->appName, FontSizes::TAB);
        label.setFillColor(i == activeTab ? Colors::PRIMARY : Colors::SECONDARY);
        sf::Vector2f labelPosition = UIHelpers::snapToGrid({startingXPosition + Layout::PADDING, Layout::TOOLBAR_OFFSET + textOffsetY});
        label.setPosition(labelPosition);
        renderer.draw(label);
    }
}

void MainWindow::drawAppArea() {
    // --- Content area ---
    float contentX = Layout::PADDING;
    float contentY = Layout::TOOLBAR_OFFSET + Layout::TAB_HEIGHT + Layout::PADDING;

    sf::RectangleShape contentArea(sf::Vector2f(Layout::MAIN_APP_WIDTH, Layout::MAIN_APP_HEIGHT));
    contentArea.setPosition({contentX, contentY});
    contentArea.setFillColor(Colors::BACKGROUND);
    contentArea.setOutlineColor(Colors::SECONDARY);
    contentArea.setOutlineThickness(LineStyles::BOX_LINE_THICKNESS);

    renderer.draw(contentArea);
}

void MainWindow::drawActiveApp(App *activeApp) {
    // Draw the app into that view
    activeApp->draw();
}