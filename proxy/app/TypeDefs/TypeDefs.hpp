#pragma once

#include <cstdint>

enum class ResponseStatus : uint8_t
{
    NO_ERROR,
    INVALID_JSON,
    HASH_CASH_NOT_FOUND,
    INVALID_HASH_CASH
};
