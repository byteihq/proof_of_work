
#include <string_view>

class RequestHandler final
{
public:
    enum class RequestStatus : uint8_t
    {
        NO_ERROR,
        INVALID_JSON,
        HASH_CASH_NOT_FOUND,
        INVALID_HASH_CASH
    };

public:
    static RequestStatus isValid(std::string_view body);
};
