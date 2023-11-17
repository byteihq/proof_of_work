#include <Redis.hpp>
#include <Config.hpp>
#include <Proxy.hpp>
#include <spdlog/spdlog.h>

int main()
{
    try
    {
        auto cfg = Config::GetInstance();
        Redis redis(*cfg);
        spdlog::info("Proxy initializing...");
        Proxy::init(*cfg, redis);
        Proxy::run();
    }
    catch (const std::exception &e)
    {
        spdlog::error("Error: '{}'", e.what());
    }
    return 0;
}
