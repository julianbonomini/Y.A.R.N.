#include "market_app.hpp"
#include<iostream>
#include <sstream>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include <string>


MarketApp::MarketApp(const std::string &appName, sf::RenderTarget &renderer, const sf::Font &font, StateMachine &stateMachine)
    : AppWithConfig(appName, renderer, font, stateMachine) {
    loadMockData();
    initConfigFromDisk();
}

void MarketApp::handleEvent(const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.scancode == sf::Keyboard::Scan::T) {
        marketOpen = !marketOpen;
    }
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
    std::cout << "Update" << std::endl;
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

    const float labelX = startX + Layout::PADDING;
    const float priceX = labelX + 120.f;
    const float changeX = priceX + 100.f;

    // Draw background box
    sf::RectangleShape backgroundBox({backgroundBoxCoordinates.size.x, backgroundBoxCoordinates.size.y});
    backgroundBox.setPosition({backgroundBoxCoordinates.position.x, backgroundBoxCoordinates.position.y});
    backgroundBox.setFillColor(Colors::BACKGROUND);
    backgroundBox.setOutlineColor(Colors::SECONDARY);
    backgroundBox.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(backgroundBox);

    // Header row
    drawSymbolsHeaderRow(startY, labelX, priceX, changeX);

    // Stock rows
    float currentY = startY + Layout::PADDING + rowHeight;

    drawLabelsAndValues(stocks, rowHeight, labelX, priceX, changeX, currentY);
}

void MarketApp::drawMarketTrackers() {
    const float rowHeight = 30.f;
    auto backgroundBoxCoordinates = getGridBox(2, 0, 2, 2);

    const float startX = backgroundBoxCoordinates.position.x;
    const float startY = backgroundBoxCoordinates.position.y;

    const float labelX = startX + Layout::PADDING;
    const float priceX = labelX + 120.f;
    const float changeX = priceX + 100.f;

    // Draw background box
    sf::RectangleShape backgroundBox({backgroundBoxCoordinates.size.x, backgroundBoxCoordinates.size.y});
    backgroundBox.setPosition({backgroundBoxCoordinates.position.x, backgroundBoxCoordinates.position.y});
    backgroundBox.setFillColor(Colors::BACKGROUND);
    backgroundBox.setOutlineColor(Colors::SECONDARY);
    backgroundBox.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(backgroundBox);

    drawSymbolsHeaderRow(startY, labelX, priceX, changeX);

    // Stock rows
    float currentY = startY + Layout::PADDING + rowHeight;

    drawLabelsAndValues(marketTrackers, rowHeight, labelX, priceX, changeX, currentY);
}

void MarketApp::drawMarketStatus() {
    // Determine background color and text color based on marketOpen
    sf::Color backgroundColor = marketOpen ? Colors::BACKGROUND : Colors::SECONDARY;
    sf::Color textColor = marketOpen ? Colors::PRIMARY : Colors::BACKGROUND;

    // Draw background box
    auto backgroundBoxCoordinates = getGridBox(4, 0, 1, 1);
    sf::RectangleShape backgroundBox({backgroundBoxCoordinates.size.x, backgroundBoxCoordinates.size.y});
    backgroundBox.setPosition({backgroundBoxCoordinates.position.x, backgroundBoxCoordinates.position.y});
    backgroundBox.setFillColor(backgroundColor);
    backgroundBox.setOutlineColor(Colors::SECONDARY);
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
    sf::Color backgroundColor = marketOpen ? Colors::BACKGROUND : Colors::SECONDARY;
    sf::Color textColor = marketOpen ? Colors::PRIMARY : Colors::BACKGROUND;

    // Draw background box
    auto backgroundBoxCoordinates = getGridBox(4, 1, 1, 1);
    sf::RectangleShape backgroundBox({backgroundBoxCoordinates.size.x, backgroundBoxCoordinates.size.y});
    backgroundBox.setPosition({backgroundBoxCoordinates.position.x, backgroundBoxCoordinates.position.y});
    backgroundBox.setFillColor(backgroundColor);
    backgroundBox.setOutlineColor(Colors::SECONDARY);
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
    backgroundBox.setFillColor(Colors::BACKGROUND);
    backgroundBox.setOutlineColor(Colors::SECONDARY);
    backgroundBox.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(backgroundBox);

    sf::Text text(font, "BEAR");
    text.setCharacterSize(20);
    text.setFillColor(Colors::PRIMARY);


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
    backgroundBox.setFillColor(Colors::BACKGROUND);
    backgroundBox.setOutlineColor(Colors::SECONDARY);
    backgroundBox.setOutlineThickness(LineStyles::LINE_THICKNESS);
    renderer.draw(backgroundBox);

    // Load image
    sf::Texture memeTexture;
    if (memeTexture.loadFromFile("assets/images/stonks.png")) {
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
    headerSymbol.setFillColor(Colors::PRIMARY);
    headerSymbol.setCharacterSize(FontSizes::LABEL);
    renderer.draw(headerSymbol);

    sf::Text headerPrice(font, "PRICE");
    headerPrice.setPosition({priceX, startY + Layout::PADDING});
    headerPrice.setFillColor(Colors::PRIMARY);
    headerPrice.setCharacterSize(FontSizes::LABEL);
    renderer.draw(headerPrice);

    sf::Text headerChange(font, "CHANGE");
    headerChange.setPosition({changeX, startY + Layout::PADDING});
    headerChange.setFillColor(Colors::PRIMARY);
    headerChange.setCharacterSize(FontSizes::LABEL);
    renderer.draw(headerChange);
}

void MarketApp::drawLabelsAndValues(const std::vector<StockData> &symbols, const float rowHeight, const float labelX, const float priceX, const float changeX, float currentY) {
    for (const auto &symbol: symbols) {
        // Symbol
        sf::Text labelText(font, symbol.ticker);
        labelText.setPosition({labelX, currentY});
        labelText.setFillColor(Colors::PRIMARY);
        labelText.setCharacterSize(FontSizes::LABEL);
        renderer.draw(labelText);

        // Price
        std::ostringstream priceStream;
        priceStream << "$" << std::fixed << std::setprecision(2) << symbol.price;
        sf::Text priceText(font, priceStream.str());
        priceText.setPosition({priceX, currentY});
        priceText.setFillColor(Colors::PRIMARY);
        priceText.setCharacterSize(FontSizes::LABEL);
        renderer.draw(priceText);

        // Change %
        std::ostringstream changeStream;
        if (symbol.changeFromOpen > 0) {
            changeStream << "+";
        }
        changeStream << std::fixed << std::setprecision(2) << symbol.changeFromOpen << "%";
        sf::Text changeText(font, changeStream.str());
        changeText.setPosition({changeX, currentY});
        changeText.setFillColor(Colors::PRIMARY);
        changeText.setCharacterSize(FontSizes::LABEL);
        renderer.draw(changeText);

        currentY += rowHeight;
    }
}

void MarketApp::loadMockData() {
    stocks = {
        {"AAPL", 172.35f, +1.23f},
        {"MSFT", 248.59f, -3.42f},
        {"AMZN", 135.72f, +0.75f},
        {"NVDA", 317.25f, -0.67f},
        {"GOOG", 849.45f, +5.38f},
        {"TSM", 849.45f, +5.38f},
        {"QCOM", 849.45f, +5.38f},
        {"AMD", 849.45f, +5.38f},
        {"MU", 849.45f, +5.38f}
    };

    marketTrackers = {
        {"SP500", 172.35f, +1.23f},
        {"NASDAQ100", 248.59f, -3.42f},
        {"ALLEUROPE", 248.59f, -3.42f},
        {"TECH100", 248.59f, -3.42f},
    };
}

void MarketApp::initConfigFromDisk() {
    configOptions = std::vector<BaseConfigOptions>();

    BaseConfigOptions refreshIntervalInMinutes;
    refreshIntervalInMinutes.label = "refresh_interval";
    refreshIntervalInMinutes.type = BaseConfigOptionType::FREE_NUMBER;;
    refreshIntervalInMinutes.options = {};
    int intervalInMinutes = stateMachine.getMarketConfig().defaultRefreshIntervalInMinutes;
    refreshIntervalInMinutes.currentValue = std::to_string(intervalInMinutes);
    refreshIntervalInMinutes.selected = false;
    refreshIntervalInMinutes.changed = false;
    refreshIntervalInMinutes.description = "How often all the symbols will be refreshed. This also affects any other type of information on this app, like market status. In seconds";
    configOptions.push_back(refreshIntervalInMinutes);

    BaseConfigOptions asd;
    asd.label = "TEST";
    asd.type = BaseConfigOptionType::TOGGLE;;
    asd.options = {};
    asd.currentValue = "1";
    asd.selected = false;
    asd.changed = false;
    asd.description = "Checkbox test";
    configOptions.push_back(asd);
}
