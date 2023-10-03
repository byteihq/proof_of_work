#pragma once

#include <chrono>

class Timer final
{
private:
    std::chrono::high_resolution_clock::time_point _timestamp;

public:
    Timer();

    void set();

    int64_t ellapsed() const;

    template <typename T = std::chrono::seconds>
    static int64_t getTimestamp()
    {
        return std::chrono::duration_cast<T>(
        std::chrono::high_resolution_clock::now().time_since_epoch())
        .count();
    }

    ~Timer() = default;
};
