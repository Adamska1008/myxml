#include "element.hpp"

namespace myxml
{
    Element::Element(std::string_view name)
        : name(name), closingType(ClosingType::Closing) {}

    std::shared_ptr<Element> Element::New(std::string_view name)
    {
        return std::shared_ptr<Element>(new Element(name));
    }

    std::shared_ptr<Element> Element::FirstChild()
    {
        return this->firstChild;
    }

    std::shared_ptr<Element> Element::Child(std::string_view name)
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
        for (auto &elem = this->firstChild; elem != nullptr; elem = elem->next)
        {
            if (elem->name == name)
            {
                this->nameToElemBuffer.emplace(name, elem);
                return elem;
            }
        }
        return nullptr;
    }

    std::shared_ptr<Element> Element::LastChild()
    {
        return this->lastChild;
    }

    std::shared_ptr<Element> Element::NextSibiling()
    {
        return this->next;
    }

    std::shared_ptr<Element> Element::PrevSibiling()
    {
        return this->prev;
    }

    std::shared_ptr<Element> Element::Parent()
    {
        return this->parent;
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

    std::string_view Element::GetName()
    {
        return this->name;
    }

    std::shared_ptr<Element> Element::InsertAtFront(const std::shared_ptr<Element> &elem)
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

    std::shared_ptr<Element> Element::InsertAtFront(Element &&elem)
    {
        auto ptr = std::make_shared<Element>(elem);
        return this->InsertAtFront(ptr);
    }

    std::shared_ptr<Element> Element::InsertAtEnd(const std::shared_ptr<Element> &elem)
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

    std::shared_ptr<Element> Element::InsertAtEnd(Element &&elem)
    {
        auto ptr = std::make_shared<Element>(elem);
        return this->InsertAtEnd(ptr);
    }

    void Element::Unlink(const std::shared_ptr<Element> &elem)
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

    Element::ClosingType Element::GetClosingType()
    {
        return this->closingType;
    }

    std::optional<Text> Element::GetText()
    {
        return this->text;
    }

    void Element::SetClosingType(ClosingType type)
    {
        this->closingType = type;
    }

    void Element::SetText(Text text)
    {
        this->text = text;
    }
}