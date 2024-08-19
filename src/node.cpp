#include "node.hpp"

namespace myxml
{
    std::shared_ptr<Element> Node::Parent()
    {
        return this->parent;
    }

    std::shared_ptr<Node> Node::Next()
    {
        return this->next;
    }

    std::shared_ptr<Node> Node::Prev()
    {
        return this->prev;
    }
}
