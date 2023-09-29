#include <thread>
#include <drogon/HttpAppFramework.h>
#include <RequestHandler.hpp>
#include <HashCash.hpp>

using namespace drogon;

using drogonCallback = std::function<void(const HttpResponsePtr &)>;

int main()
{
    HashCash::GetInstance()->Init("tcp://localhost:6379");
    app().addListener("0.0.0.0", 8080);
    app().setThreadNum(std::thread::hardware_concurrency() - 1);
    app().registerHandler("/", [](const HttpRequestPtr &, drogonCallback &&callback)
    {
        auto response = HttpResponse::newHttpResponse();
        response->setBody("Proxy Online!");
        callback(response);
    }, {Get});
    app().registerHandler("/weather", [](const HttpRequestPtr &request, drogonCallback &&callback)
    {
        auto ip = request->getLocalAddr().toIp();
        auto requestStatus = RequestHandler::isValid(ip, request->getBody());
        switch (requestStatus)
        {
        case ResponseStatus::NO_ERROR:
        {
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
            root["HashChallenge"] = HashCash::GetInstance()->createNewChallenge(ip, 5, "weather.com");
            auto response = HttpResponse::newHttpJsonResponse(root);
            response->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(response);
            break;
        }
        }
    }, {Post});

    app().run();
    
    return 0;
}
