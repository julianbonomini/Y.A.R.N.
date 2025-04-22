#ifndef ALPHA_VANTAGE_H
#define ALPHA_VANTAGE_H

#include <nlohmann/json.hpp>

#include "../../common/logger.hpp"
#include "../../core/http/http.hpp"
#include <optional>

struct AlphaVantageGlobalQuote {
    std::string symbol;
    double open;
    double high;
    double low;
    double price;
    uint64_t volume;
    std::string latest_trading_day;
    double previous_close;
    double change;
    std::string change_percent;
};

class AlphaVantage {
public:
    static std::optional<AlphaVantageGlobalQuote> getGlobalQuote(const std::string& symbol) {
        std::string api_key = EnvConfig::load_env_config()["ALPHAVANTAGE_API_KEY"];
        std::map<std::string, std::string> query_params = {
            {"function", "GLOBAL_QUOTE"},
            {"symbol", symbol},
            {"apikey", api_key}
        };
        std::map<std::string, std::string> headers = {};
        std::string host = "www.alphavantage.co";
        std::string path = "/query";
        std::string scheme = "https";

        auto response = Http::GET(host, path, query_params, headers, scheme);
        if (response) {
            auto json_data = response.value();
            if (json_data.contains("Global Quote")) {
                auto j = json_data["Global Quote"];
                try {
                    AlphaVantageGlobalQuote quote{
                        j["01. symbol"].get<std::string>(),
                        std::stod(j["02. open"].get<std::string>()),
                        std::stod(j["03. high"].get<std::string>()),
                        std::stod(j["04. low"].get<std::string>()),
                        std::stod(j["05. price"].get<std::string>()),
                        static_cast<uint64_t>(std::stoull(j["06. volume"].get<std::string>())),
                        j["07. latest trading day"].get<std::string>(),
                        std::stod(j["08. previous close"].get<std::string>()),
                        std::stod(j["09. change"].get<std::string>()),
                        j["10. change percent"].get<std::string>()
                    };
                    return quote;
                } catch (const std::exception& e) {
                    Logger::error("AlphaVantage: Failed to parse Global Quote: " + std::string(e.what()));
                }
            } else {
                Logger::error("AlphaVantage: 'Global Quote' field missing.");
            }
        } else {
            Logger::error("Error: Could not get quote data!");
        }

        return std::nullopt;
    }


    // static nlohmann::json getMarketStatus() {
    //     std::string api_key = EnvConfig::load_env_config()["ALPHAVANTAGE_API_KEY"];
    //     std::map<std::string, std::string> query_params = {
    //         {"function", "MARKET_STATUS"},
    //         {"apikey", api_key}
    //     };
    //     std::map<std::string, std::string> headers = {};
    //     std::string host = "www.alphavantage.co";
    //     std::string path = "/query";
    //     std::string scheme = "https";
    //
    //     auto response = Http::GET(host, path, query_params, headers, scheme);
    //     if (response) {
    //         auto data = response.value();
    //         if (data.contains("market_open") || data.contains("markets")) {
    //             return data;
    //         } else if (data.contains("Error Message")) {
    //             Logger::error("AlphaVantage API error: " + data["Error Message"].get<std::string>());
    //         } else {
    //             Logger::error("AlphaVantage: Unexpected response format (market status).");
    //         }
    //     } else {
    //         Logger::error("Error: Could not retrieve market status.");
    //     }
    //
    //     return std::nullopt;
    // };
};

#endif //ALPHA_VANTAGE_H
