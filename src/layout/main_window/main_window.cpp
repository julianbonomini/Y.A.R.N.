#include "main_window.hpp"
#include <SFML/Graphics.hpp>
#include "../../globals/theme.hpp"


MainWindow::MainWindow(sf::RenderWindow &window, const sf::Font &font)
    : window(window), font(font) {
    // Constructor logic can go here (optional)
}

void MainWindow::draw(const std::vector<std::unique_ptr<App> > &apps, int activeTab) {
    // --- Draw App Area (Constrain drawing to app area) ---
    drawAppArea();

    // --- Tabs ---
    // Needs to be after App Area beacuse it has to cover it
    drawTabs(apps, activeTab);

    drawActiveApp(apps[activeTab].get());

    drawFooter();

    // --- Reset the view to default for the rest of the UI ---
    window.setView(window.getDefaultView()); // Reset to default view after app content
}

void MainWindow::drawTabs(const std::vector<std::unique_ptr<App> > &apps, int activeTab) {
    float textOffsetY = 6.f;
    for (size_t i = 0; i < apps.size(); ++i) {
        float startingXPosition = Areas::PADDING + i * (Areas::TAB_WIDTH + Areas::PADDING);

        sf::ConvexShape tabOutline;
        tabOutline.setPointCount(6);

        float curve = 8.f;

        tabOutline.setPoint(0, {startingXPosition, Areas::TOOLBAR_OFFSET + Areas::TAB_HEIGHT + textOffsetY}); // bottom left
        tabOutline.setPoint(1, {startingXPosition, Areas::TOOLBAR_OFFSET + curve}); // left curve start
        tabOutline.setPoint(2, {startingXPosition + curve, Areas::TOOLBAR_OFFSET}); // top left curve
        tabOutline.setPoint(3, {startingXPosition + Areas::TAB_WIDTH - curve, Areas::TOOLBAR_OFFSET}); // top right curve
        tabOutline.setPoint(4, {startingXPosition + Areas::TAB_WIDTH, Areas::TOOLBAR_OFFSET + curve}); // right curve end
        tabOutline.setPoint(5, {startingXPosition + Areas::TAB_WIDTH, Areas::TOOLBAR_OFFSET + Areas::TAB_HEIGHT + textOffsetY}); // bottom right
        tabOutline.setOutlineThickness(Lines::LINE_THICKNESS);
        tabOutline.setOutlineColor(Colors::GRAY);


        if (i == activeTab) {
            window.draw(tabOutline);
            sf::RectangleShape coverAreaLine(sf::Vector2f(Areas::TAB_WIDTH, Lines::LINE_THICKNESS));
            coverAreaLine.setPosition({startingXPosition, Areas::TOOLBAR_OFFSET + Areas::TAB_HEIGHT + Areas::PADDING - 2.f});
            coverAreaLine.setFillColor(Colors::BACKGROUND);
            window.draw(coverAreaLine);
        }

        sf::Text label(font, apps[i].get()->appName, TextSizes::TAB);
        label.setFillColor(Colors::BLACK);
        label.setPosition({startingXPosition + Areas::PADDING, Areas::TOOLBAR_OFFSET + textOffsetY});
        window.draw(label);
    }
}

void MainWindow::drawAppArea() {
    // --- Content area ---
    float contentX = Areas::PADDING;
    float contentY = Areas::TOOLBAR_OFFSET + Areas::TAB_HEIGHT + Areas::PADDING;

    sf::RectangleShape contentArea(sf::Vector2f(Areas::MAIN_APP_WIDTH, Areas::MAIN_APP_HEIGHT));
    contentArea.setPosition({contentX, contentY});
    contentArea.setFillColor(Colors::BACKGROUND);
    contentArea.setOutlineColor(Colors::GRAY); // light gray
    contentArea.setOutlineThickness(Lines::BOX_LINE_THICKNESS);

    window.draw(contentArea);
}

void MainWindow::drawActiveApp(App *activeApp) {
    // Draw the app into that view
    activeApp->draw();
}

void MainWindow::drawFooter() {
    float textX = Areas::PADDING * 2;
    float textY = Areas::WINDOW_HEIGHT - Areas::FOOTER_HEIGHT;

    sf::Text left(font, "<A> <- Tab", TextSizes::DESCRITPION);
    left.setFillColor(Colors::BLACK); // or maybe sf::Color::Green for terminal vibes
    left.setPosition({textX, textY});
    window.draw(left);
    sf::Text right(font, "<S> Tab ->", TextSizes::DESCRITPION);
    right.setFillColor(Colors::BLACK); // or maybe sf::Color::Green for terminal vibes
    right.setPosition({textX, textY + left.getGlobalBounds().size.y + Areas::PADDING});
    window.draw(right);
}
