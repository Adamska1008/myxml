#pragma once
#include <memory>
#include <optional>

namespace myxml
{
    // defined in element.hpp
    class Element;
    // defined in text.hpp
    class Text;

    enum class NodeType
    {
        Text,
        Element,
    };

    class Node
    {
    private:
        std::shared_ptr<Element> parent;
        std::shared_ptr<Node> prev;
        std::shared_ptr<Node> next;

    public:
        std::shared_ptr<Element> &Parent();
        std::shared_ptr<Node> &Next();
        std::shared_ptr<Node> &Prev();

        virtual NodeType Type() = 0;
        virtual bool isType(NodeType) = 0;
        virtual std::optional<std::shared_ptr<Element>> AsElement() = 0;
        virtual std::optional<std::shared_ptr<Text>> AsText() = 0;
    };
}
