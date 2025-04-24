#ifndef MARKET_STATE_H
#define MARKET_STATE_H

#pragma once
#include <nlohmann/json.hpp>
#include <map>

#include "../../apps/market/market_daemon_client.hpp"

class MarketState {
public:
    MarketState();

    void updateMarketOpen(bool isOpen);

    void updateAllQuotes(std::map<std::string, MarketQuote> quotes);

    std::map<std::string, MarketQuote> getStockQuotes() const;

    std::map<std::string, MarketQuote> getTrackerQuotes() const;

    bool getIsMarketOpen() const;

    std::string getLastUpdate() const;

private:
    std::map<std::string, MarketQuote> stocksQuotes;
    std::map<std::string, MarketQuote> trackersQuotes;
    std::map<std::string, MarketQuote> allQuotes;
    bool isMarketOpen;
    std::string lastUpdate;

    std::string getFormattedNow();
};


#endif //MARKET_STATE_H
