#include "element.hpp"
#include "parser.cpp"

namespace myxml
{
    Element::Element(std::string_view name)
        : name(name) {}

    std::shared_ptr<Element> Element::New(std::string_view name)
    {
        return std::shared_ptr<Element>(new Element(name));
    }

    std::shared_ptr<Element> Element::New()
    {
        return std::shared_ptr<Element>(new Element());
    }

    std::shared_ptr<Element> Element::Parse(std::string_view buf)
    {
        return Parser(buf).ParseElement().value();
    }

    std::optional<std::string_view> Element::GetAttribute(std::string_view name)
    {
        if (auto attr = this->attributes.find(name); attr != this->attributes.end())
        {
            return attr->second;
        }
        else
        {
            return std::nullopt;
        }
    }

    std::string_view Element::GetName() const
    {
        return this->name;
    }

    void Element::SetName(std::string_view name)
    {
        this->name = name;
    }

    void Element::SetAttribute(std::string key, std::string value)
    {
        this->attributes.emplace(key, value);
    }

    void Element::ExtendAttributes(std::map<std::string, std::string> attris)
    {
        this->attributes.insert(attris.begin(), attris.end());
    }

    NodeType Element::Type()
    {
        return NodeType::Element;
    }

    bool Element::isType(NodeType type)
    {
        return type == NodeType::Element;
    }

    std::optional<std::shared_ptr<Element>> Element::AsElement()
    {
        return std::dynamic_pointer_cast<Element>(this->shared_from_this());
    }

    std::optional<std::shared_ptr<Text>> Element::AsText()
    {
        return std::nullopt;
    }

    std::string Element::ExportRaw() const
    {

        std::string builder = "<" + std::string(this->GetName());
        for (const auto &[key, value] : this->attributes)
        {
            builder += "" + key + "=\"" + value + "\"";
        }
        if (this->FirstChild() == nullptr)
        {
            builder += " />";
            return builder;
        }
        builder += ">";
        for (auto node = this->FirstChild(); node != nullptr; node = node->next)
        {
            builder += node->ExportRaw();
        }
        builder += "</" + std::string(this->GetName()) + ">";
        return builder;
    }

    std::string Element::ExportFormatted(int indentLevel, int indentSize) const
    {
        std::string indent(indentLevel * indentSize, ' ');
        std::string builder = indent + "<" + std::string(this->GetName());
        for (const auto &[key, value] : this->attributes)
        {
            builder += "" + key + "=\"" + value + "\"";
        }
        if (this->FirstChild() == nullptr)
        {
            builder += " />\n";
            return builder;
        }
        builder += ">\n";
        for (auto node = this->FirstChild(); node != nullptr; node = node->next)
        {
            builder += node->ExportFormatted(indentLevel + 1, indentSize);
        }
        builder += indent + "</" + std::string(this->GetName()) + ">\n";
        return builder;
    }
}