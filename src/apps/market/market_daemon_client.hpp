
#ifndef MARKET_DAEMON_CLIENT_H
#define MARKET_DAEMON_CLIENT_H

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

struct MarketQuote {
    std::string symbol;
    double price;
    std::optional<double> changeFromOpen;
    std::optional<double> changeFromPreviousClose;
};

class MarketDaemonClient {
public:
    explicit MarketDaemonClient(const std::string& baseUrl = "http://localhost:8080");

    bool setConfig(const std::vector<std::string>& symbols);
    std::optional<MarketQuote> getCachedPrice(const std::string& symbol);
    std::unordered_map<std::string, MarketQuote> refreshQuotes();

private:
    std::string baseUrl;

    static size_t curlWriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
    std::optional<nlohmann::json> httpGet(const std::string& url);
    std::optional<nlohmann::json> httpPost(const std::string& url, const nlohmann::json& body);
};

#endif //MARKET_DAEMON_CLIENT_H
