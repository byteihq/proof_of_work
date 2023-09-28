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
        response->setBody("Weather App Online!");
        callback(response);
    }, {Get});
    app().registerHandler("/weather", [](const HttpRequestPtr &request, drogonCallback &&callback)
    {
        std::cout << request->getBody() << "\nIP: " << request->getLocalAddr().toIpPort() << std::endl;
        auto ip = request->getLocalAddr().toIp();
        auto requestStatus = RequestHandler::isValid(ip, request->getBody());
        std::cout << "Is valid request - " << std::boolalpha << (requestStatus == RequestHandler::Status::NO_ERROR) << std::endl;
        switch (requestStatus)
        {
        case RequestHandler::Status::NO_ERROR:
            callback(HttpResponse::newHttpResponse());
            break;
        case RequestHandler::Status::INVALID_JSON:
        {
            Json::Value root;
            root["msg"] = "Body should be valid json.";
            auto response = HttpResponse::newHttpJsonResponse(root);
            response->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(response);
            break;
        }
        case RequestHandler::Status::HASH_CASH_NOT_FOUND:
        case RequestHandler::Status::INVALID_HASH_CASH:
        {
            Json::Value root;
            root["msg"] = "HashCash is not valid. Solve a new challenge.";
            root["HashChallenge"] = HashCash::GetInstance()->createNewChallenge(ip, 17, "weather.com");
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
