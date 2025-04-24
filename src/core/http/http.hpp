#ifndef HTTP_H
#define HTTP_H

#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <optional>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

class Http {
public:
    static std::optional<nlohmann::json> GET(const std::string &host,
                                             const std::string &path,
                                             const std::map<std::string, std::string> &query_params,
                                             const std::map<std::string, std::string> &headers
    );

    static std::optional<nlohmann::json> GET_UNSECURE(const std::string &host,
                                                      const std::string &path,
                                                      const std::map<std::string,
                                                          std::string> &query_params,
                                                      const std::map<std::string,
                                                          std::string> &headers
    );

    static std::optional<nlohmann::json> SILENT_GET(const std::string &host,
                                                    const std::string &path,
                                                    const std::map<std::string,
                                                        std::string> &query_params,
                                                    const std::map<std::string,
                                                        std::string> &headers
    );

    static std::optional<nlohmann::json> POST(const std::string &host,
                                              const std::string &path,
                                              const nlohmann::json &body,
                                              const std::map<std::string, std::string> &headers
    );

    static std::optional<nlohmann::json> POST_UNSECURE(const std::string &host,
                                                       const std::string &path,
                                                       const nlohmann::json &body,
                                                       const std::map<std::string, std::string> &headers
    );

private:
    static std::string UrlEncode(const std::string &value);

    static std::string BuildQueryString(const std::map<std::string, std::string> &params);
};


#endif //HTTP_H
