#include "Proxy.hpp"

#include <RequestHandler.hpp>
#include <HashCash.hpp>
#include <UserInfo.hpp>
#include <Timer.hpp>

#define CREATE_PROXY_METHOD_WITH_HASH_CHALLENGE(link)                                                           \
app().registerHandler(link.url, [&redis, link](const HttpRequestPtr &request, drogonCallback &&callback)        \
{                                                                                                               \
    auto ip = request->getLocalAddr().toIp();                                                                   \
    std::unique_ptr<UserInfo> ui = std::make_unique<UserInfo>();                                                \
    if (auto value = redis().get(ip); value.has_value())                                                        \
    {                                                                                                           \
        ui = std::make_unique<UserInfo>(value.value());                                                         \
    }                                                                                                           \
    auto requestStatus = RequestHandler::isValid(request->getBody(), *ui.get());                                \
    ui->setLastRequest(Timer::getTimestamp());                                                                  \
    switch (requestStatus)                                                                                      \
    {                                                                                                           \
    case ResponseStatus::NO_ERROR:                                                                              \
    {                                                                                                           \
        ui->setAccess(true);                                                                                    \
        callback(HttpResponse::newRedirectionResponse(link.redirection, HttpStatusCode::k307TemporaryRedirect));\
        break;                                                                                                  \
    }                                                                                                           \
    case ResponseStatus::INVALID_JSON:                                                                          \
    {                                                                                                           \
        Json::Value root;                                                                                       \
        root["msg"] = "Body should be valid json.";                                                             \
        auto response = HttpResponse::newHttpJsonResponse(root);                                                \
        response->setStatusCode(HttpStatusCode::k400BadRequest);                                                \
        callback(response);                                                                                     \
        break;                                                                                                  \
    }                                                                                                           \
    case ResponseStatus::HASH_CASH_NOT_FOUND:                                                                   \
    case ResponseStatus::INVALID_HASH_CASH:                                                                     \
    {                                                                                                           \
        Json::Value root;                                                                                       \
        root["msg"] = "HashCash is not valid. Solve a new challenge.";                                          \
        root["HashChallenge"] = HashCash::createNewChallenge(10, "weather.com");                                \
        ui->setHashCash(root["HashChallenge"].asString());                                                      \
        auto response = HttpResponse::newHttpJsonResponse(root);                                                \
        response->setStatusCode(HttpStatusCode::k400BadRequest);                                                \
        callback(response);                                                                                     \
        break;                                                                                                  \
    }                                                                                                           \
    }                                                                                                           \
    redis().set(ip, ui->toFormat(), UserInfo::expiresTime());                                                   \
}, {link.method});

using namespace drogon;
using drogonCallback = std::function<void(const HttpResponsePtr &)>;

static std::optional<HttpMethod> strToDrogonMethod(std::string_view str)
{
    if (str == "get")
        return HttpMethod::Get;
    if (str == "post")
        return HttpMethod::Post;
    
    return {};
}

std::optional<Proxy::Link> Proxy::parseLink(Json::Value &root)
{
    Link link;
    if (!root.isMember("url") || !root["url"].isString())
        return {};
    link.url = root["url"].asString();

    if (!root.isMember("method") || !root["method"].isString())
        return {};
    if (auto value = strToDrogonMethod(root["method"].asString()); !value.has_value())
        return {};
    else
        link.method = value.value();

    if (!root.isMember("hash_challenge_enabled"))
    {
        link.hash_challenge_enabled = false;
    }
    else
    {
        if (!root["hash_challenge_enabled"].isBool())
            return {};
        else
            link.hash_challenge_enabled = root["hash_challenge_enabled"].asBool();
    }

    if (!root.isMember("redirection") || !root["redirection"].isString())
        return {};
    link.redirection = root["redirection"].asString();

    if (!root.isMember("max_requests_per_second"))
    {
        if (link.hash_challenge_enabled)
            return {};
        link.max_requests_per_second = 0;
    }
    else
    {
        if (!link.hash_challenge_enabled || !root["max_requests_per_second"].isUInt())
            return {};
        link.max_requests_per_second = root["max_requests_per_second"].asUInt();
    }

    return link;
}

void Proxy::init(Config &cfg, Redis &redis)
{
    auto json = cfg.get();
    if (!json.isMember("proxy") || !json["proxy"].isObject())
        return;
    auto proxy = json["proxy"];

    {
        std::string ip = "localhost";
        uint16_t port = 8080;
        if (proxy.isMember("listener_ip") && proxy["listener_ip"].isString())
        {
            ip = proxy["listener_ip"].asString();
        }
        if (proxy.isMember("listener_port") && proxy["listener_port"].isUInt())
        {
            port = proxy["listener_port"].asUInt();
        }

        app().addListener(ip, port);
    }

    {
        uint16_t threadsCount = std::thread::hardware_concurrency();
        if (proxy.isMember("max_threads") && proxy["max_threads"].isUInt())
        {
            threadsCount = proxy["max_threads"].asUInt();
        }
        app().setThreadNum(threadsCount);
    }
    app().registerHandler("/", [](const HttpRequestPtr &, drogonCallback &&callback)
    {
        auto response = HttpResponse::newHttpResponse();
        response->setBody("Proxy Online!");
        callback(response);
    }, {Get});

    if (!proxy.isMember("links") || !proxy["links"].isArray())
        return;
    
    auto links = proxy["links"];
    for (Json::Value::ArrayIndex i = 0; i < links.size(); ++i)
    {
        if (auto link = parseLink(links[i]); link.has_value())
        {
            // TODO add params checks
            auto lv = link.value();
            CREATE_PROXY_METHOD_WITH_HASH_CHALLENGE(lv);
        }
    }
}

void Proxy::run()
{
    app().run();
}