#include "market_daemon_client.hpp"


std::map<std::string, MarketQuote> MarketDaemonClient::latestQuotes;
std::mutex MarketDaemonClient::quoteMutex;
std::atomic<bool> MarketDaemonClient::quotesReady(false);

void MarketDaemonClient::fetchAllQuotesAsync(std::atomic<bool>* fetchingFlag) {
    std::map<std::string, MarketQuote> quotes = MarketDaemonClient::getAllQuotes();

    {
        std::lock_guard<std::mutex> lock(quoteMutex);
        latestQuotes = std::move(quotes);
        quotesReady = true;
        Logger::debug("Finished fetching async quotes...");
    }

    *fetchingFlag = false;
}

bool MarketDaemonClient::areQuotesReady() {
    return quotesReady;
}

std::map<std::string, MarketQuote>& MarketDaemonClient::getLatestQuotes() {
    std::lock_guard<std::mutex> lock(quoteMutex);
    return latestQuotes;
}