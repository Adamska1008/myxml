#include "text.hpp"

namespace myxml
{
    NodeType Text::Type()
    {
        return NodeType::Text;
    }

    bool Text::isType(NodeType type)
    {
        return type == NodeType::Text;
    }

    std::optional<std::shared_ptr<Element>> Text::AsElement()
    {
        return std::nullopt;
    }

    std::optional<std::shared_ptr<Text>> Text::AsText()
    {
        Text *test = dynamic_cast<Text *>(this);
        return std::dynamic_pointer_cast<Text>(this->shared_from_this());
    }

    Text::Text(std::string_view str)
        : inner(str) {}

    std::string Text::Export()
    {
        return this->inner;
    }
}
