#include "market_state.hpp"
#include <ctime>
#include <iomanip>
#include "../../common/logger.hpp"


MarketState::MarketState() {
    // Optional default initialization
    stocksQuotes = {};
    trackersQuotes = {};
}

void MarketState::updateMarketOpen(bool isOpen) {
    isMarketOpen = isOpen;
}

std::string MarketState::getFormattedNow() {
    std::time_t now = std::time(nullptr);
    std::tm *localTime = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(localTime, "%H:%M:%S");
    return oss.str();
}

void MarketState::updateStockQuotes(const std::map<std::string, MarketQuote> quotes) {
    stocksQuotes = quotes;
    lastStockUpdate = getFormattedNow();
}


void MarketState::updateTrackerQuotes(const std::map<std::string, MarketQuote> quotes) {
    trackersQuotes = quotes;
    lastTrackerUpdate = getFormattedNow();
}

std::map<std::string, MarketQuote> MarketState::getStockQuotes() const {
    return stocksQuotes;
}

std::map<std::string, MarketQuote> MarketState::getTrackerQuotes() const {
    return stocksQuotes;
}

bool MarketState::getIsMarketOpen() const {
    return isMarketOpen;
}

std::string MarketState::getLastStockUpdateTime() const {
    return lastStockUpdate;
}

std::string MarketState::getLastTrackerUpdateTime() const {
    return lastTrackerUpdate;
}