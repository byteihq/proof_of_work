#pragma once

#include <jsoncpp/json/json.h>
#include <string>
#include <string_view>
#include <optional>

struct ApiResp
{
    int8_t temp;

    Json::Value json();
};

struct ApiRequest
{
    std::string city;
};

class Api final
{
public:
    static ApiResp getResponse(const ApiRequest &);

    static std::optional<ApiRequest> tryParseRequest(std::string_view body);
};
