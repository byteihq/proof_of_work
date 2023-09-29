#include "Api.hpp"

#include <thread>
#include <chrono>
#include <random>

Json::Value ApiResp::json()
{
    Json::Value root;
    root["temp"] = temp;
    return root;
}

ApiResp Api::getResponse(const ApiRequest &)
{
    std::random_device rng;
    std::uniform_int_distribution<uint16_t> delay(0, 1000);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay(rng)));

    std::uniform_int_distribution<int8_t> temp(-40, 40);
    return {temp(rng)};
}

std::optional<ApiRequest> Api::tryParseRequest(std::string_view body)
{
    Json::Value root;
    if (!Json::Reader().parse(body.data(), root))
        return std::nullopt;

    if (!root.isMember("city"))
        return std::nullopt;
    if (!root["city"].isString())
        return std::nullopt;

    return ApiRequest{root["city"].asString()};
}
