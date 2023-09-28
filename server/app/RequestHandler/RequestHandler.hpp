#pragma once

#include <string_view>

class RequestHandler final
{
public:
    enum class Status : uint8_t
    {
        NO_ERROR,
        INVALID_JSON,
        HASH_CASH_NOT_FOUND,
        INVALID_HASH_CASH
    };

public:
    static Status isValid(std::string_view sourceIp, std::string_view body);
};
