#ifndef MARKET_DAEMON_CLIENT_H
#define MARKET_DAEMON_CLIENT_H

#pragma once

#include <string>
#include <vector>
#include <optional>
#include <map>
#include <nlohmann/json.hpp>
#include <mutex>
#include <atomic>

#include "../../core/http/http.hpp"
#include "../../common/logger.hpp"

struct MarketQuote {
    std::string symbol;
    double price;
    std::optional<double> changeFromOpen;
    std::optional<double> changeFromPreviousClose;
    std::string type;
};

struct MarketStatus {
    std::string name;
    std::string yfit_market_id;
    std::string status;
    std::string message;
};


class MarketDaemonClient {
public:
    static void fetchAllQuotesAsync(std::atomic<bool> *fetchingFlag);

    static bool areQuotesReady();

    static void setQuotesReadyToFalse() {
        quotesReady.store(false);
    }

    static std::map<std::string, MarketQuote> &getLatestQuotes();

    static bool setConfig(const std::vector<std::string> &symbols) {
        nlohmann::json payload;
        payload["symbols"] = symbols;

        std::string path = "/config";
        std::map<std::string, std::string> headers = {
            {"Content-Type", "application/json"},
        };

        // Perform the POST request
        auto response_json = Http::POST_UNSECURE("127.0.0.1:8080", path, payload, headers);

        // Handle the response
        if (response_json.has_value()) {
            nlohmann::json response = response_json.value();
            // Log the successful response (optional)
            Logger::debug("Config set successfully. Response:", response.dump(4));

            // You might want to check the response for a specific success indicator
            if (response.contains("status") && response["status"] == "success") {
                return true;
            } else if (response.contains("code") && response["code"] == 200) {
                return true;
            } else {
                std::cerr << "Warning: Config set, but success status not explicitly confirmed in response.\n";
                return true; // Or false, depending on how critical explicit confirmation is
            }
        }
        std::cerr << "Error setting config. POST request failed.\n";
        return false;
    };

    static std::optional<MarketQuote> getCachedQuote(const std::string &symbol) {
        std::map<std::string, std::string> query_params = {};
        std::map<std::string, std::string> headers = {};
        std::string path = "/cached-quote" + symbol;

        auto response = Http::GET_UNSECURE("127.0.0.1:8080", path, query_params, headers);

        if (!response || response->contains("error")) return std::nullopt;

        MarketQuote quote;
        quote.symbol = response->at("symbol");
        quote.price = response->at("price");
        quote.type = response->at("type");
        if (response->contains("percent_change_from_open"))
            quote.changeFromOpen = response->at("percent_change_from_open");
        if (response->contains("percent_change_from_prev_close"))
            quote.changeFromPreviousClose = response->at("percent_change_from_prev_close");

        return quote;
    };

    static std::optional<MarketQuote> getQuote(const std::string &symbol) {
        std::map<std::string, std::string> query_params = {};
        std::map<std::string, std::string> headers = {};
        std::string path = "/quote" + symbol;

        auto response = Http::GET_UNSECURE("127.0.0.1:8080", path, query_params, headers);

        if (!response || response->contains("error")) return std::nullopt;

        MarketQuote quote;
        quote.symbol = response->at("symbol");
        quote.price = response->at("price");
        quote.type = response->at("type");
        if (response->contains("percent_change_from_open"))
            quote.changeFromOpen = response->at("percent_change_from_open");
        if (response->contains("percent_change_from_prev_close"))
            quote.changeFromPreviousClose = response->at("percent_change_from_prev_close");

        return quote;
    };

    static bool ready() {
        std::map<std::string, std::string> query_params = {};
        std::map<std::string, std::string> headers = {};
        std::string path = "/ready";

        auto response = Http::GET_UNSECURE("127.0.0.1:8080", path, query_params, headers);

        if (response && (*response)["status"] == "ok") {
            return true;
        }

        return false;
    };

    static std::optional<MarketStatus> getMarketStatus() {
        std::map<std::string, std::string> query_params = {};
        std::map<std::string, std::string> headers = {};
        std::string path = "/market-status";
        Logger::debug("a");
        auto response = Http::GET_UNSECURE("127.0.0.1:8080", path, query_params, headers);
        Logger::debug("b");
        if (!response || response->contains("error")) return std::nullopt;

        if (response && (*response)["status"] == "ok") {
            Logger::debug("c");
            MarketStatus status;
            status.name = response->at("name");
            status.yfit_market_id = response->at("yfit_market_id");
            status.status = response->at("status");
            status.message = response->at("message");
            Logger::debug("d");
            return status;
        }

        return std::nullopt;
    };

    static bool isMarketOpen() {
        std::map<std::string, std::string> query_params = {};
        std::map<std::string, std::string> headers = {};
        std::string path = "/market-status";

        auto response = Http::GET_UNSECURE("127.0.0.1:8080", path, query_params, headers);

        if (!response || response->contains("error")) return false;

        if (response) {
            return response->at("status") == "open";
        }

        return false;
    };

    static std::map<std::string, MarketQuote> getAllQuotes() {
        std::map<std::string, std::string> query_params = {};
        std::map<std::string, std::string> headers = {};
        std::string host = "127.0.0.1:8080";
        std::string path = "/quotes";

        auto response = Http::GET_UNSECURE(host, path, query_params, headers);

        std::map<std::string, MarketQuote> result;
        for (auto &[key, val]: response->items()) {
            MarketQuote quote;
            quote.symbol = key;
            quote.price = val["price"];
            quote.type = val["type"];
            if (val.contains("percent_change_from_open"))
                quote.changeFromOpen = val["percent_change_from_open"];
            if (val.contains("percent_change_from_prev_close"))
                quote.changeFromPreviousClose = val["percent_change_from_prev_close"];
            result[key] = quote;
        }

        return result;
    };

private:
    static std::map<std::string, MarketQuote> latestQuotes;
    static std::mutex quoteMutex;
    static std::atomic<bool> quotesReady;
};

#endif //MARKET_DAEMON_CLIENT_H
