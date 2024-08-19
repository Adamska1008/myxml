#pragma once
#include <memory>
#include <optional>
#include <string>
#include <map>

#include "text.hpp"


namespace myxml
{
    class Element : public std::enable_shared_from_this<Element>, public Node
    {
    public:
        enum class ClosingType
        {
            Closed,
            Closing,
        };

    private:
        // list node
        // std::shared_ptr<Element> parent;
        // std::shared_ptr<Node> next;
        // std::shared_ptr<Node> prev;

        // element
        std::shared_ptr<Node> firstChild;
        std::shared_ptr<Node> lastChild;
        std::string name;
        std::map<std::string, std::string, std::less<>> attributes;
        std::map<std::string, std::weak_ptr<Element>, std::less<>> nameToElemBuffer;

        // Initializer
        Element(std::string_view name);
        Element() = default;

    public:
        // Builder
        // Wraps creating shared_ptr
        static std::shared_ptr<Element> New(std::string_view name);
        static std::shared_ptr<Element> New();
        static std::shared_ptr<Element> Parse(std::string_view buf);

        // Query
        std::shared_ptr<Node> FirstChild();
        std::shared_ptr<Node> LastChild();
        std::shared_ptr<Element> Elem(std::string_view name);
        std::optional<std::string_view> GetAttribute(std::string_view name);
        std::string_view GetName();

        // Manipulate
        std::shared_ptr<Node> InsertAtFront(const std::shared_ptr<Node> &);
        std::shared_ptr<Node> InsertAtEnd(const std::shared_ptr<Node> &);
        void Unlink(const std::shared_ptr<Node> &);
        void SetName(std::string_view);

        // implement node
        virtual NodeType Type() override;
        virtual bool isType(NodeType) override;
        virtual std::optional<std::shared_ptr<Element>> AsElement() override;
        virtual std::optional<std::shared_ptr<Text>> AsText() override;
    };
}
