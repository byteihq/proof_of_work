#include "RequestHandler.hpp"
#include "HashCash.hpp"
#include <jsoncpp/json/json.h>

RequestHandler::Status RequestHandler::isValid(std::string_view body)
{
    Json::Value jsonBody;
    {
        Json::Reader reader;
        if (!reader.parse(body.data(), jsonBody))
            return Status::INVALID_JSON;
    }

    if (!jsonBody.isMember(HashCash::str()))
        return Status::HASH_CASH_NOT_FOUND;
    
    if (!jsonBody[HashCash::str()].isString())
        return Status::INVALID_HASH_CASH;

    if (!HashCash::isValid(jsonBody[HashCash::str()].asString()))
        return Status::INVALID_HASH_CASH;
    
    return Status::NO_ERROR;
}
