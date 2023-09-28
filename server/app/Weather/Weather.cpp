#include "Weather.hpp"

#include <thread>
#include <chrono>
#include <random>

Json::Value WeatherResp::json()
{
    Json::Value root;
    root["temp"] = temp;
    return root;
}

WeatherResp Weather::get(std::string_view)
{
    std::random_device rng;
    std::uniform_int_distribution<uint16_t> delay(0, 1000);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay(rng)));

    std::uniform_int_distribution<int8_t> temp(-40, 40);
    return {temp(rng)};
}
