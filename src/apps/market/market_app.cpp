#include "market_app.hpp"
#include<iostream>
#include <sstream>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include <string>

#include "../../core/execute/execute_utils.hpp"
#include "../../ui/themes/theme_manager.hpp"


MarketApp::MarketApp(const std::string &appName, sf::RenderTarget &renderer, const sf::Font &font, StateMachine &stateMachine)
    : AppWithConfig(appName, renderer, font, stateMachine) {
    initConfigFromDisk();
}

void MarketApp::handleEvent(const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.scancode == sf::Keyboard::Scan::T) {
        marketOpen = !marketOpen;
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

void MarketApp::update(float /*deltaTime*/) {
    Logger::info("Update");
}

void MarketApp::draw() {
    drawStandaloneSymbols();
    drawMarketTrackers();
    drawMarketStatus();
    drawMarketSession();

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

    drawLabelsAndValues(stocksQuotes, rowHeight, labelX, priceX, changeX, currentY);
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

    drawLabelsAndValues(trackersQuotes, rowHeight, labelX, priceX, changeX, currentY);
}

void MarketApp::drawMarketStatus() {
    // Determine background color and text color based on marketOpen
    sf::Color backgroundColor = !marketOpen ? ThemeManager::instance().getCurrentTheme().background() : ThemeManager::instance().getCurrentTheme().secondary();
    sf::Color textColor = !marketOpen ? ThemeManager::instance().getCurrentTheme().primary() : ThemeManager::instance().getCurrentTheme().background();

    // Draw background box
    auto backgroundBoxCoordinates = getGridBox(4, 0, 1, 1);
    sf::RectangleShape backgroundBox({backgroundBoxCoordinates.size.x, backgroundBoxCoordinates.size.y});
    backgroundBox.setPosition({backgroundBoxCoordinates.position.x, backgroundBoxCoordinates.position.y});
    backgroundBox.setFillColor(backgroundColor);
    backgroundBox.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    backgroundBox.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(backgroundBox);

    // Draw market status text (OPEN or CLOSED)
    sf::Text marketStatusText(font);
    marketStatusText.setCharacterSize(20); // Adjust size as needed
    marketStatusText.setFillColor(textColor);

    if (marketOpen) {
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

void MarketApp::drawMarketSession() {
    // Determine background color and text color based on marketOpen
    sf::Color backgroundColor = marketOpen ? ThemeManager::instance().getCurrentTheme().background() : ThemeManager::instance().getCurrentTheme().secondary();
    sf::Color textColor = marketOpen ? ThemeManager::instance().getCurrentTheme().primary() : ThemeManager::instance().getCurrentTheme().background();

    // Draw background box
    auto backgroundBoxCoordinates = getGridBox(4, 1, 1, 1);
    sf::RectangleShape backgroundBox({backgroundBoxCoordinates.size.x, backgroundBoxCoordinates.size.y});
    backgroundBox.setPosition({backgroundBoxCoordinates.position.x, backgroundBoxCoordinates.position.y});
    backgroundBox.setFillColor(backgroundColor);
    backgroundBox.setOutlineColor(ThemeManager::instance().getCurrentTheme().secondary());
    backgroundBox.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(backgroundBox);

    // Draw market status text (OPEN or CLOSED)
    sf::Text marketStatusText(font, "14:23 left");
    marketStatusText.setCharacterSize(20); // Adjust size as needed
    marketStatusText.setFillColor(textColor);

    // Center the text within the background box
    sf::FloatRect textBounds = marketStatusText.getLocalBounds();
    marketStatusText.setOrigin({textBounds.getCenter().x, textBounds.getCenter().y});
    marketStatusText.setPosition({backgroundBoxCoordinates.position.x + backgroundBoxCoordinates.size.x / 2, backgroundBoxCoordinates.position.y + backgroundBoxCoordinates.size.y / 2});
    renderer.draw(marketStatusText);
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

    sf::Text text(font, "BEAR");
    text.setCharacterSize(20);
    text.setFillColor(ThemeManager::instance().getCurrentTheme().primary());


    // Center the text within the background box
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({textBounds.getCenter().x, textBounds.getCenter().y});
    text.setPosition({backgroundBoxCoordinates.position.x + backgroundBoxCoordinates.size.x / 2, backgroundBoxCoordinates.position.y + backgroundBoxCoordinates.size.y / 2});
    renderer.draw(text);
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
        sf::Text priceText(font, priceStream.str());
        priceText.setPosition({priceX, currentY});
        priceText.setFillColor(ThemeManager::instance().getCurrentTheme().primary());
        priceText.setCharacterSize(FontSizes::LABEL);
        renderer.draw(priceText);

        // Change %
        std::ostringstream changeStream;
        if (market_quote.second.changeFromOpen > 0) {
            changeStream << "+";
        }
        changeStream << std::fixed << std::setprecision(2) << "%";
        sf::Text changeText(font, changeStream.str());
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

    symbols = stateMachine.getMarketConfig().symbols;
    for (const auto &symbol : symbols) {
        MarketQuote quote;
        quote.symbol = symbol;
        quote.price = 1;
        quote.changeFromOpen = 1;
        quote.changeFromPreviousClose = 1;
        stocksQuotes[symbol] = quote;
    }

    // TODO: add this con config when finish testing
    // "symbols": [
    //     "AAPL",
    //     "MSFT",
    //     "AMZN",
    //     "NVDA",
    //     "GOOG",
    //     "TSM",
    //     "QCOM",
    //     "AMD",
    //     "MU",
    //     "ASML",
    //     "SMCI",
    //     "AMAT",
    //     "AVGO",
    //     "LRCX"
    // ],

    trackers = stateMachine.getMarketConfig().trackers;
    for (const auto &tracker : trackers) {
        MarketQuote quote;
        quote.symbol = tracker;
        quote.price = 1;
        quote.changeFromOpen = 1;
        quote.changeFromPreviousClose = 1;
        trackersQuotes[tracker] = quote;
    }
}
