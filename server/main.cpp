#include <thread>
#include <drogon/HttpAppFramework.h>
#include <RequestHandler.hpp>

using drogonCallback = std::function<void(const drogon::HttpResponsePtr &)>;

int main()
{
    drogon::app().addListener("0.0.0.0", 8080);
    drogon::app().setThreadNum(std::thread::hardware_concurrency() - 1);
    drogon::app().registerHandler("/weather", [](const drogon::HttpRequestPtr &request, drogonCallback &&callback)
    {
        std::cout << request->getBody() << std::endl;
        auto requestStatus = RequestHandler::isValid(request->getBody());
        std::cout << "Is valid request - " << std::boolalpha << (requestStatus == RequestHandler::RequestStatus::NO_ERROR) << std::endl;
        switch (requestStatus)
        {
        case RequestHandler::RequestStatus::NO_ERROR:
            callback(drogon::HttpResponse::newHttpResponse());
            break;
        case RequestHandler::RequestStatus::INVALID_JSON:
        {
            Json::Value root;
            root["msg"] = "body should be valid json";
            auto response = drogon::HttpResponse::newHttpJsonResponse(root);
            response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
            callback(response);
            break;
        }
        case RequestHandler::RequestStatus::HASH_CASH_NOT_FOUND:
        {
            Json::Value root;
            root["msg"] = "HashCash field not found";
            root["HashChallenge"] = "hello world";
            auto response = drogon::HttpResponse::newHttpJsonResponse(root);
            response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
            callback(response);
            break;
        }
        case RequestHandler::RequestStatus::INVALID_HASH_CASH:
        {
            Json::Value root;
            root["msg"] = "hash cash is not valid";
            root["HashChallenge"] = "hello world";
            auto response = drogon::HttpResponse::newHttpJsonResponse(root);
            response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
            callback(response);
            break;
        }
        }
    }, {drogon::Post});
    drogon::app().run();
    return 0;
}
