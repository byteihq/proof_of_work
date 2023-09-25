#include "RequestHandler.hpp"
#include "HashCash.hpp"
#include <jsoncpp/json/json.h>

RequestHandler::RequestStatus RequestHandler::isValid(std::string_view body)
{
    Json::Value jsonBody;
    {
        Json::Reader reader;
        if (!reader.parse(body.data(), jsonBody))
            return RequestStatus::INVALID_JSON;
    }

    if (!jsonBody.isMember(HashCash::str()))
        return RequestStatus::HASH_CASH_NOT_FOUND;
    
    if (!jsonBody[HashCash::str()].isString())
        return RequestStatus::INVALID_HASH_CASH;

    if (!HashCash::isValid(jsonBody[HashCash::str()].asString()))
        return RequestStatus::INVALID_HASH_CASH;
    
    return RequestStatus::NO_ERROR;
}
