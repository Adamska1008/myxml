#pragma once
#include <string>

namespace myxml
{
    class Text
    {
    private:
        std::string inner;

    public:
        explicit Text(std::string_view str);
        std::string Export();
    };
}
