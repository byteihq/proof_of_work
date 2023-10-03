#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <jsoncpp/json/json.h>
#include <TypeDefs.hpp>
#include <UserInfo.hpp>

class HashCash final
{
private:
    struct Challenge
    {
        static constexpr char version = 'H';
        static constexpr char delim = ':';
        static constexpr const char *hashAlgorithm = "SHA-256";

        int64_t expiresAt = 0;
        uint8_t bitsToZero = 0;
        std::pair<std::string, std::string> hashCash;

        bool parse(const std::string &data);
    };

private:
    static int64_t expiresAt(uint16_t delta = 3); // 3 seconds
    static std::string genRandomBase64Str(size_t len = 10);

public:
    static ResponseStatus isValid(Json::Value &root, const UserInfo &ui);

    static std::string createNewChallenge(uint8_t difficulty, const std::string &url);

    static constexpr const char *str() noexcept
    {
        return "HashCash";
    }
};
