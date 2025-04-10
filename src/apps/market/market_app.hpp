#ifndef MARKET_APP_H
#define MARKET_APP_H

#pragma once
#include "../common/app.hpp"
#include <SFML/Graphics.hpp>

#include "../common/app_with_config.hpp"

// Represents a single stock's data
struct StockData {
    std::string ticker;
    float price;
    float changeFromOpen;
};

class MarketApp : public AppWithConfig {
public:
    MarketApp(sf::RenderWindow &window, const sf::Font &font, StateMachine &stateMachine, const std::string &appName);

    void handleEvent(const sf::Event::KeyPressed &keyPressed) override;

    void draw() override;

    void handleSettings() override;

    void handleHelp() override;

    void update(float deltaTime);

private:
    sf::Font font;
    sf::RenderWindow &window;

    std::vector<StockData> stocks;
    std::vector<StockData> marketTrackers;
    bool marketOpen = true;

    void drawStandaloneSymbols();

    void drawMarketTrackers();

    void drawMarketStatus();

    void drawMarketSession();

    void drawSymbolsHeaderRow(float startY, float labelX, float priceX, float changeX);

    void drawLabelsAndValues(const std::vector<StockData> &symbols, float rowHeight, float labelX, float priceX, float changeX, float currentY);

    void loadMockData(); // For now, simulate some data

    void initConfigFromDisk() override;
};


#endif //MARKET_APP_H
