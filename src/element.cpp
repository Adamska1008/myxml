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

    std::shared_ptr<Node> Element::FirstChild()
    {
        return this->firstChild;
    }

    std::shared_ptr<Element> Element::Elem(std::string_view name)
    {
        if (auto buf = this->nameToElemBuffer.find(name); buf != this->nameToElemBuffer.end())
        {
            std::weak_ptr<Element> ptr = buf->second;
            if (auto child = ptr.lock(); child != nullptr)
            {
                return child;
            }
            else
            {
                this->nameToElemBuffer.erase(buf);
            }
        }
        for (auto child = this->firstChild; child != nullptr; child = child->next)
        {
            if (auto elem = child->AsElement(); elem && (*elem)->name == name)
            {
                this->nameToElemBuffer.emplace(name, *elem);
                return *elem;
            }
        }
        return nullptr;
    }

    std::shared_ptr<Node> Element::LastChild()
    {
        return this->lastChild;
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

    std::shared_ptr<Node> Element::InsertAtFront(const std::shared_ptr<Node> &elem)
    {
        if (elem->parent != nullptr)
        {
            elem->parent->Unlink(elem);
        }
        elem->parent = this->shared_from_this();
        if (this->firstChild == nullptr)
        {
            this->firstChild = elem;
            this->lastChild = elem;
        }
        else
        {
            this->firstChild->prev = elem;
            elem->next = this->firstChild;
            this->firstChild = elem;
        }
        return elem;
    }

    std::shared_ptr<Node> Element::InsertAtEnd(const std::shared_ptr<Node> &elem)
    {
        if (elem->parent != nullptr)
        {
            elem->parent->Unlink(elem);
        }
        elem->parent = this->shared_from_this();
        if (this->firstChild == nullptr)
        {
            this->firstChild = elem;
            this->lastChild = elem;
        }
        else
        {
            this->lastChild->next = elem;
            elem->prev = this->lastChild;
            this->lastChild = elem;
        }
        return elem;
    }

    void Element::Unlink(const std::shared_ptr<Node> &elem)
    {
        if (elem->parent.get() != this)
        {
            return;
        }
        if (elem == this->firstChild)
        {
            this->firstChild = this->firstChild->next;
        }
        if (elem == this->lastChild)
        {
            this->lastChild = this->lastChild->prev;
        }
        if (elem->prev != nullptr)
        {
            elem->prev->next = elem->next;
        }
        if (elem->next != nullptr)
        {
            elem->next->prev = elem->prev;
        }
        elem->next = nullptr;
        elem->prev = nullptr;
        elem->parent = nullptr;
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
        if (this->firstChild == nullptr)
        {
            builder += " />";
            return builder;
        }
        builder += ">";
        for (auto node = this->firstChild; node != nullptr; node = node->next)
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
        if (this->firstChild == nullptr)
        {
            builder += " />\n";
            return builder;
        }
        builder += ">\n";
        for (auto node = this->firstChild; node != nullptr; node = node->next)
        {
            builder += node->ExportFormatted(indentLevel + 1, indentSize);
        }
        builder += indent + "</" + std::string(this->GetName()) + ">\n";
        return builder;
    }
}