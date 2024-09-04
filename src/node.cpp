#include <type_traits>
#include "myxml/node.hpp"
#include "myxml/element.hpp"

namespace myxml
{
    void Node::entity_encoding(bool flag)
    {
        this->config.entity_encoding = flag;
    }

    void Node::platform_specific_newline(bool flag)
    {
        this->config.platform_specific_newline = flag;
    }

    std::shared_ptr<Node> Node::NextSibiling()
    {
        return this->next;
    }

    std::shared_ptr<Node> Node::PrevSibiling()
    {
        return this->prev;
    }

    std::shared_ptr<element_impl> Node::NextElem()
    {
        return this->Next<element_impl>();
    }

    std::shared_ptr<element_impl> Node::PrevElem()
    {
        return this->Prev<element_impl>();
    }

    std::shared_ptr<text_impl> Node::NextText()
    {
        return this->Next<text_impl>();
    }

    std::shared_ptr<text_impl> Node::PrevText()
    {
        return this->Prev<text_impl>();
    }

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

    std::shared_ptr<element_impl> CompositeNode::FirstElem()
    {
        return this->First<element_impl>();
    }

    std::shared_ptr<text_impl> CompositeNode::FirstText()
    {
        return this->First<text_impl>();
    }

    std::shared_ptr<element_impl> CompositeNode::Elem(std::string_view name)
    {
        if (auto buf = this->nameToElemBuffer.find(name); buf != this->nameToElemBuffer.end())
        {
            std::weak_ptr<element_impl> ptr = buf->second;
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
            if (auto elem = child->As<element_impl>(); elem && elem->name == name)
            {
                this->nameToElemBuffer.emplace(name, elem);
                return elem;
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
        elem->parent = this->shared_from_this()->As<CompositeNode>();
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
        elem->parent = this->shared_from_this()->As<CompositeNode>();
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

    void CompositeNode::entity_encoding(bool flag)
    {
        this->config.entity_encoding = flag;
        for (auto it = this->FirstChild(); it != nullptr; it = it->next)
        {
            it->entity_encoding(flag);
        }
    }

    void CompositeNode::platform_specific_newline(bool flag)
    {
        this->config.platform_specific_newline = flag;
        for (auto it = this->FirstChild(); it != nullptr; it = it->next)
        {
            it->platform_specific_newline(flag);
        }
    }
}
