#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <sw/redis++/redis++.h>
#include <jsoncpp/json/json.h>
#include <TypeDefs.hpp>

class HashCash final
{
private:
    static constexpr const char *version = "H";
    static constexpr char delim = ':';
    static constexpr const char *hashAlgorithm = "SHA-256";

    std::unique_ptr<sw::redis::Redis> _redis;    
private:
    static int64_t expiresAt(uint16_t delta = 3); // 3 seconds
    static int64_t getTimestamp();
    static std::string genRandomBase64Str(size_t len = 10);

    HashCash() = default;
public:
    HashCash(const HashCash &) = delete;
    HashCash(HashCash &&) = delete;
    HashCash &operator=(const HashCash &) = delete;
    HashCash &operator=(HashCash &&) = delete;

    ResponseStatus isValid(std::string_view sourceIp, Json::Value &root);

    std::string createNewChallenge(std::string_view sourceIp, uint8_t difficulty, const std::string &url);

    static constexpr const char *str() noexcept
    {
        return "HashCash";
    }

public:
    void Init(const std::string &redisUrl);

    static HashCash *GetInstance();
};
