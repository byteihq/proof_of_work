#include "RequestHandler.hpp"
#include "HashCash.hpp"
#include "Timer.hpp"
#include <jsoncpp/json/json.h>

ResponseStatus RequestHandler::isValid(std::string_view body, UserInfo &ui, const DiffParams &dp)
{
    Json::Value jsonBody;
    {
        Json::Reader reader;
        if (!reader.parse(body.data(), jsonBody))
            return ResponseStatus::INVALID_JSON;
    }
    if (ui.accessEnabled())
    {
        if (Timer::getTimestamp() - ui.lastRequest() >= 1000.f / dp.max_requests_per_second)
            return ResponseStatus::NO_ERROR;
        return ResponseStatus::TOO_MANY_REQUESTS;
    }
    return HashCash::isValid(jsonBody, ui);
}
