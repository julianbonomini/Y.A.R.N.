#include "market_app.hpp"
#include<iostream>
#include <sstream>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include <string>

MarketApp::MarketApp(sf::RenderWindow &window, const sf::Font &font, const std::string &appName)
    : App(appName), font(font), window(window) {
    loadMockData();
}

void MarketApp::handleEvent(const sf::Event::KeyPressed &keyPressed) {
    std::cout << "Handle evet" << std::endl;
}

void MarketApp::update(float /*deltaTime*/) {
    std::cout << "Update" << std::endl;
}

void MarketApp::draw() {
    drawStandaloneSymbols();
}

void MarketApp::drawStandaloneSymbols() {

    const float rowHeight = 30.f;
    const float startX = TOP_LEFT.x + Areas::PADDING;
    const float startY = TOP_LEFT.y + Areas::PADDING;

    const float labelX = startX + Areas::PADDING;
    const float priceX = labelX + 120.f;
    const float changeX = priceX + 100.f;

    // Draw background box
    sf::RectangleShape backgroundBox;
    backgroundBox.setPosition({startX, startY});
    backgroundBox.setSize({changeX + 80.f - startX, Areas::MAIN_APP_HEIGHT - 2 * Areas::PADDING});
    backgroundBox.setFillColor(Colors::WHITE);
    backgroundBox.setOutlineColor(Colors::GRAY);
    backgroundBox.setOutlineThickness(Lines::LINE_THICKNESS);
    window.draw(backgroundBox);

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

    // Stock rows
    float currentY = startY + Areas::PADDING + rowHeight;

    for (const auto &symbol : stocks) {
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
}
