#pragma once
#include <memory>
#include <optional>
#include <map>

#include "exportable.hpp"

namespace myxml
{
    // defined in element.hpp
    class Element;
    // defined in text.hpp
    class Text;
    // defined below
    class CompositeNode;

    enum class NodeType
    {
        Text,
        Element,
        Declaration,
    };

    // Document, Element, Text, Declaration are Node.
    class Node : public Exportable
    {
    public:
        virtual ~Node() = default;

        std::shared_ptr<CompositeNode> parent;
        std::shared_ptr<Node> prev;
        std::shared_ptr<Node> next;

        virtual NodeType Type() = 0;
        virtual bool isType(NodeType) = 0;
        virtual std::optional<std::shared_ptr<Element>> AsElement() = 0;
        virtual std::optional<std::shared_ptr<Text>> AsText() = 0;
    };

    // Doucment and Element are Composite Node.
    class CompositeNode : public Node, public std::enable_shared_from_this<CompositeNode>
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
}
