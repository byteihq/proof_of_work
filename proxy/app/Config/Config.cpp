#include "Config.hpp"
#include <fstream>

#ifdef DEBUG
static constexpr auto CFG_FILE_PATH = "config_debug.json";
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
        return;
    Json::Reader().parse(cfgFile, _cfg);
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
