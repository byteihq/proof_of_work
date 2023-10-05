#pragma once

#include <sw/redis++/redis++.h>
#include <memory>
#include <Config.hpp>

class Redis
{
private:
    std::unique_ptr<sw::redis::Redis> _redis;

public:
    Redis(Config &cfg);

    sw::redis::Redis &operator()();

    ~Redis() = default;
};
