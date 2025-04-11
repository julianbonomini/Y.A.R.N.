#include "weather_app.hpp"

#include "../../ui/themes/theme_manager.hpp"


WeatherApp::WeatherApp(const std::string &appName, sf::RenderTarget &renderer, const sf::Font &font, StateMachine &stateMachine)
    : AppWithConfig(appName, renderer, font, stateMachine) {
    initConfigFromDisk();
}


void WeatherApp::handleEvent(const sf::Event::KeyPressed &keyPressed) {
    if (settingsOpen) {
        if (keyPressed.scancode == sf::Keyboard::Scan::Down) {
            moveDown();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Right) {
            changeOptionRight();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Left) {
            changeOptionLeft();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Up) {
            moveUp();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Escape && unsavedChangesFlag) {
            closeWithUnsavedChanges();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Escape && !unsavedChangesFlag) {
            closeWithoutChanges();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::C && !unsavedChangesFlag) {
            closeWithoutChanges();
        }
        if (keyPressed.scancode == sf::Keyboard::Scan::Enter) {
            // saveAndClose();
        }
    }
}

void WeatherApp::handleHelp() {
    if (helpOpen) {
        drawModalRectangle("HELP");
    }
}

void WeatherApp::handleSettings() {
    sf::FloatRect settingsBoxGlobalBounds = drawSettings();
    drawAppConfigOptions(settingsBoxGlobalBounds);
}

void WeatherApp::draw() {

    drawOverview();
    drawHighLows();
    drawDetails();
    drawLastUpdated();
    drawSensorData();

    if (isSettingsOpen()) {
        handleSettings();
    }
    if (isHelpOpen()) {
        handleHelp();
    }
}

void WeatherApp::drawOverview() {
    sf::FloatRect box = getGridBox(2, 0, 3, 3);
    sf::RectangleShape rect({box.size.x, box.size.y});
    rect.setPosition({box.position.x, box.position.y});
    rect.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    rect.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);
}

void WeatherApp::drawLastUpdated() {
    sf::FloatRect box = getGridBox(4, 3, 1, 1);
    sf::RectangleShape rect({box.size.x, box.size.y});
    rect.setPosition({box.position.x, box.position.y});
    rect.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    rect.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);
}

void WeatherApp::drawHighLows() {
    sf::FloatRect box = getGridBox(0, 2, 2, 2);
    sf::RectangleShape rect({box.size.x, box.size.y});
    rect.setPosition({box.position.x, box.position.y});
    rect.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    rect.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);
}

void WeatherApp::drawSensorData() {
    sf::FloatRect box = getGridBox(0, 0, 2, 2);
    sf::RectangleShape rect({box.size.x, box.size.y});
    rect.setPosition({box.position.x, box.position.y});
    rect.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    rect.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);
}

void WeatherApp::drawDetails() {
    sf::FloatRect box = getGridBox(2, 3, 2, 1);
    sf::RectangleShape rect({box.size.x, box.size.y});
    rect.setPosition({box.position.x, box.position.y});
    rect.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    rect.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);
}

void WeatherApp::initConfigFromDisk() {
    configOptions = std::vector<BaseConfigOptions>();

    BaseConfigOptions asd;
    asd.label = "default";
    asd.type = BaseConfigOptionType::FREE_NUMBER;
    asd.options = {};
    asd.currentValue = "9";
    asd.selected = false;
    asd.changed = false;
    asd.description = "This is a default test config without any effect.";
    configOptions.push_back(asd);

}
