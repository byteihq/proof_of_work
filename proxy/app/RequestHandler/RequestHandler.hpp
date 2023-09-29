#pragma once

#include <string_view>
#include <TypeDefs.hpp>

class RequestHandler final
{
public:
    static ResponseStatus isValid(std::string_view sourceIp, std::string_view body);
};
