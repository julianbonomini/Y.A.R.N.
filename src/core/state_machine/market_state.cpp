#include "market_state.hpp"


MarketState::MarketState() {
    // Optional default initialization
    stocksQuotes = {};
    trackersQuotes = {};
}

void MarketState::updateStockQuotes(const std::map<std::string, MarketQuote> quotes) {
    stocksQuotes = quotes;
}

void MarketState::updateTrackerQuotes(const std::map<std::string, MarketQuote> quotes) {
    trackersQuotes = quotes;
}

std::map<std::string, MarketQuote> MarketState::getStockQuotes() const {
    return stocksQuotes;
}

std::map<std::string, MarketQuote> MarketState::getTrackerQuotes() const {
    return stocksQuotes;
}
