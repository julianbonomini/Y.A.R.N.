#include "market_app.hpp"
#include<iostream>
#include <sstream>
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
    drawHeader();
    drawStockList();
}

void MarketApp::drawHeader() {
    float startingYPosition = TOP_LEFT.y + Areas::PADDING;
    float labelPositionX = TOP_LEFT.x + Areas::PADDING;
    float pricePositionX = TOP_LEFT.x + Areas::PADDING + 100.f;
    float changePositionX = TOP_LEFT.x + Areas::PADDING + 200.f;

    sf::Text labelText(font, "SYMBOL");
    labelText.setPosition({labelPositionX, startingYPosition});
    labelText.setFillColor(Colors::BLACK);
    labelText.setCharacterSize(TextSizes::LABEL);
    window.draw(labelText);

    sf::Text priceText(font, "PRICE");
    priceText.setPosition({pricePositionX, startingYPosition});
    priceText.setFillColor(Colors::BLACK);
    priceText.setCharacterSize(TextSizes::LABEL);
    window.draw(priceText);

    sf::Text changeText(font, "CHANGE");
    changeText.setPosition({changePositionX, startingYPosition});
    changeText.setFillColor(Colors::BLACK);
    changeText.setCharacterSize(TextSizes::LABEL);
    window.draw(changeText);
}

void MarketApp::drawStockList() {
    float labelPositionX = TOP_LEFT.x + Areas::PADDING;
    float pricePositionX = TOP_LEFT.x + Areas::PADDING + 100.f;
    float changePositionX = TOP_LEFT.x + Areas::PADDING + 200.f;
    float verticalOffset = Areas::PADDING + 35.f;

    // Iterate over all key-value pairs
    for (const auto &symbol: stocks) {
        sf::Text labelText(font, symbol.ticker);
        labelText.setPosition({labelPositionX, TOP_LEFT.y + verticalOffset});
        labelText.setFillColor(Colors::BLACK);
        labelText.setCharacterSize(TextSizes::LABEL);
        window.draw(labelText);

        std::ostringstream priceStream;
        priceStream << "$" << std::fixed << std::setprecision(2) << symbol.price;
        sf::Text priceText(font, priceStream.str());
        priceText.setPosition({pricePositionX, TOP_LEFT.y + verticalOffset});
        priceText.setFillColor(Colors::BLACK);
        priceText.setCharacterSize(TextSizes::LABEL);
        window.draw(priceText);

        std::ostringstream changeStream;
        if (symbol.changeFromOpen > 0) {
            changeStream << "+";
        }
        changeStream << std::fixed << std::setprecision(2) << symbol.changeFromOpen << "%";
        sf::Text changeText(font, changeStream.str());
        changeText.setPosition({changePositionX, TOP_LEFT.y + verticalOffset});
        changeText.setFillColor(Colors::BLACK);
        changeText.setCharacterSize(TextSizes::LABEL);
        window.draw(changeText);

        verticalOffset += 25.f;
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
        {"MU", 849.45f, +5.38f},
        // {"ASML", 849.45f, +5.38f},
        // {"SMCI", 849.45f, +5.38f},
        // {"AMAT", 849.45f, +5.38f},
        // {"AVGO", 849.45f, +5.38f},
        // {"LRXC", 849.45f, +5.38f}
    };
}
