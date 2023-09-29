#include "RequestHandler.hpp"
#include "HashCash.hpp"
#include <jsoncpp/json/json.h>

ResponseStatus RequestHandler::isValid(std::string_view sourceIp, std::string_view body)
{
    Json::Value jsonBody;
    {
        Json::Reader reader;
        if (!reader.parse(body.data(), jsonBody))
            return ResponseStatus::INVALID_JSON;
    }

    return HashCash::GetInstance()->isValid(sourceIp, jsonBody);
}
