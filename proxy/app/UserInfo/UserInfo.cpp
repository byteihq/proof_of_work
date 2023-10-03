#include "UserInfo.hpp"
#include <Timer.hpp>
#include <jsoncpp/json/json.h>

constexpr auto HASH_CASH = "HashCash";
constexpr auto LAST_REQUEST = "LastRequest";
constexpr auto ACCESS_ENABLED = "AccessEnabled";

UserInfo::UserInfo() : _lastRequest(Timer::getTimestamp()), _accessEnabled(false)
{
}

UserInfo::UserInfo(const std::string &str) : _accessEnabled(false)
{
    Json::Value root;
    if (Json::Reader().parse(str, root))
    {
        if (root.isMember(HASH_CASH))
        {
            if (root[HASH_CASH].isString())
                _hashCash = root[HASH_CASH].asString();
        }

        if (root.isMember(LAST_REQUEST))
        {
            if (root[LAST_REQUEST].isInt64())
                _lastRequest = root[LAST_REQUEST].asInt64();
        }

        if (root.isMember(ACCESS_ENABLED))
        {
            if (root[ACCESS_ENABLED].isBool())
                _accessEnabled = root[ACCESS_ENABLED].asBool();
        }
    }
}

UserInfo::UserInfo(std::string hashCash, int64_t lastRequest, bool accessEnabled) : _hashCash(std::move(hashCash)), _lastRequest(lastRequest), _accessEnabled(accessEnabled)
{
}

std::string UserInfo::hashCash() const
{
    return _hashCash;
}

int64_t UserInfo::lastRequest() const noexcept
{
    return _lastRequest;
}

bool UserInfo::accessEnabled() const noexcept
{
    return _accessEnabled;
}

void UserInfo::setHashCash(std::string hashCash)
{
    _hashCash = std::move(hashCash);
}

void UserInfo::setLastRequest(int64_t timestamp) noexcept
{
    _lastRequest = timestamp;
}

void UserInfo::setAccess(bool access) noexcept
{
    _accessEnabled = access;
}

std::string UserInfo::toFormat() const
{
    Json::Value root;
    root[HASH_CASH] = _hashCash;
    root[LAST_REQUEST] = static_cast<Json::Value::Int64>(_lastRequest);
    root[ACCESS_ENABLED] = _accessEnabled;

    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    return Json::writeString(builder, root);
}
