#include "text.hpp"

namespace myxml
{
    Text::Text(std::string_view str)
        : inner(str) {}

    std::string Text::Export()
    {
        return this->inner;
    }
}
