#include "main_window.hpp"
#include <SFML/Graphics.hpp>
#include "../../globals/ui_globals.hpp"
#include "../../ui/themes/theme_manager.hpp"
#include "../../ui/utils/ui_helpers.hpp"


MainWindow::MainWindow(sf::RenderTarget &renderer, const sf::Font &font)
    : renderer(renderer), font(font) {
    // Constructor logic can go here (optional)
}

void MainWindow::draw(std::vector<std::unique_ptr<App> > &apps, StateMachine &stateMachine, bool &mouseClicked,
                      sf::Vector2f &mouseLastPosition) {
    // --- Draw App Area (Constrain drawing to app area) ---
    drawAppArea();

    // --- Tabs ---
    // Needs to be after App Area because it has to cover it
    drawTabs(apps, Tabs::tabToIndex(stateMachine.getActiveTab()), mouseClicked, mouseLastPosition, stateMachine);

    drawActiveApp(apps, stateMachine);
}

void MainWindow::drawTabs(
    const std::vector<std::unique_ptr<App> > &apps,
    int activeTab,
    bool &mouseClicked,
    sf::Vector2f &mouseLastPosition,
    StateMachine &stateMachine
) {
    float textOffsetY = 6.f;
    for (size_t i = 0; i < apps.size(); ++i) {
        float startingXPosition = Layout::PADDING + i * (Layout::TAB_WIDTH + Layout::PADDING);

        sf::ConvexShape tabOutline;
        tabOutline.setPointCount(6);

        float curve = 8.f;

        tabOutline.setPoint(0, {
                                startingXPosition, Layout::TOOLBAR_OFFSET + Layout::TAB_HEIGHT + textOffsetY
                            }); // bottom left
        tabOutline.setPoint(1, {startingXPosition, Layout::TOOLBAR_OFFSET + curve}); // left curve start
        tabOutline.setPoint(2, {startingXPosition + curve, Layout::TOOLBAR_OFFSET}); // top left curve
        tabOutline.setPoint(3, {startingXPosition + Layout::TAB_WIDTH - curve, Layout::TOOLBAR_OFFSET});
        // top right curve
        tabOutline.setPoint(4, {startingXPosition + Layout::TAB_WIDTH, Layout::TOOLBAR_OFFSET + curve});
        // right curve end
        tabOutline.setPoint(5, {
                                startingXPosition + Layout::TAB_WIDTH,
                                Layout::TOOLBAR_OFFSET + Layout::TAB_HEIGHT + textOffsetY
                            }); // bottom right
        tabOutline.setOutlineThickness(LineStyles::LINE_THICKNESS);
        tabOutline.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
        tabOutline.setFillColor(ThemeManager::instance().getCurrentTheme().background());


        if ((size_t) i == activeTab) {
            renderer.draw(tabOutline);
            sf::RectangleShape coverAreaLine(sf::Vector2f(Layout::TAB_WIDTH, LineStyles::LINE_THICKNESS));
            coverAreaLine.setPosition({
                startingXPosition, Layout::TOOLBAR_OFFSET + Layout::TAB_HEIGHT + Layout::PADDING - 2.f
            });
            coverAreaLine.setFillColor(ThemeManager::instance().getCurrentTheme().background());
            renderer.draw(coverAreaLine);
        }

        sf::Text label(font, apps[i].get()->appName, FontSizes::TAB);
        label.setFillColor(i == activeTab
                               ? ThemeManager::instance().getCurrentTheme().primary()
                               : ThemeManager::instance().getCurrentTheme().secondary());
        sf::Vector2f labelPosition = UIHelpers::snapToGrid({
            startingXPosition + Layout::PADDING, Layout::TOOLBAR_OFFSET + textOffsetY
        });
        label.setPosition(labelPosition);
        renderer.draw(label);

        if (mouseClicked) {
            // Define the bounding box of the current tab for click detection
            sf::FloatRect tabBounds(
                sf::Vector2f(startingXPosition, Layout::TOOLBAR_OFFSET), // Position
                sf::Vector2f(Layout::TAB_WIDTH, Layout::TAB_HEIGHT + textOffsetY + Layout::PADDING - 2.f) // Size
            );

            // Check if the mouse click position is within the bounds of the current tab
            if (tabBounds.contains(mouseLastPosition)) {
                Logger::debug("Tab clicked:", apps[i]->appName, "Setting to active");
                stateMachine.setActiveTab(Tabs::indexToTab(i));
                mouseClicked = false; // Reset the flag after handling the click
            }
        }
    }
}

void MainWindow::drawAppArea() {
    // --- Content area ---
    float contentX = Layout::PADDING;
    float contentY = Layout::TOOLBAR_OFFSET + Layout::TAB_HEIGHT + Layout::PADDING;

    sf::RectangleShape contentArea(sf::Vector2f(Layout::MAIN_APP_WIDTH, Layout::MAIN_APP_HEIGHT));
    contentArea.setPosition({contentX, contentY});
    contentArea.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    contentArea.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    contentArea.setOutlineThickness(LineStyles::BOX_LINE_THICKNESS);

    renderer.draw(contentArea);
}


void MainWindow::drawActiveApp(const std::vector<std::unique_ptr<App> > &apps, StateMachine &stateMachine) {
    size_t index = Tabs::tabToIndex(stateMachine.getActiveTab());

    if (index >= apps.size()) {
        Logger::error("Invalid tab index:", index, "app.size", apps.size());
        healTab(apps, stateMachine);
        return;
    }

    App *activeApp = apps[index].get();
    if (activeApp) {
        activeApp->draw();
    } else {
        Logger::error("Tried to draw a null app at index", index);
        healTab(apps, stateMachine);
    }
}

void MainWindow::healTab(const std::vector<std::unique_ptr<App> > &apps, StateMachine &stateMachine) {
    Logger::warning("Healing OS config, setting default active tab to 0");
    stateMachine.healOsConfig();
    stateMachine.setActiveTab(Tab::INF);

    Logger::info("Updating config app to display latest config...");
    for (size_t i = 0; i < apps.size(); ++i) {
        if (apps[i]->appName == "CNF") {
            apps[i]->initConfigFromDisk();
        }
    }
}
