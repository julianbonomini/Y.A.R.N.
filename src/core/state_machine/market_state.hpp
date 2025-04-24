#ifndef MARKET_STATE_H
#define MARKET_STATE_H

#pragma once
#include <nlohmann/json.hpp>
#include <map>

#include "../../apps/market/market_daemon_client.hpp"

class MarketState {
public:
    MarketState();


    void updateStockQuotes(const std::map<std::string, MarketQuote> quotes);

    void updateTrackerQuotes(const std::map<std::string, MarketQuote> quotes);

    std::map<std::string, MarketQuote> getStockQuotes() const;

    std::map<std::string, MarketQuote> getTrackerQuotes() const;

private:
    std::map<std::string, MarketQuote> stocksQuotes;
    std::map<std::string, MarketQuote> trackersQuotes;
};


#endif //MARKET_STATE_H
