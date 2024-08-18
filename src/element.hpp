#pragma once
#include <memory>
#include <optional>
#include <string>
#include <map>

#include "text.hpp"

namespace myxml
{
    class Element : public std::enable_shared_from_this<Element>
    {
    public:
        enum class ClosingType
        {
            Closed,
            Closing,
        };

    private:
        // list node
        std::shared_ptr<Element> parent;
        std::shared_ptr<Element> next;
        std::shared_ptr<Element> prev;
        std::shared_ptr<Element> firstChild;
        std::shared_ptr<Element> lastChild;
        // element
        std::string name;
        std::map<std::string, std::string, std::less<>> attributes;
        std::map<std::string, std::weak_ptr<Element>, std::less<>> nameToElemBuffer;
        std::optional<Text> text;
        ClosingType closingType;

        Element(std::string_view name);

    public:
        // Initializer
        Element() = delete;

        // Builder
        // Wraps creating shared_ptr
        static std::shared_ptr<Element> New(std::string_view name);

        // Query
        std::shared_ptr<Element> FirstChild();
        std::shared_ptr<Element> Child(std::string_view name);
        std::shared_ptr<Element> LastChild();
        std::shared_ptr<Element> NextSibiling();
        std::shared_ptr<Element> PrevSibiling();
        std::shared_ptr<Element> Parent();
        std::optional<std::string_view> GetAttribute(std::string_view name);
        std::string_view GetName();
        ClosingType GetClosingType();

        // Manipulate
        std::shared_ptr<Element> InsertAtFront(const std::shared_ptr<Element> &);
        std::shared_ptr<Element> InsertAtFront(Element &&);
        std::shared_ptr<Element> InsertAtEnd(const std::shared_ptr<Element> &);
        std::shared_ptr<Element> InsertAtEnd(Element &&);
        void Unlink(const std::shared_ptr<Element> &);
        void SetClosingType(ClosingType);
        void SetText(Text);
    };
}
