#ifndef MARKET_DAEMON_CLIENT_H
#define MARKET_DAEMON_CLIENT_H

#pragma once

#include <string>
#include <vector>
#include <optional>
#include <map>
#include <nlohmann/json.hpp>

#include "../../core/http/http.hpp"

struct MarketQuote {
    std::string symbol;
    double price;
    std::optional<double> changeFromOpen;
    std::optional<double> changeFromPreviousClose;
};


class MarketDaemonClient {
public:
    static bool setConfig(const std::vector<std::string> &symbols) {
        nlohmann::json payload;
        payload["symbols"] = symbols;

        std::string path = "/config";
        std::map<std::string, std::string> headers = {
            {"Content-Type", "application/json"},
        };

        // Perform the POST request
        auto response_json = Http::POST("http://localhost:8080", path, payload, headers, "http");

        // Handle the response
        if (response_json.has_value()) {
            nlohmann::json response = response_json.value();
            // Log the successful response (optional)
            std::cout << "Config set successfully. Response:\n" << response.dump(4) << std::endl;

            // You might want to check the response for a specific success indicator
            if (response.contains("status") && response["status"] == "success") {
                return true;
            } else if (response.contains("code") && response["code"] == 200) {
                return true;
            } else {
                std::cerr << "Warning: Config set, but success status not explicitly confirmed in response.\n";
                return true; // Or false, depending on how critical explicit confirmation is
            }
        } else {
            std::cerr << "Error setting config. POST request failed.\n";
            return false;
        }
    };

    static std::optional<MarketQuote> getCachedQuote(const std::string &symbol) {
        std::map<std::string, std::string> query_params = {};
        std::map<std::string, std::string> headers = {};
        std::string path = "/cached-quote" + symbol;
        std::string scheme = "http";

        auto response = Http::GET("http://localhost:8080", path, query_params, headers, scheme);

        if (!response || response->contains("error")) return std::nullopt;

        MarketQuote quote;
        quote.symbol = response->at("symbol");
        quote.price = response->at("price");
        if (response->contains("changeFromOpen"))
            quote.changeFromOpen = response->at("changeFromOpen");
        if (response->contains("changeFromPreviousClose"))
            quote.changeFromPreviousClose = response->at("changeFromPreviousClose");

        return quote;
    };

    static std::optional<MarketQuote> getQuote(const std::string &symbol) {
        std::map<std::string, std::string> query_params = {};
        std::map<std::string, std::string> headers = {};
        std::string path = "/quote" + symbol;
        std::string scheme = "http";

        auto response = Http::GET("http://localhost:8080", path, query_params, headers, scheme);

        if (!response || response->contains("error")) return std::nullopt;

        MarketQuote quote;
        quote.symbol = response->at("symbol");
        quote.price = response->at("price");
        if (response->contains("changeFromOpen"))
            quote.changeFromOpen = response->at("changeFromOpen");
        if (response->contains("changeFromPreviousClose"))
            quote.changeFromPreviousClose = response->at("changeFromPreviousClose");

        return quote;
    };

    static bool ready() {
        std::map<std::string, std::string> query_params = {};
        std::map<std::string, std::string> headers = {};
        std::string path = "/ready";
        std::string scheme = "http";

        auto response = Http::SILENT_GET("http://localhost:8080", path, query_params, headers, scheme);

        if (response && (*response)["status"] == "ok") {
            return true;
        } else {
            return false;
        }
    };

    static std::map<std::string, MarketQuote> getAllQuotes() {
        std::map<std::string, std::string> query_params = {};
        std::map<std::string, std::string> headers = {};
        std::string host = "http://localhost:8080";
        std::string path = "/quotes";
        std::string scheme = "http";

        auto response = Http::GET(host, path, query_params, headers, scheme);

        std::map<std::string, MarketQuote> result;
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
    };
};

#endif //MARKET_DAEMON_CLIENT_H
