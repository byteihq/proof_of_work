#include "Config.hpp"
#include <fstream>
#include <spdlog/fmt/fmt.h>

#if defined DEBUG
static constexpr auto CFG_FILE_PATH = "config_debug.json";
#elif defined DOCKER
static constexpr auto CFG_FILE_PATH = "config_docker.json";
#else
static constexpr auto CFG_FILE_PATH = "config.json";
#endif


Config::Config()
{
    readFile();
}

void Config::readFile()
{
    std::ifstream cfgFile(CFG_FILE_PATH);
    if (!cfgFile)
        throw std::runtime_error(fmt::format("Failed to open {}", CFG_FILE_PATH));
    if (!Json::Reader().parse(cfgFile, _cfg))
        throw std::runtime_error(fmt::format("{} conatins invalid json", CFG_FILE_PATH));
}

Json::Value &Config::get()
{
    return _cfg;
}

Config *Config::GetInstance()
{
    static Config config;
    return &config;
}
