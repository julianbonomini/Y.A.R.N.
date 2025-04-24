#include "market_app.hpp"
#include<iostream>
#include <sstream>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include <string>

#include "../../core/execute/execute_utils.hpp"
#include "../../core/state_machine/market_state.hpp"
#include "../../ui/themes/theme_manager.hpp"


MarketApp::MarketApp(const std::string &appName, sf::RenderTarget &renderer, const sf::Font &font, StateMachine &stateMachine, MarketState &marketState)
    : AppWithConfig(appName, renderer, font, stateMachine), marketState(marketState) {
    initConfigFromDisk();
    initMockedQuotes();
}

void MarketApp::handleEvent(const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.scancode == sf::Keyboard::Scan::R) {
        update();
    }
    if (settingsOpen) {
        handleSettingsInputs(keyPressed);
        if (keyPressed.scancode == sf::Keyboard::Scan::Enter) {
            saveAndClose(AppConfigTypes::MARKET, configOptions);
        }
    }
}

void MarketApp::handleHelp() {
    if (helpOpen) {
        drawModalRectangle("HELP");
    }
}

void MarketApp::handleSettings() {
    sf::FloatRect settingsBoxGlobalBounds = drawSettings();
    drawAppConfigOptions(settingsBoxGlobalBounds);
}

void MarketApp::update() {
    Logger::info("[MARKET_APP] Manual data refresh");
    std::map<std::string, MarketQuote> quotes = MarketDaemonClient::getAllQuotes();
    marketState.updateAllQuotes(quotes);
    Logger::done_separator();
}

void MarketApp::draw() {
    drawStandaloneSymbols();
    drawMarketTrackers();
    drawMarketStatus();
    drawLastUpdate();

    drawMarketSentiment();

    drawStonksMeme();

    if (isSettingsOpen()) {
        handleSettings();
    }
    if (isHelpOpen()) {
        handleHelp();
    }
}

void MarketApp::drawStandaloneSymbols() {
    auto backgroundBoxCoordinates = getGridBox(0, 0, 2, 4);
    const float rowHeight = 30.f;
    const float startX = backgroundBoxCoordinates.position.x;
    const float startY = backgroundBoxCoordinates.position.y;
    const float sizeX = backgroundBoxCoordinates.size.x;

    const float labelX = startX + Layout::PADDING;
    const float priceX = labelX + 150.f;
    const float changeX = startX + sizeX - Layout::PADDING;

    // Draw background box
    sf::RectangleShape backgroundBox({backgroundBoxCoordinates.size.x, backgroundBoxCoordinates.size.y});
    backgroundBox.setPosition({backgroundBoxCoordinates.position.x, backgroundBoxCoordinates.position.y});
    backgroundBox.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    backgroundBox.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    backgroundBox.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(backgroundBox);

    // Header row
    drawSymbolsHeaderRow(startY, labelX, priceX, changeX);

    // Stock rows
    float currentY = startY + Layout::PADDING + rowHeight;

    drawLabelsAndValues(marketState.getStockQuotes(), rowHeight, labelX, priceX, changeX, currentY);
}

void MarketApp::drawMarketTrackers() {
    const float rowHeight = 30.f;
    auto backgroundBoxCoordinates = getGridBox(2, 0, 2, 2);

    const float startX = backgroundBoxCoordinates.position.x;
    const float startY = backgroundBoxCoordinates.position.y;
    const float sizeX = backgroundBoxCoordinates.size.x;

    const float labelX = startX + Layout::PADDING;
    const float priceX = labelX + 150.f;
    const float changeX = startX + sizeX - Layout::PADDING;

    // Draw background box
    sf::RectangleShape backgroundBox({backgroundBoxCoordinates.size.x, backgroundBoxCoordinates.size.y});
    backgroundBox.setPosition({backgroundBoxCoordinates.position.x, backgroundBoxCoordinates.position.y});
    backgroundBox.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    backgroundBox.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    backgroundBox.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(backgroundBox);

    drawSymbolsHeaderRow(startY, labelX, priceX, changeX);

    // Stock rows
    float currentY = startY + Layout::PADDING + rowHeight;

    drawLabelsAndValues(marketState.getTrackerQuotes(), rowHeight, labelX, priceX, changeX, currentY);
}

void MarketApp::drawMarketStatus() {
    // Determine background color and text color based on marketOpen
    sf::Color backgroundColor = !marketState.getIsMarketOpen() ? ThemeManager::instance().getCurrentTheme().background() : ThemeManager::instance().getCurrentTheme().secondary();
    sf::Color textColor = !marketState.getIsMarketOpen() ? ThemeManager::instance().getCurrentTheme().primary() : ThemeManager::instance().getCurrentTheme().background();

    // Draw background box
    auto backgroundBoxCoordinates = getGridBox(4, 0, 1, 1);
    sf::RectangleShape backgroundBox({backgroundBoxCoordinates.size.x, backgroundBoxCoordinates.size.y});
    backgroundBox.setPosition({backgroundBoxCoordinates.position.x, backgroundBoxCoordinates.position.y});
    backgroundBox.setFillColor(backgroundColor);
    backgroundBox.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    backgroundBox.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(backgroundBox);

    sf::Text boxTitle(font, "MARKET_STATUS");
    boxTitle.setCharacterSize(FontSizes::TITLE); // Adjust size as needed
    boxTitle.setFillColor(textColor);
    boxTitle.setPosition({backgroundBoxCoordinates.position.x + Layout::PADDING, backgroundBoxCoordinates.position.y + Layout::PADDING});
    renderer.draw(boxTitle);

    // Draw market status text (OPEN or CLOSED)
    sf::Text marketStatusText(font);
    marketStatusText.setCharacterSize(20); // Adjust size as needed
    marketStatusText.setFillColor(textColor);

    if (marketState.getIsMarketOpen()) {
        marketStatusText.setString("OPEN");
    } else {
        marketStatusText.setString("CLOSED");
    }

    // Center the text within the background box
    sf::FloatRect textBounds = marketStatusText.getLocalBounds();
    marketStatusText.setOrigin({textBounds.getCenter().x, textBounds.getCenter().y});
    marketStatusText.setPosition({backgroundBoxCoordinates.position.x + backgroundBoxCoordinates.size.x / 2, backgroundBoxCoordinates.position.y + backgroundBoxCoordinates.size.y / 2});
    renderer.draw(marketStatusText);
}

void MarketApp::drawLastUpdate() {
    // Draw background box
    auto backgroundBoxCoordinates = getGridBox(4, 1, 1, 1);
    sf::RectangleShape backgroundBox({backgroundBoxCoordinates.size.x, backgroundBoxCoordinates.size.y});
    backgroundBox.setPosition({backgroundBoxCoordinates.position.x, backgroundBoxCoordinates.position.y});
    backgroundBox.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    backgroundBox.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    backgroundBox.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(backgroundBox);

    sf::Text boxTitle(font, "UPDATED_AT");
    boxTitle.setCharacterSize(FontSizes::TITLE); // Adjust size as needed
    boxTitle.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    boxTitle.setPosition({backgroundBoxCoordinates.position.x + Layout::PADDING, backgroundBoxCoordinates.position.y + Layout::PADDING});
    renderer.draw(boxTitle);

    sf::Text lastUpdateText(font, marketState.getLastUpdate());
    lastUpdateText.setCharacterSize(20); // Adjust size as needed
    lastUpdateText.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    sf::FloatRect textBounds = lastUpdateText.getLocalBounds();
    lastUpdateText.setOrigin({textBounds.getCenter().x, textBounds.getCenter().y});
    lastUpdateText.setPosition({backgroundBoxCoordinates.position.x + backgroundBoxCoordinates.size.x / 2, backgroundBoxCoordinates.position.y + backgroundBoxCoordinates.size.y / 2});
    renderer.draw(lastUpdateText);
}

void MarketApp::drawMarketSentiment() {
    // Draw background box
    auto backgroundBoxCoordinates = getGridBox(2, 2, 1.5, 2);
    sf::RectangleShape backgroundBox({backgroundBoxCoordinates.size.x, backgroundBoxCoordinates.size.y});
    backgroundBox.setPosition({backgroundBoxCoordinates.position.x, backgroundBoxCoordinates.position.y});
    backgroundBox.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    backgroundBox.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    backgroundBox.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(backgroundBox);

    // sf::Text text(font, "BEAR");
    // text.setCharacterSize(20);
    // text.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    //
    //
    // // Center the text within the background box
    // sf::FloatRect textBounds = text.getLocalBounds();
    // text.setOrigin({textBounds.getCenter().x, textBounds.getCenter().y});
    // text.setPosition({backgroundBoxCoordinates.position.x + backgroundBoxCoordinates.size.x / 2, backgroundBoxCoordinates.position.y + backgroundBoxCoordinates.size.y / 2});
    // renderer.draw(text);
}

void MarketApp::drawStonksMeme() {
    // Draw background box
    auto backgroundBoxCoordinates = getGridBox(3.5, 2, 1.5, 2);
    sf::RectangleShape backgroundBox({backgroundBoxCoordinates.size.x, backgroundBoxCoordinates.size.y});
    backgroundBox.setPosition({backgroundBoxCoordinates.position.x, backgroundBoxCoordinates.position.y});
    backgroundBox.setFillColor(ThemeManager::instance().getCurrentTheme().background());
    backgroundBox.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    backgroundBox.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(backgroundBox);

    // Load image
    sf::Texture memeTexture;
    std::string filename = ExecuteUtils::getResourcePath("assets/images/stonks.png");
    if (memeTexture.loadFromFile(filename)) {
        sf::Sprite memeSprite(memeTexture);

        // Scale image to fit inside the background box
        float scaleX = backgroundBoxCoordinates.size.x / memeTexture.getSize().x;
        float scaleY = backgroundBoxCoordinates.size.y / memeTexture.getSize().y;
        memeSprite.setScale({scaleX, scaleY});

        // Position the image
        memeSprite.setPosition({backgroundBoxCoordinates.position.x, backgroundBoxCoordinates.position.y});

        renderer.draw(memeSprite);
    }
}

void MarketApp::drawSymbolsHeaderRow(const float startY, const float labelX, const float priceX, const float changeX) {
    // Header row
    sf::Text headerSymbol(font, "SYMBOL");
    headerSymbol.setPosition({labelX, startY + Layout::PADDING});
    headerSymbol.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    headerSymbol.setCharacterSize(FontSizes::LABEL);
    renderer.draw(headerSymbol);

    sf::Text headerPrice(font, "PRICE");
    headerPrice.setPosition({priceX, startY + Layout::PADDING});
    headerPrice.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    headerPrice.setCharacterSize(FontSizes::LABEL);
    renderer.draw(headerPrice);

    sf::Text headerChange(font, "CHANGE");
    headerChange.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
    headerChange.setCharacterSize(FontSizes::LABEL);
    sf::FloatRect textBounds = headerChange.getLocalBounds();
    float alignedX = changeX - textBounds.size.x;
    alignedX -= textBounds.position.length();
    headerChange.setPosition({alignedX, startY + Layout::PADDING});
    renderer.draw(headerChange);
}

void MarketApp::drawLabelsAndValues(const std::map<std::string, MarketQuote> &quotes, const float rowHeight, const float labelX, const float priceX, const float changeX, float currentY) {
    for (const auto &market_quote: quotes) {
        // Symbol
        sf::Text labelText(font, market_quote.second.symbol);
        labelText.setPosition({labelX, currentY});
        labelText.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
        labelText.setCharacterSize(FontSizes::LABEL);
        renderer.draw(labelText);

        // Price
        std::ostringstream priceStream;
        priceStream << "$" << std::fixed << std::setprecision(2) << market_quote.second.price;
        std::string priceTextValue = market_quote.second.price == 0 ? "loading..." : priceStream.str();
        sf::Text priceText(font, priceTextValue);
        priceText.setPosition({priceX, currentY});
        priceText.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
        priceText.setCharacterSize(FontSizes::LABEL);
        renderer.draw(priceText);

        // Change %
        std::ostringstream changeStream;
        if (market_quote.second.changeFromPreviousClose.has_value()) {
            if (market_quote.second.changeFromPreviousClose > 0) {
                changeStream << "+";
            }
            changeStream << std::fixed << std::setprecision(2) << market_quote.second.changeFromPreviousClose.value() << "%";
        } else {
            changeStream << "no_data";
        }
        std::string changeTextValue = market_quote.second.changeFromPreviousClose.has_value() && market_quote.second.changeFromPreviousClose == 0 ? "loading..." : changeStream.str();
        sf::Text changeText(font, changeTextValue);
        changeText.setCharacterSize(FontSizes::LABEL);
        changeText.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
        sf::FloatRect textBounds = changeText.getLocalBounds();
        float alignedX = changeX - textBounds.size.x;
        alignedX -= textBounds.position.length();
        changeText.setPosition({alignedX, currentY});
        renderer.draw(changeText);

        currentY += rowHeight;
    }
}

void MarketApp::initMockedQuotes() {
    std::map<std::string, MarketQuote> mockQuotes = {};
    for (std::string symbol: stateMachine.getMarketConfig().symbols) {
        MarketQuote quote;
        quote.symbol = symbol;
        quote.type = "stock";
        quote.price = 0;
        quote.changeFromOpen = 0;
        quote.changeFromPreviousClose = 0;
        mockQuotes[symbol] = quote;
    }
    for (std::string tracker: stateMachine.getMarketConfig().trackers) {
        MarketQuote quote;
        quote.symbol = tracker;
        quote.type = "index";
        quote.price = 0;
        quote.changeFromOpen = 0;
        quote.changeFromPreviousClose = 0;
        mockQuotes[tracker] = quote;
    }
    marketState.updateAllQuotes(mockQuotes);
}

void MarketApp::initConfigFromDisk() {
    configOptions = std::vector<BaseConfigOptions>();

    BaseConfigOptions refreshIntervalInMinutes;
    refreshIntervalInMinutes.label = "refresh_interval";
    refreshIntervalInMinutes.type = BaseConfigOptionType::FREE_NUMBER;;
    refreshIntervalInMinutes.options = {};
    int intervalInMinutes = stateMachine.getMarketConfig().refreshIntervalInMinutes;
    refreshIntervalInMinutes.currentValue = std::to_string(intervalInMinutes);
    refreshIntervalInMinutes.selected = false;
    refreshIntervalInMinutes.changed = false;
    refreshIntervalInMinutes.description = "How often all the symbols will be refreshed. This also affects any other type of information on this app, like market status. In seconds";
    configOptions.push_back(refreshIntervalInMinutes);
}
