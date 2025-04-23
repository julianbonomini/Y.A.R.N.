#include "market_daemon_client.hpp"
#include <sstream>
#include <iostream>

MarketDaemonClient::MarketDaemonClient(const std::string& baseUrl)
    : baseUrl(baseUrl) {}

size_t MarketDaemonClient::curlWriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::optional<nlohmann::json> MarketDaemonClient::httpGet(const std::string& url) {
    CURL* curl = curl_easy_init();
    if (!curl) return std::nullopt;

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) return std::nullopt;

    try {
        return nlohmann::json::parse(response);
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<nlohmann::json> MarketDaemonClient::httpPost(const std::string& url, const nlohmann::json& body) {
    CURL* curl = curl_easy_init();
    if (!curl) return std::nullopt;

    std::string response;
    std::string jsonStr = body.dump();

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) return std::nullopt;

    try {
        return nlohmann::json::parse(response);
    } catch (...) {
        return std::nullopt;
    }
}

bool MarketDaemonClient::setConfig(const std::vector<std::string>& symbols) {
    nlohmann::json payload;
    payload["symbols"] = symbols;
    auto response = httpPost(baseUrl + "/config", payload);
    return response.has_value();
}

std::optional<MarketQuote> MarketDaemonClient::getCachedPrice(const std::string& symbol) {
    std::string url = baseUrl + "/cached-price/" + symbol;
    auto response = httpGet(url);
    if (!response || response->contains("error")) return std::nullopt;

    MarketQuote quote;
    quote.symbol = response->at("symbol");
    quote.price = response->at("price");
    if (response->contains("changeFromOpen"))
        quote.changeFromOpen = response->at("changeFromOpen");
    if (response->contains("changeFromPreviousClose"))
        quote.changeFromPreviousClose = response->at("changeFromPreviousClose");

    return quote;
}

std::unordered_map<std::string, MarketQuote> MarketDaemonClient::refreshQuotes() {
    std::unordered_map<std::string, MarketQuote> result;
    auto response = httpGet(baseUrl + "/quotes");
    if (!response) return result;

    for (auto& [key, val] : response->items()) {
        MarketQuote quote;
        quote.symbol = key;
        quote.price = val["price"];
        if (val.contains("changeFromOpen"))
            quote.changeFromOpen = val["changeFromOpen"];
        if (val.contains("changeFromPreviousClose"))
            quote.changeFromPreviousClose = val["changeFromPreviousClose"];
        result[key] = quote;
    }

    return result;
}
