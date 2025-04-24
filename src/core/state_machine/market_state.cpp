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

void MarketState::updateAllQuotes(const std::map<std::string, MarketQuote> quotes) {
    allQuotes = quotes;
    std::map<std::string, MarketQuote> filteredStocksQuotes = {};
    std::map<std::string, MarketQuote> filteredTrackerQuotes = {};
    for (const auto& pair : quotes) {
        if (pair.second.type == "stock") {
            filteredStocksQuotes[pair.first] = pair.second;
        }
        if (pair.second.type == "index") {
            filteredTrackerQuotes[pair.first] = pair.second;
        }
    }
    stocksQuotes = filteredStocksQuotes;
    trackersQuotes = filteredTrackerQuotes;
    lastUpdate = getFormattedNow();
}

std::map<std::string, MarketQuote> MarketState::getStockQuotes() const {
    return stocksQuotes;
}

std::map<std::string, MarketQuote> MarketState::getTrackerQuotes() const {
    return trackersQuotes;
}

bool MarketState::getIsMarketOpen() const {
    return isMarketOpen;
}

std::string MarketState::getLastUpdate() const {
    return lastUpdate;
}