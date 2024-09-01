#pragma once
#include <memory>
#include <optional>
#include <string>
#include <map>

#include "myxml/text.hpp"
#include "myxml/exportable.hpp"

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
        std::string name;
        std::map<std::string, std::string, std::less<>> attributes;

        /* Set initializer as private to avoid using Element without share_ptr*/
        explicit Element(std::string_view name);
        Element() = default;

    public:
        virtual ~Element() = default;

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
        std::string &operator[](const std::string &);

        /* Implement Exportable */
        virtual std::string ExportRaw() const override;
        virtual std::string ExportFormatted(int indentLevel = 0, int indentSize = 4) const override;
    };

    namespace literals
    {
        // Custom String Literal for Element
        std::shared_ptr<Element> operator""_elem(const char *, std::size_t);
    }
}
