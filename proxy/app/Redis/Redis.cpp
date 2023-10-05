#include "Redis.hpp"
#include <exception>

Redis::Redis(Config &cfg)
{
    auto json = cfg.get();
    if (!json.isMember("redis") || !json["redis"].isMember("url") || !json["redis"]["url"].isString())
        throw std::runtime_error("Invalid redis url");
    _redis = std::make_unique<sw::redis::Redis>(json["redis"]["url"].asString());
}

sw::redis::Redis &Redis::operator()()
{
    return *_redis.get();
}
