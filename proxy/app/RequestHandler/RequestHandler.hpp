#pragma once

#include <string_view>
#include <TypeDefs.hpp>
#include <UserInfo.hpp>

class RequestHandler final
{
public:
    static ResponseStatus isValid(std::string_view body, UserInfo &ui);
};
