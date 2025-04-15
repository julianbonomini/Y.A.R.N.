#include "weather_app.hpp"

#include "openweather.hpp"
#include "../../ui/themes/theme_manager.hpp"
#include "../../ui/utils/ui_helpers.hpp"


WeatherApp::WeatherApp(const std::string &appName, sf::RenderTarget &renderer, const sf::Font &font, StateMachine &stateMachine, WeatherState &weatherState)
    : AppWithConfig(appName, renderer, font, stateMachine), weatherState(weatherState) {
    initConfigFromDisk();
}


void WeatherApp::handleEvent(const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.scancode == sf::Keyboard::Scan::R) {
        Logger::info("MANUAL_REFRESH");
        nlohmann::json weatherData = OpenWeather::getWeather();
        weatherState.updateFromJson(weatherData);
        Logger::done_separator();
    }

    if (settingsOpen) {
        handleSettingsInputs(keyPressed);
        if (keyPressed.scancode == sf::Keyboard::Scan::Enter) {
            // saveAndClose();
        }
    }
}

void WeatherApp::handleHelp() {
    if (helpOpen) {
        sf::FloatRect helpBox = drawModalRectangle("HELP");

        sf::Text windSpeedLabel(font, "<R> ");
        windSpeedLabel.setCharacterSize(FontSizes::LABEL);
        windSpeedLabel.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
        windSpeedLabel.setPosition(UIHelpers::snapToGrid({ helpBox.position.x + Layout::PADDING, helpBox.position.y + 40.f }));
        renderer.draw(windSpeedLabel);
        sf::Text windSpeedValue(font, "Manually update data");
        windSpeedValue.setCharacterSize(FontSizes::VALUE);
        windSpeedValue.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
        windSpeedValue.setPosition(UIHelpers::snapToGrid({ helpBox.position.x + helpBox.size.x / 3.0f + Layout::PADDING, helpBox.position.y + 40.f }));
        renderer.draw(windSpeedValue);
    }
}

void WeatherApp::handleSettings() {
    sf::FloatRect settingsBoxGlobalBounds = drawSettings();
    drawAppConfigOptions(settingsBoxGlobalBounds);
}

void WeatherApp::draw() {

    drawOverview();
    drawExtraData();
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

    sf::Text title(font, "TEMPERATURE_AMSTERDAM");
    title.setCharacterSize(FontSizes::TITLE);
    title.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    title.setPosition({box.position.x + Layout::PADDING, box.position.y + Layout::PADDING});
    renderer.draw(title);

    sf::Text temperature(font, weatherState.getWeatherData().temperature);
    temperature.setCharacterSize(FontSizes::HUGE_TEXT);
    temperature.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    sf::FloatRect temperatureBounds = temperature.getLocalBounds();
    temperature.setOrigin({ temperatureBounds.position.x + temperatureBounds.size.x / 2.0f, temperatureBounds.position.y + temperatureBounds.size.y / 2.0f });
    temperature.setPosition({ box.position.x + box.size.x / 2.0f, box.position.y + box.size.y / 2.0f - temperatureBounds.size.y });
    renderer.draw(temperature);

    sf::Text feelsLike(font, weatherState.getWeatherData().feels_like);
    feelsLike.setCharacterSize(FontSizes::BIG_TEXT);
    feelsLike.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    sf::FloatRect feelsLikeBounds = feelsLike.getLocalBounds();
    feelsLike.setOrigin({ feelsLikeBounds.position.x + feelsLikeBounds.size.x / 2.0f, feelsLikeBounds.position.y + feelsLikeBounds.size.y / 2.0f });
    feelsLike.setPosition({ box.position.x + box.size.x / 2.0f, box.position.y + box.size.y / 2.0f + temperatureBounds.size.y });
    renderer.draw(feelsLike);

    // sf::Text sunny(font, weatherState.getWeatherData().weather_title);
    // sunny.setCharacterSize(FontSizes::HUGE_TEXT);
    // sunny.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    // sf::FloatRect sunnyBounds = sunny.getLocalBounds();
    // sunny.setOrigin({ sunnyBounds.position.x + sunnyBounds.size.x / 2.0f, sunnyBounds.position.y + sunnyBounds.size.y / 2.0f });
    // sunny.setPosition({ box.position.x + box.size.x / 2.0f, box.position.y + box.size.y - Layout::PADDING - sunnyBounds.size.y });
    // renderer.draw(sunny);

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

    sf::Text text(font, weatherState.getWeatherData().last_weather_update);
    text.setCharacterSize(FontSizes::BIG_TEXT);
    text.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({ textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f });
    text.setPosition(UIHelpers::snapToGrid({ box.position.x + box.size.x / 2.0f, box.position.y + box.size.y / 2.0f }));
    renderer.draw(text);
}

void WeatherApp::drawExtraData() {
    // TODO: draw rain, pressure, etc...
    sf::FloatRect box = getGridBox(0, 2, 2, 2);
    sf::RectangleShape rect({box.size.x, box.size.y});
    rect.setPosition({box.position.x, box.position.y});
    rect.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    rect.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    rect.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(rect);

    sf::Text title(font, "DETAILS");
    title.setCharacterSize(FontSizes::TITLE);
    title.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    title.setPosition({box.position.x + Layout::PADDING, box.position.y + Layout::PADDING});
    renderer.draw(title);

    sf::Text windSpeedLabel(font, "WIND_SPD: ");
    windSpeedLabel.setCharacterSize(FontSizes::LABEL);
    windSpeedLabel.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    windSpeedLabel.setPosition(UIHelpers::snapToGrid({ box.position.x + Layout::PADDING, box.position.y + 40.f }));
    renderer.draw(windSpeedLabel);
    sf::Text windSpeedValue(font, weatherState.getWeatherData().wind_speed_meters_per_second);
    windSpeedValue.setCharacterSize(FontSizes::VALUE);
    windSpeedValue.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    windSpeedValue.setPosition(UIHelpers::snapToGrid({ box.position.x + box.size.x / 2.0f + Layout::PADDING, box.position.y + 40.f }));
    renderer.draw(windSpeedValue);

    sf::Text windGustLabel(font, "WIND_GST: ");
    windGustLabel.setCharacterSize(FontSizes::LABEL);
    windGustLabel.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    windGustLabel.setPosition(UIHelpers::snapToGrid({ box.position.x + Layout::PADDING, windSpeedLabel.getGlobalBounds().position.y + windSpeedLabel.getGlobalBounds().size.y + Layout::PADDING }));
    renderer.draw(windGustLabel);
    sf::Text windGustValue(font, weatherState.getWeatherData().wind_gust_meters_per_second);
    windGustValue.setCharacterSize(FontSizes::VALUE);
    windGustValue.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    windGustValue.setPosition(UIHelpers::snapToGrid({ box.position.x + box.size.x / 2.0f + Layout::PADDING, windSpeedLabel.getGlobalBounds().position.y + windSpeedLabel.getGlobalBounds().size.y + Layout::PADDING }));
    renderer.draw(windGustValue);

    sf::Text cloudsLabel(font, "CLDS: ");
    cloudsLabel.setCharacterSize(FontSizes::LABEL);
    cloudsLabel.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    cloudsLabel.setPosition(UIHelpers::snapToGrid({ box.position.x + Layout::PADDING, windGustLabel.getGlobalBounds().position.y + windGustLabel.getGlobalBounds().size.y + Layout::PADDING }));
    renderer.draw(cloudsLabel);
    sf::Text cloudsValue(font, weatherState.getWeatherData().cloudiness_pct);
    cloudsValue.setCharacterSize(FontSizes::VALUE);
    cloudsValue.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    cloudsValue.setPosition(UIHelpers::snapToGrid({ box.position.x + box.size.x / 2.0f + Layout::PADDING, windGustLabel.getGlobalBounds().position.y + windGustLabel.getGlobalBounds().size.y + Layout::PADDING }));
    renderer.draw(cloudsValue);

    sf::Text rainLabel(font, "RAIN: ");
    rainLabel.setCharacterSize(FontSizes::LABEL);
    rainLabel.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    rainLabel.setPosition(UIHelpers::snapToGrid({ box.position.x + Layout::PADDING, cloudsLabel.getGlobalBounds().position.y + cloudsLabel.getGlobalBounds().size.y + Layout::PADDING }));
    renderer.draw(rainLabel);
    sf::Text rainValue(font, weatherState.getWeatherData().rain_1h_mm);
    rainValue.setCharacterSize(FontSizes::VALUE);
    rainValue.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    rainValue.setPosition(UIHelpers::snapToGrid({ box.position.x + box.size.x / 2.0f + Layout::PADDING, cloudsLabel.getGlobalBounds().position.y + cloudsLabel.getGlobalBounds().size.y + Layout::PADDING }));
    renderer.draw(rainValue);

    sf::Text visibilityLabel(font, "VSBLY: ");
    visibilityLabel.setCharacterSize(FontSizes::LABEL);
    visibilityLabel.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    visibilityLabel.setPosition(UIHelpers::snapToGrid({ box.position.x + Layout::PADDING, rainLabel.getGlobalBounds().position.y + rainLabel.getGlobalBounds().size.y + Layout::PADDING }));
    renderer.draw(visibilityLabel);
    sf::Text visibilityValue(font, weatherState.getWeatherData().visibility_meters);
    visibilityValue.setCharacterSize(FontSizes::VALUE);
    visibilityValue.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    visibilityValue.setPosition(UIHelpers::snapToGrid({ box.position.x + box.size.x / 2.0f + Layout::PADDING, rainLabel.getGlobalBounds().position.y + rainLabel.getGlobalBounds().size.y + Layout::PADDING }));
    renderer.draw(visibilityValue);

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

    std::string desc = wrapText(weatherState.getWeatherData().weather_description, box.size.x - Layout::PADDING * 2 ,FontSizes::BIG_TEXT);
    sf::Text text(font, desc);
    text.setCharacterSize(FontSizes::BIG_TEXT);
    text.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({ textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f });
    text.setPosition(UIHelpers::snapToGrid({ box.position.x + box.size.x / 2.0f, box.position.y + box.size.y / 2.0f }));
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
