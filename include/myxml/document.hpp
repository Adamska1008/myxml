#pragma once
#include <map>
#include "myxml/node.hpp"
#include "myxml/element.hpp"

// Declaration and Documant are both NOT Node
namespace myxml
{
    struct declaration : public exportable
    {
        std::string version;
        std::optional<std::string> encoding;
        std::optional<std::string> standalone;

        // return `std::nullopt` if declartion is in bad format
        // TODO: use exception to distinguish each of bad format
        static std::optional<declaration> from_attrs(std::map<std::string, std::string> attrs);

        /* Exportable */
        virtual std::string ExportRaw() const;
        virtual std::string ExportFormatted(int indentLevel = 0, int indentSize = 4) const;
    };

    class document : public exportable
    {
    private:
        declaration decl;
        std::shared_ptr<Element> root;

    public:
        /* Manipulate */
        void set_declaration(const declaration &);
        void set_root(std::shared_ptr<Element> root);

        /* Query */
        const declaration &get_declaration() const;
        declaration &get_declaration();
        const std::shared_ptr<Element> &get_root() const;
        std::shared_ptr<Element> get_root();
        std::shared_ptr<Element> first_elem(std::string_view);
        std::shared_ptr<Element> first_elem();
        std::shared_ptr<Text> first_text();

        /** Load */
        static document parse(std::string_view);
        static document load(std::string fileName);

        /* Exportable */
        virtual std::string ExportRaw() const;
        virtual std::string ExportFormatted(int indentLevel = 0, int indentSize = 4) const;
        virtual void entity_encoding(bool);
        virtual void platform_specific_newline(bool);
    };

    namespace util
    {
        bool is_valid_xml_version(std::string_view);
        bool is_valid_xml_encoding(std::string_view);
        bool is_valid_xml_standalone(std::string_view);
    }

    namespace literals
    {
        document operator""_doc(const char *, std::size_t);
    }
}
