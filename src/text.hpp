#pragma once
#include <string>
#include "node.hpp"

namespace myxml
{
    class Text : public Node, public std::enable_shared_from_this<Text>
    {
    private:
        std::string inner;

    public:
        explicit Text(std::string_view str);
        std::string Export();

        virtual NodeType Type() override;
        virtual bool isType(NodeType) override;
        virtual std::optional<std::shared_ptr<Element>> AsElement() override;
        virtual std::optional<std::shared_ptr<Text>> AsText() override;
    };
}
