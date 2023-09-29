#include <drogon/HttpAppFramework.h>
#include <Api.hpp>

using namespace drogon;
using drogonCallback = std::function<void(const HttpResponsePtr &)>;

int main()
{
    app().addListener("0.0.0.0", 8081);
    app().registerHandler("/", [](const HttpRequestPtr &, drogonCallback &&callback)
    {
        auto response = HttpResponse::newHttpResponse();
        response->setBody("Weather App Online!");
        callback(response);
    }, {Get});
    app().registerHandler("/weather", [](const HttpRequestPtr &request, drogonCallback &&callback)
    {
        auto apiRequest = Api::tryParseRequest(request->getBody());
        HttpResponsePtr response = nullptr;
        if (!apiRequest)
        {
            response = HttpResponse::newHttpResponse();
            response->setBody("Invalid request");
            response->setStatusCode(HttpStatusCode::k400BadRequest);
        }
        else
        {
            Json::Value root;
            root["data"] = Api::getResponse(apiRequest.value()).json();
            response = HttpResponse::newHttpJsonResponse(root);        
            response->setStatusCode(HttpStatusCode::k200OK);
        }
        callback(response);
    }, {Post});

    app().run();

    return 0;
}
