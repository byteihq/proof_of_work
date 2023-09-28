#pragma once

#include <jsoncpp/json/json.h>

struct WeatherResp
{
    int8_t temp;

    Json::Value json();
};

class Weather final
{
public:
    static WeatherResp get(std::string_view);
};
