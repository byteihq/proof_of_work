#pragma once

#include <cstdint>
#include <string>

enum class ResponseStatus : uint8_t
{
    NO_ERROR,
    INVALID_JSON,
    INVALID_HASH_CASH,
    HASH_CASH_NOT_FOUND,
    TOO_MANY_REQUESTS
};

struct DiffParams
{
    std::string url;
    uint32_t max_requests_per_second;
    uint16_t min_difficulty;
    uint16_t max_difficulty;
};
