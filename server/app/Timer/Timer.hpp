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

    ~Timer() = default;
};
