#include "document.hpp"

namespace myxml
{
    NodeType Declaration::Type()
    {
        return NodeType::Declaration;
    }

    bool Declaration::isType(NodeType type)
    {
        return type == NodeType::Declaration;
    }

    std::optional<std::shared_ptr<Element>> Declaration::AsElement()
    {
        return std::nullopt;
    }

    std::optional<std::shared_ptr<Text>> Declaration::AsText()
    {
        return std::nullopt;
    }

    std::string Declaration::ExportRaw() const
    {
        return "<?xml version=\"" + this->version + "\" encoding=\"" + this->encoding + "\"?>";
    }

    std::string Declaration::ExportFormatted(int indentLevel, int indentSize) const
    {
        return std::string(indentLevel * indentSize, ' ') + this->ExportRaw();
    }
}
