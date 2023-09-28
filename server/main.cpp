#include <thread>
#include <drogon/HttpAppFramework.h>
#include <RequestHandler.hpp>
#include <HashCash.hpp>
#include <Timer.hpp>
#include <Weather.hpp>

using namespace drogon;

using drogonCallback = std::function<void(const HttpResponsePtr &)>;

int main()
{
    HashCash::GetInstance()->Init("tcp://localhost:6379");
    Timer t;
    app().addListener("0.0.0.0", 8080);
    app().setThreadNum(std::thread::hardware_concurrency() - 1);
    app().registerHandler("/", [](const HttpRequestPtr &, drogonCallback &&callback)
    {
        auto response = HttpResponse::newHttpResponse();
        response->setBody("Weather App Online!");
        callback(response);
    }, {Get});
    app().registerHandler("/weather", [&t](const HttpRequestPtr &request, drogonCallback &&callback)
    {
        std::cout << request->getBody() << "\nIP: " << request->getLocalAddr().toIpPort() << std::endl;
        t.set();
        auto ip = request->getLocalAddr().toIp();
        auto requestStatus = RequestHandler::isValid(ip, request->getBody());
        std::cout << "Is valid request - " << std::boolalpha << (requestStatus == RequestHandler::Status::NO_ERROR) << std::endl;
        switch (requestStatus)
        {
        case RequestHandler::Status::NO_ERROR:
        {
            Json::Value root;
            Json::Value jsonBody;
            Json::Reader().parse(request->getBody().data(), jsonBody);
            root["data"] = Weather::get(jsonBody["city"].asString()).json();
            root["time_response_ms"] = static_cast<Json::Value::Int64>(t.ellapsed());

            auto response = HttpResponse::newHttpJsonResponse(root);
            response->setStatusCode(HttpStatusCode::k200OK);
            callback(response);
            break;
        }
        case RequestHandler::Status::INVALID_JSON:
        {
            Json::Value root;
            root["msg"] = "Body should be valid json.";
            root["time_response_ms"] = static_cast<Json::Value::Int64>(t.ellapsed());
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
            root["HashChallenge"] = HashCash::GetInstance()->createNewChallenge(ip, 5, "weather.com");
            root["time_response_ms"] = static_cast<Json::Value::Int64>(t.ellapsed());
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
