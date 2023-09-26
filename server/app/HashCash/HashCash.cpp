#include "HashCash.hpp"
#include "base64.hpp"
#include <chrono>
#include <random>
#include <cstring>
#include <algorithm>

bool HashCash::isValid(std::string_view hashCash)
{
    // TODO: add examination
    return true;
}

int64_t HashCash::expiresAt(uint16_t delta)
{
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch())
        .count() + delta;
}

std::string HashCash::genRandomBase64Str(size_t len)
{
    static constexpr auto chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::random_device rng;
    std::uniform_int_distribution<size_t> dst(0, std::strlen(chars));
    std::string result(len, '\0');
    std::generate_n(result.begin(), len, [&](){ return chars[dst(rng)]; });
    
    return base64::to_base64(result);
}

std::string HashCash::createNewChallenge(uint8_t difficulty, const std::string &url)
{
    std::string challenge;

    challenge += HashCash::version; // version
    challenge += HashCash::delim;
    challenge += std::to_string(difficulty) + HashCash::delim; // difficulty
    challenge += std::to_string(HashCash::expiresAt()) + HashCash::delim; // expires at
    challenge += url + HashCash::delim; // url
    challenge += HashCash::hashAlgorithm; // hash algorithm
    challenge += + HashCash::delim;
    challenge += HashCash::genRandomBase64Str();

    return challenge;
}
