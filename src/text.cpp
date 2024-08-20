#include <algorithm>
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

    bool Text::isAllSpace() const
    {
        return std::all_of(this->inner.begin(), this->inner.end(), isspace);
    }

    std::string Text::ExportRaw() const
    {
        return this->inner;
    }

    std::string Text::ExportFormatted(int indentLevel, int indentSize) const
    {
        // TODO: better implementation
        return std::string(indentLevel * indentSize, ' ') + this->inner + '\n';
    }

}
