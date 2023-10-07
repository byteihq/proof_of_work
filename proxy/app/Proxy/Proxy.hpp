#pragma once

#include <Config.hpp>
#include <Redis.hpp>

#include <jsoncpp/json/json.h>
#include <drogon/HttpAppFramework.h>
#include <TypeDefs.hpp>
#include <optional>

class Proxy
{
public:
    struct Link
    {
        std::string url;
        drogon::HttpMethod method;
        bool hash_challenge_enabled;
        std::string redirection;
        uint32_t max_requests_per_second;
        uint16_t min_difficulty;
        uint16_t max_difficulty;

        DiffParams toDiffParams() const;
    };

private:
    static std::optional<Link> parseLink(Json::Value &root);

public:
    static void init(Config &cfg, Redis &redis);

    static void run();
};
