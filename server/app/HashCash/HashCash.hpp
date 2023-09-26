#pragma once

#include <string>
#include <string_view>

class HashCash final
{
private:
    static constexpr const char *version = "H";
    static constexpr char delim = ':';
    static constexpr const char *hashAlgorithm = "SHA-256";

private:
    static int64_t expiresAt(uint16_t delta = 3); // 3 seconds
    static std::string genRandomBase64Str(size_t len = 10);

public:
    static bool isValid(std::string_view hashCash);

    static std::string createNewChallenge(uint8_t difficulty, const std::string &url);

    static constexpr const char *str() noexcept
    {
        return "HashCash";
    }
};
