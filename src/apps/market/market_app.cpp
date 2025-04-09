#include "market_app.hpp"
#include<iostream>
#include <sstream>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include <string>


MarketApp::MarketApp(sf::RenderWindow &window, const sf::Font &font, const std::string &appName)
    : App(appName, window, font), font(font), window(window) {
    loadMockData();
}

void MarketApp::handleEvent(const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.scancode == sf::Keyboard::Scan::T) {
        marketOpen = !marketOpen;
    }
}

void MarketApp::handleHelp() {
    if (helpOpen) {
        drawModalRectangle(window, "HELP");
    }
}

void MarketApp::handleSettings() {
    if (settingsOpen) {
        drawModalRectangle(window, "SETTINGS");
    }
}


void MarketApp::update(float /*deltaTime*/) {
    std::cout << "Update" << std::endl;
}

void MarketApp::draw() {
    drawStandaloneSymbols();
    drawMarketTrackers();
    drawMarketStatus();
    drawMarketSession();

    handleHelp();
    handleSettings();
}

void MarketApp::drawStandaloneSymbols() {
    auto backgroundBoxCoordinates = getGridBox(0, 0, 2, 4);
    const float rowHeight = 30.f;
    const float startX = backgroundBoxCoordinates.position.x;
    const float startY = backgroundBoxCoordinates.position.y;

    const float labelX = startX + Areas::PADDING;
    const float priceX = labelX + 120.f;
    const float changeX = priceX + 100.f;

    // Draw background box
    sf::RectangleShape backgroundBox({backgroundBoxCoordinates.size.x, backgroundBoxCoordinates.size.y});
    backgroundBox.setPosition({backgroundBoxCoordinates.position.x, backgroundBoxCoordinates.position.y});
    backgroundBox.setFillColor(Colors::WHITE);
    backgroundBox.setOutlineColor(Colors::GRAY);
    backgroundBox.setOutlineThickness(Lines::LINE_THICKNESS);
    window.draw(backgroundBox);

    // Header row
    drawSymbolsHeaderRow(startY, labelX, priceX, changeX);

    // Stock rows
    float currentY = startY + Areas::PADDING + rowHeight;

    drawLabelsAndValues(stocks, rowHeight, labelX, priceX, changeX, currentY);
}

void MarketApp::drawMarketTrackers() {
    const float rowHeight = 30.f;
    auto backgroundBoxCoordinates = getGridBox(2, 0, 2, 2);

    const float startX = backgroundBoxCoordinates.position.x;
    const float startY = backgroundBoxCoordinates.position.y;

    const float labelX = startX + Areas::PADDING;
    const float priceX = labelX + 120.f;
    const float changeX = priceX + 100.f;

    // Draw background box
    sf::RectangleShape backgroundBox({backgroundBoxCoordinates.size.x, backgroundBoxCoordinates.size.y});
    backgroundBox.setPosition({backgroundBoxCoordinates.position.x, backgroundBoxCoordinates.position.y});
    backgroundBox.setFillColor(Colors::WHITE);
    backgroundBox.setOutlineColor(Colors::GRAY);
    backgroundBox.setOutlineThickness(Lines::LINE_THICKNESS);
    window.draw(backgroundBox);

    drawSymbolsHeaderRow(startY, labelX, priceX, changeX);

    // Stock rows
    float currentY = startY + Areas::PADDING + rowHeight;

    drawLabelsAndValues(marketTrackers, rowHeight, labelX, priceX, changeX, currentY);
}

void MarketApp::drawMarketStatus() {
    // Determine background color and text color based on marketOpen
    sf::Color backgroundColor = marketOpen ? Colors::WHITE : Colors::GRAY;
    sf::Color textColor = marketOpen ? Colors::BLACK : Colors::WHITE;

    // Draw background box
    auto backgroundBoxCoordinates = getGridBox(4, 0, 1, 1);
    sf::RectangleShape backgroundBox({backgroundBoxCoordinates.size.x, backgroundBoxCoordinates.size.y});
    backgroundBox.setPosition({backgroundBoxCoordinates.position.x, backgroundBoxCoordinates.position.y});
    backgroundBox.setFillColor(backgroundColor);
    backgroundBox.setOutlineColor(Colors::GRAY);
    backgroundBox.setOutlineThickness(Lines::LINE_THICKNESS);
    window.draw(backgroundBox);

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
    window.draw(marketStatusText);
}

void MarketApp::drawMarketSession() {
    // Determine background color and text color based on marketOpen
    sf::Color backgroundColor = marketOpen ? Colors::WHITE : Colors::GRAY;
    sf::Color textColor = marketOpen ? Colors::BLACK : Colors::WHITE;

    // Draw background box
    auto backgroundBoxCoordinates = getGridBox(4, 1, 1, 1);
    sf::RectangleShape backgroundBox({backgroundBoxCoordinates.size.x, backgroundBoxCoordinates.size.y});
    backgroundBox.setPosition({backgroundBoxCoordinates.position.x, backgroundBoxCoordinates.position.y});
    backgroundBox.setFillColor(backgroundColor);
    backgroundBox.setOutlineColor(Colors::GRAY);
    backgroundBox.setOutlineThickness(Lines::LINE_THICKNESS);
    window.draw(backgroundBox);

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
    window.draw(marketStatusText);
}

void MarketApp::drawSymbolsHeaderRow(const float startY, const float labelX, const float priceX, const float changeX) {
    // Header row
    sf::Text headerSymbol(font, "SYMBOL");
    headerSymbol.setPosition({labelX, startY + Areas::PADDING});
    headerSymbol.setFillColor(Colors::BLACK);
    headerSymbol.setCharacterSize(TextSizes::LABEL);
    window.draw(headerSymbol);

    sf::Text headerPrice(font, "PRICE");
    headerPrice.setPosition({priceX, startY + Areas::PADDING});
    headerPrice.setFillColor(Colors::BLACK);
    headerPrice.setCharacterSize(TextSizes::LABEL);
    window.draw(headerPrice);

    sf::Text headerChange(font, "CHANGE");
    headerChange.setPosition({changeX, startY + Areas::PADDING});
    headerChange.setFillColor(Colors::BLACK);
    headerChange.setCharacterSize(TextSizes::LABEL);
    window.draw(headerChange);
}

void MarketApp::drawLabelsAndValues(const std::vector<StockData> &symbols, const float rowHeight, const float labelX, const float priceX, const float changeX, float currentY) {
    for (const auto &symbol: symbols) {
        // Symbol
        sf::Text labelText(font, symbol.ticker);
        labelText.setPosition({labelX, currentY});
        labelText.setFillColor(Colors::BLACK);
        labelText.setCharacterSize(TextSizes::LABEL);
        window.draw(labelText);

        // Price
        std::ostringstream priceStream;
        priceStream << "$" << std::fixed << std::setprecision(2) << symbol.price;
        sf::Text priceText(font, priceStream.str());
        priceText.setPosition({priceX, currentY});
        priceText.setFillColor(Colors::BLACK);
        priceText.setCharacterSize(TextSizes::LABEL);
        window.draw(priceText);

        // Change %
        std::ostringstream changeStream;
        if (symbol.changeFromOpen > 0) {
            changeStream << "+";
        }
        changeStream << std::fixed << std::setprecision(2) << symbol.changeFromOpen << "%";
        sf::Text changeText(font, changeStream.str());
        changeText.setPosition({changeX, currentY});
        changeText.setFillColor(Colors::BLACK);
        changeText.setCharacterSize(TextSizes::LABEL);
        window.draw(changeText);

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
