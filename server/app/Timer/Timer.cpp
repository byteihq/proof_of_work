#include "Timer.hpp"

Timer::Timer() : _timestamp(std::chrono::high_resolution_clock::now())
{
}

void Timer::set()
{
    _timestamp = std::chrono::high_resolution_clock::now();
}

int64_t Timer::ellapsed() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - _timestamp).count();
}
