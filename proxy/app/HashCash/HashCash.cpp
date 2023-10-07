#include "HashCash.hpp"
#include "base64.hpp"
#include <picosha2.h>
#include <Timer.hpp>

#include <chrono>
#include <random>
#include <cstring>
#include <algorithm>
#include <vector>
#include <bitset>
#include <regex>

constexpr auto HASH_CASH_EXPIRES_TIME = 2000; // 2 seconds

bool HashCash::Challenge::parse(const std::string &data)
{
    std::regex reg(R"((^H:([1-9][0-9]*):([1-9][0-9]*):([a-zA-Z0-9:/.]+):SHA-256:([a-zA-Z0-9=]+)):([a-zA-Z0-9=]+)$)");
    std::smatch matches;
    if (std::regex_match(data, matches, reg))
    {
        expiresAt = std::stoll(matches[3].str());
        bitsToZero = std::stoul(matches[2].str());
        hashCash.first = matches[1].str();
        hashCash.second = matches[6].str();
        return true;
    }
    return false;
}

ResponseStatus HashCash::isValid(Json::Value &root, const UserInfo &ui)
{
    if (!root.isMember(HashCash::str()))
        return ResponseStatus::HASH_CASH_NOT_FOUND;
    if (!root[HashCash::str()].isString())
        return ResponseStatus::INVALID_HASH_CASH;

    auto hashCash = root[HashCash::str()].asString();
    Challenge challengeParams;
    if (!challengeParams.parse(hashCash))
        return ResponseStatus::INVALID_HASH_CASH;

    if (ui.hashCash() != challengeParams.hashCash.first || challengeParams.expiresAt < Timer::getTimestamp())
        return ResponseStatus::INVALID_HASH_CASH;

    auto numberZeros = challengeParams.bitsToZero;

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

    if (std::all_of(bitsHash.begin(), bitsHash.begin() + numberZeros, [](uint8_t c){ return c == 0; }))
    {
        return ResponseStatus::NO_ERROR;
    }
    return ResponseStatus::INVALID_HASH_CASH;
}

int64_t HashCash::expiresAt(uint16_t delta)
{
    return Timer::getTimestamp() + delta;
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

uint16_t HashCash::getDifficulty(const UserInfo &ui, const DiffParams &dp)
{
    uint32_t minDelayMs = 1000 / dp.max_requests_per_second;
    uint32_t curDelayMs = (Timer::getTimestamp() - ui.lastRequest());
    if (curDelayMs >= minDelayMs)
        return dp.min_difficulty;
    if (curDelayMs == 0 || minDelayMs / curDelayMs > 4)
        return dp.max_difficulty;
    return std::min(static_cast<uint16_t>(dp.min_difficulty + static_cast<uint8_t>(std::pow(3u, static_cast<float>(minDelayMs) / curDelayMs))), dp.max_difficulty);
}

std::string HashCash::createNewChallenge(const UserInfo &ui, const DiffParams &dp)
{
    std::string challenge;

    challenge += HashCash::Challenge::version;                                                              // version
    challenge += HashCash::Challenge::delim;
    challenge += std::to_string(getDifficulty(ui, dp)) + HashCash::Challenge::delim;                        // difficulty
    challenge += std::to_string(HashCash::expiresAt(HASH_CASH_EXPIRES_TIME)) + HashCash::Challenge::delim;  // expires at
    challenge += dp.url + HashCash::Challenge::delim;                                                       // url
    challenge += HashCash::Challenge::hashAlgorithm;                                                        // hash algorithm
    challenge += HashCash::Challenge::delim;
    challenge += HashCash::genRandomBase64Str();

    return challenge;
}
