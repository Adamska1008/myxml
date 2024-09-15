#pragma once
#include <map>
#include "myxml/node.hpp"
#include "myxml/element.hpp"

// Declaration and Documant are both NOT Node
namespace myxml
{
    struct declaration : public printable
    {
        std::string version;
        std::optional<std::string> encoding;
        std::optional<std::string> standalone;

        // return `std::nullopt` if declartion is in bad format
        // TODO: use exception to distinguish each of bad format
        static std::optional<declaration> from_attrs(std::map<std::string, std::string> attrs);

        /* Printable */
        virtual void entity_encoding(bool) {};
        virtual void platform_specific_newline(bool) {};
        virtual void print(std::ostream &os) const {}
    };

    class document : public printable
    {
    private:
        declaration _decl;
        element _root;

    public:
        /* Manipulate */
        void set_declaration(const declaration &);
        void set_root(std::shared_ptr<element_impl> root);

        /* Query */
        declaration &get_declaration();
        element &root();
        std::optional<element> first_elem(std::string_view);
        std::optional<element> first_elem();
        std::optional<text> first_text();

        /** Load */
        static document parse(std::string_view);
        static document load(std::string fileName);

        /* Implement printable */
        virtual void print(std::ostream &os) const override;
        virtual void entity_encoding(bool) override;
        virtual void platform_specific_newline(bool) override;
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
