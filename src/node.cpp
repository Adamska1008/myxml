#include <type_traits>
#include "myxml/node.hpp"
#include "myxml/element.hpp"

namespace myxml
{
    void node::entity_encoding(bool flag)
    {
        this->config.entity_encoding = flag;
    }

    void node::platform_specific_newline(bool flag)
    {
        this->config.platform_specific_newline = flag;
    }

    std::shared_ptr<node> node::next_sibiling()
    {
        return this->_next;
    }

    std::shared_ptr<node> node::prev_sibiling()
    {
        return this->_prev;
    }

    std::shared_ptr<node> composite_node::last_child()
    {
        return this->lastChild;
    }

    const std::shared_ptr<node> &composite_node::last_child() const
    {
        return this->lastChild;
    }

    std::shared_ptr<node> composite_node::first_child()
    {
        return this->firstChild;
    }

    const std::shared_ptr<node> &composite_node::first_child() const
    {
        return this->firstChild;
    }

    std::shared_ptr<element_impl> composite_node::first_elem(std::string_view name)
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
        for (auto child = this->firstChild; child != nullptr; child = child->_next)
        {
            if (auto elem = child->as<element_impl>(); elem && elem->name == name)
            {
                this->nameToElemBuffer.emplace(name, elem);
                return elem;
            }
        }
        return nullptr;
    }

    std::shared_ptr<node> composite_node::push_front(const std::shared_ptr<node> &elem)
    {
        if (elem->_parent != nullptr)
        {
            elem->_parent->unlink(elem);
        }
        elem->_parent = this->shared_from_this()->as<composite_node>();
        if (this->firstChild == nullptr)
        {
            this->firstChild = elem;
            this->lastChild = elem;
        }
        else
        {
            this->firstChild->_prev = elem;
            elem->_next = this->firstChild;
            this->firstChild = elem;
        }
        return elem;
    }

    std::shared_ptr<node> composite_node::push_back(const std::shared_ptr<node> &elem)
    {
        if (elem->_parent != nullptr)
        {
            elem->_parent->unlink(elem);
        }
        elem->_parent = this->shared_from_this()->as<composite_node>();
        if (this->firstChild == nullptr)
        {
            this->firstChild = elem;
            this->lastChild = elem;
        }
        else
        {
            this->lastChild->_next = elem;
            elem->_prev = this->lastChild;
            this->lastChild = elem;
        }
        return elem;
    }

    void composite_node::unlink(const std::shared_ptr<node> &elem)
    {
        if (elem->_parent.get() != this)
        {
            return;
        }
        if (elem == this->firstChild)
        {
            this->firstChild = this->firstChild->_next;
        }
        if (elem == this->lastChild)
        {
            this->lastChild = this->lastChild->_prev;
        }
        if (elem->_prev != nullptr)
        {
            elem->_prev->_next = elem->_next;
        }
        if (elem->_next != nullptr)
        {
            elem->_next->_prev = elem->_prev;
        }
        elem->_next = nullptr;
        elem->_prev = nullptr;
        elem->_parent = nullptr;
    }

    void composite_node::entity_encoding(bool flag)
    {
        this->config.entity_encoding = flag;
        for (auto it = this->first_child(); it != nullptr; it = it->_next)
        {
            it->entity_encoding(flag);
        }
    }

    void composite_node::platform_specific_newline(bool flag)
    {
        this->config.platform_specific_newline = flag;
        for (auto it = this->first_child(); it != nullptr; it = it->_next)
        {
            it->platform_specific_newline(flag);
        }
    }
}
