#pragma once
#include <memory>
#include <optional>
#include <map>

#include "myxml/exportable.hpp"

namespace myxml
{
    // defined in element.hpp
    class Element;
    // defined in text.hpp
    class Text;
    // defined below
    class CompositeNode;

    // Element, Text are Node.
    class Node : public std::enable_shared_from_this<Node>, public Exportable
    {
    public:
        virtual ~Node() = default;

        std::shared_ptr<CompositeNode> parent;
        std::shared_ptr<Node> prev;
        std::shared_ptr<Node> next;

        template <typename T>
        std::enable_if_t<std::is_base_of_v<Node, T>,
                         std::optional<std::shared_ptr<T>>>
        As();
    };

    // Element are Composite Node.
    class CompositeNode : public Node
    {
    private:
        std::shared_ptr<Node> firstChild;
        std::shared_ptr<Node> lastChild;
        std::map<std::string, std::weak_ptr<Element>, std::less<>> nameToElemBuffer;

    public:
        virtual ~CompositeNode() = default;

        /* Query */
        std::shared_ptr<Node> FirstChild();
        const std::shared_ptr<Node> &FirstChild() const;
        std::shared_ptr<Node> LastChild();
        const std::shared_ptr<Node> &LastChild() const;
        std::shared_ptr<Element> Elem(std::string_view name);

        /* Manipulate */
        std::shared_ptr<Node> InsertAtFront(const std::shared_ptr<Node> &);
        std::shared_ptr<Node> InsertAtEnd(const std::shared_ptr<Node> &);
        void Unlink(const std::shared_ptr<Node> &);
    };

    template <typename T>
    std::enable_if_t<std::is_base_of_v<Node, T>,
                     std::optional<std::shared_ptr<T>>>
    Node::As()
    {
        auto derivedPtr = std::dynamic_pointer_cast<T>(this->shared_from_this());
        return derivedPtr ? std::optional(derivedPtr) : std::nullopt;
    }
}
