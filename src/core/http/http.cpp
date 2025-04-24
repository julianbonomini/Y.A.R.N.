#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "http.hpp"
#include "httplib.h"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <memory>

#include "../../common/logger.hpp"

std::optional<nlohmann::json> Http::GET(
    const std::string &host,
    const std::string &path,
    const std::map<std::string, std::string> &query_params,
    const std::map<std::string, std::string> &headers
) {
    std::unique_ptr<httplib::SSLClient> client;

    client = std::make_unique<httplib::SSLClient>(host);

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
            Logger::error("[HTTP_CLIENT]", "JSON parse error:", e.what());
            return std::nullopt;
        }
    }

    Logger::error("[HTTP_CLIENT]", "Request failed or status != 200");
    if (res) {
        Logger::error("[HTTP_CLIENT]", "Response status:", res->status);
    }
    return std::nullopt;
}

std::optional<nlohmann::json> Http::GET_UNSECURE(
    const std::string &host,
    const std::string &path,
    const std::map<std::string, std::string> &query_params,
    const std::map<std::string, std::string> &headers
) {
    std::unique_ptr<httplib::Client> client;

    client = std::make_unique<httplib::Client>(host);

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
            Logger::error("[HTTP_CLIENT]", "JSON parse error:", e.what());
            return std::nullopt;
        }
    }

    Logger::error("[HTTP_CLIENT]", "Request failed or status != 200");
    if (res) {
        Logger::error("[HTTP_CLIENT]", "Response status:", res->status);
    }
    return std::nullopt;
}

std::optional<nlohmann::json> Http::SILENT_GET(
    const std::string &host,
    const std::string &path,
    const std::map<std::string, std::string> &query_params,
    const std::map<std::string, std::string> &headers
) {
    std::unique_ptr<httplib::SSLClient> client;

    client = std::make_unique<httplib::SSLClient>(host);

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
            return std::nullopt;
        }
    }

    return std::nullopt;
}

std::optional<nlohmann::json> Http::POST(
    const std::string &host,
    const std::string &path,
    const nlohmann::json &body,
    const std::map<std::string, std::string> &headers
) {
    std::unique_ptr<httplib::SSLClient> client;

    client = std::make_unique<httplib::SSLClient>(host);

    httplib::Headers req_headers;
    for (const auto &[key, value]: headers) {
        req_headers.emplace(key, value);
    }

    // Ensure the Content-Type header is set for JSON
    req_headers.emplace("Content-Type", "application/json");

    std::string body_str = body.dump();
    auto res = client->Post(path.c_str(), req_headers, body_str, "application/json");

    if (res && res->status >= 200 && res->status < 300) {
        try {
            auto j = nlohmann::json::parse(res->body);
            return j;
        } catch (nlohmann::json::parse_error &e) {
            Logger::error("[HTTP_CLIENT]", "JSON parse error:", e.what());
            return std::nullopt;
        }
    }

    Logger::error("[HTTP_CLIENT]", "POST request failed or status not in [200-299]");
    if (res) {
        Logger::error("[HTTP_CLIENT]", "Response status:", res->status);
        Logger::error("[HTTP_CLIENT]", "Response body:", res->body);
    }
    return std::nullopt;
}

std::optional<nlohmann::json> Http::POST_UNSECURE(
    const std::string &host,
    const std::string &path,
    const nlohmann::json &body,
    const std::map<std::string, std::string> &headers
) {
    std::unique_ptr<httplib::Client> client;
    client = std::make_unique<httplib::Client>(host);

    httplib::Headers req_headers;
    for (const auto &[key, value]: headers) {
        req_headers.emplace(key, value);
    }

    // Ensure the Content-Type header is set for JSON
    req_headers.emplace("Content-Type", "application/json");

    std::string body_str = body.dump();
    auto res = client->Post(path.c_str(), req_headers, body_str, "application/json");

    if (res && res->status >= 200 && res->status < 300) {
        try {
            auto j = nlohmann::json::parse(res->body);
            return j;
        } catch (nlohmann::json::parse_error &e) {
            Logger::error("[HTTP_CLIENT]", "JSON parse error:", e.what());
            return std::nullopt;
        }
    }

    Logger::error("[HTTP_CLIENT]", "POST request failed or status not in [200-299]");
    if (res) {
        Logger::error("[HTTP_CLIENT]", "Response status:", res->status);
        Logger::error("[HTTP_CLIENT]", "Response body:", res->body);
    }
    return std::nullopt;
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
