
#include <string_view>

class HashCash final
{
public:
    static bool isValid(std::string_view hashCash);

    static constexpr const char *str() noexcept
    {
        return "HashCash";
    }
};
