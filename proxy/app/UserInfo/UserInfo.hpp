#pragma once

#include <string>
#include <chrono>

class UserInfo final
{
private:
    std::string _hashCash;
    int64_t _lastRequest;
    bool _accessEnabled;

public:
    UserInfo();
    UserInfo(const std::string &str);
    UserInfo(std::string hashCash, int64_t lastRequest, bool accessEnabled);

    std::string hashCash() const;
    int64_t lastRequest() const noexcept;
    bool accessEnabled() const noexcept;

    void setHashCash(std::string hashCash);
    void setLastRequest(int64_t timestamp) noexcept;
    void setAccess(bool access) noexcept;

    std::string toFormat() const;

    static constexpr std::chrono::seconds expiresTime()
    {
        return std::chrono::seconds(600); // 10 min
    }

    ~UserInfo() = default;
};
