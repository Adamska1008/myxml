#include "node.hpp"
#include "element.hpp"

namespace myxml
{

    std::shared_ptr<Node> CompositeNode::LastChild()
    {
        return this->lastChild;
    }

    const std::shared_ptr<Node> &CompositeNode::LastChild() const
    {
        return this->lastChild;
    }

    std::shared_ptr<Node> CompositeNode::FirstChild()
    {
        return this->firstChild;
    }

    const std::shared_ptr<Node> &CompositeNode::FirstChild() const
    {
        return this->firstChild;
    }

    std::shared_ptr<Element> CompositeNode::Elem(std::string_view name)
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
            if (auto elem = child->AsElement(); elem && (*elem)->GetName() == name)
            {
                this->nameToElemBuffer.emplace(name, *elem);
                return *elem;
            }
        }
        return nullptr;
    }

    std::shared_ptr<Node> CompositeNode::InsertAtFront(const std::shared_ptr<Node> &elem)
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

    std::shared_ptr<Node> CompositeNode::InsertAtEnd(const std::shared_ptr<Node> &elem)
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

    void CompositeNode::Unlink(const std::shared_ptr<Node> &elem)
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

}
