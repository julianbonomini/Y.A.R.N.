#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "http.hpp"
#include "httplib.h"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <memory>

std::optional<nlohmann::json> Http::GET(
    const std::string &host,
    const std::string &path,
    const std::map<std::string, std::string> &query_params,
    const std::map<std::string, std::string> &headers,
    const std::string &scheme
) {
    std::unique_ptr<httplib::SSLClient> client;

    // if (scheme == "https") {
    // Use SSLClient for HTTPS
    client = std::make_unique<httplib::SSLClient>(host);
    // } else {
    // Use regular Client for HTTP
    // client = std::make_unique<httplib::Client>(host);
    // }

    std::string full_path = path;
    std::string query = BuildQueryString(query_params);
    if (!query.empty()) {
        full_path += "?" + query;
    }

    httplib::Headers req_headers;
    for (const auto &[key, value]: headers) {
        req_headers.emplace(key, value);
    }

    auto res = client->Get(full_path.c_str(), req_headers);

    if (res && res->status == 200) {
        try {
            auto j = nlohmann::json::parse(res->body);
            return j;
        } catch (nlohmann::json::parse_error &e) {
            std::cerr << "[HttpClient] JSON parse error: " << e.what() << std::endl;
            return std::nullopt;
        }
    } else {
        std::cerr << "[HttpClient] Request failed or status != 200\n";
        if (res) std::cerr << "Response status: " << res->status << "\n";
        return std::nullopt;
    }
}

std::string Http::UrlEncode(const std::string &value) {
    std::ostringstream encoded;
    for (unsigned char c: value) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded << c;
        } else {
            encoded << '%' << std::uppercase << std::setw(2)
                    << std::setfill('0') << std::hex << static_cast<int>(c);
        }
    }
    return encoded.str();
}

std::string Http::BuildQueryString(const std::map<std::string, std::string> &params) {
    std::ostringstream query;
    bool first = true;
    for (const auto &[key, value]: params) {
        if (!first) query << '&';
        query << UrlEncode(key) << '=' << UrlEncode(value);
        first = false;
    }
    return query.str();
}
