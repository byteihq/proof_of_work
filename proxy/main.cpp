#include <iostream>
#include <Redis.hpp>
#include <Config.hpp>
#include <Proxy.hpp>

int main()
{
    try
    {
        auto cfg = Config::GetInstance();
        Redis redis(*cfg);
        Proxy::init(*cfg, redis);
        Proxy::run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
