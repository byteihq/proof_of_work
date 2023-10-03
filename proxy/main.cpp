#include <thread>
#include <memory>
#include <drogon/HttpAppFramework.h>
#include <RequestHandler.hpp>
#include <HashCash.hpp>
#include <UserInfo.hpp>
#include <Timer.hpp>
#include <sw/redis++/redis++.h>

using namespace drogon;

using drogonCallback = std::function<void(const HttpResponsePtr &)>;

int main()
{
    sw::redis::Redis redis("tcp://localhost:6379");
    app().addListener("0.0.0.0", 8080);
    app().setThreadNum(std::thread::hardware_concurrency() - 1);
    app().registerHandler("/", [](const HttpRequestPtr &, drogonCallback &&callback)
    {
        auto response = HttpResponse::newHttpResponse();
        response->setBody("Proxy Online!");
        callback(response);
    }, {Get});
    app().registerHandler("/weather", [&redis](const HttpRequestPtr &request, drogonCallback &&callback)
    {
        auto ip = request->getLocalAddr().toIp();
        std::unique_ptr<UserInfo> ui = std::make_unique<UserInfo>();
        if (auto value = redis.get(ip); value.has_value())
        {
            ui = std::make_unique<UserInfo>(value.value());
        }
        auto requestStatus = RequestHandler::isValid(request->getBody(), *ui.get());
        ui->setLastRequest(Timer::getTimestamp());

        switch (requestStatus)
        {
        case ResponseStatus::NO_ERROR:
        {
            ui->setAccess(true);
            callback(HttpResponse::newRedirectionResponse("http://localhost:8081/weather", HttpStatusCode::k307TemporaryRedirect));
            break;
        }
        case ResponseStatus::INVALID_JSON:
        {
            Json::Value root;
            root["msg"] = "Body should be valid json.";
            auto response = HttpResponse::newHttpJsonResponse(root);
            response->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(response);
            break;
        }
        case ResponseStatus::HASH_CASH_NOT_FOUND:
        case ResponseStatus::INVALID_HASH_CASH:
        {
            Json::Value root;
            root["msg"] = "HashCash is not valid. Solve a new challenge.";
            root["HashChallenge"] = HashCash::createNewChallenge(10, "weather.com");
            ui->setHashCash(root["HashChallenge"].asString());
            auto response = HttpResponse::newHttpJsonResponse(root);
            response->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(response);
            break;
        }
        }
        redis.set(ip, ui->toFormat(), UserInfo::expiresTime());
    }, {Post});

    app().run();

    return 0;
}
