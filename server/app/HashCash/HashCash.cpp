#include "HashCash.hpp"
#include "base64.hpp"
#include <picosha2.h>

#include <chrono>
#include <random>
#include <cstring>
#include <algorithm>
#include <vector>
#include <bitset>

constexpr auto EXPIRES_TIME = 2; // 2 seconds

bool HashCash::isValid(std::string_view sourceIp, std::string_view hashCash)
{
    auto lastDelimPos = std::find(hashCash.rbegin(), hashCash.rend(), HashCash::delim);
    if (lastDelimPos == hashCash.rend())
        return false;

    if (_redis->get(sourceIp).value_or("") != std::string(hashCash.begin(), lastDelimPos.base() - 1))
        return false;

    auto firstDelimPos = std::find(hashCash.begin(), hashCash.end(), HashCash::delim);
    auto secondDelimPos = std::find(firstDelimPos + 1, hashCash.end(), HashCash::delim);

    auto numberZeros = std::stoi(std::string(firstDelimPos + 1, secondDelimPos));

    std::vector<uint8_t> hash(picosha2::k_digest_size);
    picosha2::hash256(hashCash.begin(), hashCash.end(), hash.begin(), hash.end());
    
    std::vector<uint8_t> bitsHash;
    bitsHash.reserve(hash.size() * 8);
    for (auto b : hash)
    {
        std::bitset<8> bits(b);
        for (int i = 7; i >= 0; --i)
            bitsHash.push_back(bits[i]);
    }

    return std::all_of(bitsHash.begin(), bitsHash.begin() + numberZeros, [](uint8_t c) { return c == 0; });
}

int64_t HashCash::getTimestamp()
{
    return std::chrono::duration_cast<std::chrono::seconds>(
    std::chrono::high_resolution_clock::now().time_since_epoch())
    .count();
}

int64_t HashCash::expiresAt(uint16_t delta)
{
    return HashCash::getTimestamp() + delta;
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
    std::generate_n(result.begin(), len, [&]()
                    { return chars[dst(rng)]; });

    return base64::to_base64(result);
}

std::string HashCash::createNewChallenge(std::string_view sourceIp, uint8_t difficulty, const std::string &url)
{
    std::string challenge;

    challenge += HashCash::version;                                                     // version
    challenge += HashCash::delim;
    challenge += std::to_string(difficulty) + HashCash::delim;                          // difficulty
    challenge += std::to_string(HashCash::expiresAt(EXPIRES_TIME)) + HashCash::delim;   // expires at
    challenge += url + HashCash::delim;                                                 // url
    challenge += HashCash::hashAlgorithm;                                               // hash algorithm
    challenge += HashCash::delim;
    challenge += HashCash::genRandomBase64Str();

    _redis->set(sourceIp, challenge);
    _redis->expire(sourceIp, EXPIRES_TIME);

    return challenge;
}

HashCash *HashCash::GetInstance()
{
    static HashCash hashcash;
    return &hashcash;
}

void HashCash::Init(const std::string &redisUrl)
{
    _redis = std::make_unique<sw::redis::Redis>(redisUrl);
}
