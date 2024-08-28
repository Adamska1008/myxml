#pragma once
#include <memory>
#include <optional>
#include <string>
#include <map>

#include "text.hpp"
#include "exportable.hpp"

namespace myxml
{
    class Element : public CompositeNode // public std::enable_shared_from_this<Element>, public Node
    {
    public:
        enum class ClosingType
        {
            Closed,
            Closing,
        };

    private:
        // std::shared_ptr<Node> firstChild;
        // std::shared_ptr<Node> lastChild;
        std::string name;
        std::map<std::string, std::string, std::less<>> attributes;
        // std::map<std::string, std::weak_ptr<Element>, std::less<>> nameToElemBuffer;

        /* Set nitializer as private to avoid using Element without share_ptr*/
        Element(std::string_view name);
        Element() = default;

    public:
        /* Builder */
        // Wraps creating shared_ptr
        static std::shared_ptr<Element> New(std::string_view name);
        static std::shared_ptr<Element> New();
        static std::shared_ptr<Element> Parse(std::string_view buf);

        /* Query */
        std::optional<std::string_view> GetAttribute(std::string_view name);
        std::string_view GetName() const;

        /* Manipulate */
        void SetName(std::string_view);
        void SetAttribute(std::string key, std::string value);
        void ExtendAttributes(std::map<std::string, std::string>);

        /* Implement Node */
        virtual NodeType Type() override;
        virtual bool IsType(NodeType) override;
        virtual std::optional<std::shared_ptr<Element>> AsElement() override;
        virtual std::optional<std::shared_ptr<Text>> AsText() override;

        /* Implement Exportable */
        virtual std::string ExportRaw() const override;
        virtual std::string ExportFormatted(int indentLevel = 0, int indentSize = 4) const override;
    };
}
