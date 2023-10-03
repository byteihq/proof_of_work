#include "RequestHandler.hpp"
#include "HashCash.hpp"
#include <jsoncpp/json/json.h>

ResponseStatus RequestHandler::isValid(std::string_view body, UserInfo &ui)
{
    Json::Value jsonBody;
    {
        Json::Reader reader;
        if (!reader.parse(body.data(), jsonBody))
            return ResponseStatus::INVALID_JSON;
    }
    if (ui.accessEnabled())
        return ResponseStatus::NO_ERROR;
    return HashCash::isValid(jsonBody, ui);
}
