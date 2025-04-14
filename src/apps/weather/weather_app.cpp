#include "weather_app.hpp"

#include "../../ui/themes/theme_manager.hpp"
#include "../../ui/utils/ui_helpers.hpp"


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

    sf::Text title(font, "CONDITIONS_OUTSIDE");
    title.setCharacterSize(FontSizes::TITLE);
    title.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    title.setPosition({box.position.x + Layout::PADDING, box.position.y + Layout::PADDING});
    renderer.draw(title);

    sf::Text temperature(font, "24 C");
    temperature.setCharacterSize(FontSizes::HUGE_TEXT);
    temperature.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    sf::FloatRect temperatureBounds = temperature.getLocalBounds();
    temperature.setOrigin({ temperatureBounds.position.x + temperatureBounds.size.x / 2.0f, temperatureBounds.position.y + temperatureBounds.size.y / 2.0f });
    temperature.setPosition({ box.position.x + box.size.x / 2.0f, box.position.y + box.size.y / 2.0f - temperatureBounds.size.y });
    renderer.draw(temperature);

    sf::Text feelsLike(font, "FEELS_LIKE: 22 C");
    feelsLike.setCharacterSize(FontSizes::BIG_TEXT);
    feelsLike.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    sf::FloatRect feelsLikeBounds = feelsLike.getLocalBounds();
    feelsLike.setOrigin({ feelsLikeBounds.position.x + feelsLikeBounds.size.x / 2.0f, feelsLikeBounds.position.y + feelsLikeBounds.size.y / 2.0f });
    feelsLike.setPosition({ box.position.x + box.size.x / 2.0f, box.position.y + box.size.y / 2.0f + temperatureBounds.size.y });
    renderer.draw(feelsLike);

    sf::Text sunny(font, "SUNNY");
    sunny.setCharacterSize(FontSizes::HUGE_TEXT);
    sunny.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    sf::FloatRect sunnyBounds = sunny.getLocalBounds();
    sunny.setOrigin({ sunnyBounds.position.x + sunnyBounds.size.x / 2.0f, sunnyBounds.position.y + sunnyBounds.size.y / 2.0f });
    sunny.setPosition({ box.position.x + box.size.x / 2.0f, box.position.y + box.size.y - Layout::PADDING - sunnyBounds.size.y });
    renderer.draw(sunny);

}

void WeatherApp::drawLastUpdated() {
    sf::FloatRect box = getGridBox(4, 3, 1, 1);
    sf::RectangleShape rect({box.size.x, box.size.y});
    rect.setPosition({box.position.x, box.position.y});
    rect.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    rect.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);

    sf::Text title(font, "LAST_UPDATED");
    title.setCharacterSize(FontSizes::TITLE);
    title.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    title.setPosition({box.position.x + Layout::PADDING, box.position.y + Layout::PADDING});
    renderer.draw(title);

    sf::Text text(font, "14:02");
    text.setCharacterSize(FontSizes::BIG_TEXT);
    text.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({ textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f });
    text.setPosition({ box.position.x + box.size.x / 2.0f, box.position.y + box.size.y / 2.0f });
    renderer.draw(text);
}

void WeatherApp::drawHighLows() {
    sf::FloatRect box = getGridBox(0, 2, 2, 2);
    sf::RectangleShape rect({box.size.x, box.size.y});
    rect.setPosition({box.position.x, box.position.y});
    rect.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    rect.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);

    sf::Text title(font, "HIGH/LOW");
    title.setCharacterSize(FontSizes::TITLE);
    title.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    title.setPosition({box.position.x + Layout::PADDING, box.position.y + Layout::PADDING});
    renderer.draw(title);

    sf::Text high(font, "24 C");
    high.setCharacterSize(FontSizes::HUGE_TEXT);
    high.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    sf::FloatRect highTextBounds = high.getLocalBounds();
    high.setOrigin({ highTextBounds.position.x + highTextBounds.size.x / 2.0f, highTextBounds.position.y + highTextBounds.size.y / 2.0f });
    high.setPosition(UIHelpers::snapToGrid({ box.position.x + box.size.x / 2.0f, box.position.y + box.size.y / 2.0f + highTextBounds.size.y + Layout::PADDING }));
    renderer.draw(high);

    sf::Text low(font, "18 C");
    low.setCharacterSize(FontSizes::HUGE_TEXT);
    low.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    sf::FloatRect lowTextBounds = low.getLocalBounds();
    low.setOrigin({ lowTextBounds.position.x + lowTextBounds.size.x / 2.0f, lowTextBounds.position.y + lowTextBounds.size.y / 2.0f });
    low.setPosition(UIHelpers::snapToGrid({ box.position.x + box.size.x / 2.0f, box.position.y + box.size.y / 2.0f - lowTextBounds.size.y - Layout::PADDING }));
    renderer.draw(low);
}

void WeatherApp::drawSensorData() {
    sf::FloatRect box = getGridBox(0, 0, 2, 2);
    sf::RectangleShape rect({box.size.x, box.size.y});
    rect.setPosition({box.position.x, box.position.y});
    rect.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    rect.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);

    sf::Text title(font, "SENSOR_DATA");
    title.setCharacterSize(FontSizes::TITLE);
    title.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    title.setPosition({box.position.x + Layout::PADDING, box.position.y + Layout::PADDING});
    renderer.draw(title);

    sf::Text temp(font, "24 C");
    temp.setCharacterSize(FontSizes::BIG_TEXT);
    temp.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    sf::FloatRect tempBounds = temp.getLocalBounds();
    temp.setOrigin({ tempBounds.position.x + tempBounds.size.x / 2.0f, tempBounds.position.y + tempBounds.size.y / 2.0f });
    temp.setPosition(UIHelpers::snapToGrid({ box.position.x + box.size.x / 2.0f, box.position.y + box.size.y / 2.0f - tempBounds.size.y }));
    renderer.draw(temp);

    sf::Text humidity(font, "75% humidity");
    humidity.setCharacterSize(FontSizes::BIG_TEXT);
    humidity.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    sf::FloatRect humidityBounds = humidity.getLocalBounds();
    humidity.setOrigin({ humidityBounds.position.x + humidityBounds.size.x / 2.0f, humidityBounds.position.y + humidityBounds.size.y / 2.0f });
    humidity.setPosition(UIHelpers::snapToGrid({ box.position.x + box.size.x / 2.0f, box.position.y + box.size.y / 2.0f + humidityBounds.size.y * 2 }));
    renderer.draw(humidity);

    sf::Text updated(font, "LIVE_DATA");
    updated.setCharacterSize(FontSizes::LABEL);
    updated.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    sf::FloatRect updatedBounds = updated.getLocalBounds();
    // updated.setOrigin({ lowTextBounds.position.x + lowTextBounds.size.x / 2.0f, lowTextBounds.position.y + lowTextBounds.size.y / 2.0f });
    updated.setPosition(UIHelpers::snapToGrid({ box.position.x + box.size.x - updatedBounds.size.x - Layout::PADDING, box.position.y + box.size.y - updatedBounds.size.y - Layout::PADDING }));
    renderer.draw(updated);
}

void WeatherApp::drawDetails() {
    sf::FloatRect box = getGridBox(2, 3, 2, 1);
    sf::RectangleShape rect({box.size.x, box.size.y});
    rect.setPosition({box.position.x, box.position.y});
    rect.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    rect.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);

    sf::Text title(font, "CONDITIONS");
    title.setCharacterSize(FontSizes::TITLE);
    title.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    title.setPosition({box.position.x + Layout::PADDING, box.position.y + Layout::PADDING});
    renderer.draw(title);

    sf::Text text(font, "RAIN: 18%, WIND: 12KM/H");
    text.setCharacterSize(FontSizes::BIG_TEXT);
    text.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({ textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f });
    text.setPosition({ box.position.x + box.size.x / 2.0f, box.position.y + box.size.y / 2.0f });
    renderer.draw(text);
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
