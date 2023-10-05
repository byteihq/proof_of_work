#pragma once

#include <jsoncpp/json/json.h>

class Config final
{
private:
    Json::Value _cfg;

private:
    Config();
    void readFile();

public:
    Config(const Config &) = delete;
    Config(Config &&) = delete;
    Config &operator==(const Config &) = delete;
    Config &operator==(Config &&) = delete;

    Json::Value &get();

    static Config *GetInstance();
};
