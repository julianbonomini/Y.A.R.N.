#ifndef MARKET_APP_H
#define MARKET_APP_H

#pragma once
#include "../common/app.hpp"
#include <SFML/Graphics.hpp>

// Represents a single stock's data
struct StockData {
    std::string ticker;
    float price;
    float changeFromOpen;
};

class MarketApp : public App {
public:
    MarketApp(sf::RenderWindow &window, const sf::Font &font, const std::string &appName);

    void handleEvent(const sf::Event::KeyPressed &keyPressed) override;

    void draw() override;

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

    void loadMockData(); // For now, simulate some data
};


#endif //MARKET_APP_H
