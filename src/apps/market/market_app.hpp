#ifndef MARKET_APP_H
#define MARKET_APP_H

#pragma once
#include "../common/app.hpp"
#include <SFML/Graphics.hpp>

#include "../common/app_with_config.hpp"
#include "./market_daemon_client.hpp"
#include "../../core/state_machine/market_state.hpp"

class MarketApp : public AppWithConfig {
public:
    MarketApp(const std::string &appName, sf::RenderTarget &renderer, const sf::Font &font, StateMachine &stateMachine, MarketState &marketState);

    void handleEvent(const sf::Event::KeyPressed &keyPressed) override;

    void draw() override;

    void handleSettings() override;

    void handleHelp() override;

    void update(float deltaTime);

    void initConfigFromDisk() override;


private:
    MarketState &marketState;
    std::vector<std::string> symbols;
    std::vector<std::string> trackers;
    bool marketOpen = true;

    void drawStandaloneSymbols();

    void drawMarketTrackers();

    void drawMarketStatus();

    void drawMarketSession();

    void drawMarketSentiment();

    void drawStonksMeme();

    void drawSymbolsHeaderRow(float startY, float labelX, float priceX, float changeX);

    void drawLabelsAndValues(const std::map<std::string, MarketQuote> &quotes, float rowHeight, float labelX, float priceX, float changeX, float currentY);
};


#endif //MARKET_APP_H
