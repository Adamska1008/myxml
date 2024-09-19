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
        virtual void print(std::ostream &os) const;
    };

    class document : public printable
    {
    private:
        std::optional<declaration> _decl;
        element _root;

    public:
        /* Manipulate */
        void set_declaration(const declaration &);
        void set_root(std::shared_ptr<element_impl> root);

        /* Query */
        std::optional<declaration> get_declaration();
        element &root();

        /* Forwarding element method*/
        std::optional<element> first_elem(std::string_view name) { return _root.first_elem(name); }
        std::optional<element> first_elem() { return _root.first_elem(); }
        std::optional<text> first_text() { return _root.first_text(); }
        std::optional<cdata> first_cdata() { return _root.first_cdata(); }
        std::optional<element> parent() { return _root.parent(); }
        template <typename T, typename SFINAE = std::enable_if_t<std::is_base_of_v<interface, T>>>
        void push_front(T child) { _root.push_front(child); }
        template <typename T, typename SFINAE = std::enable_if_t<std::is_base_of_v<interface, T>>>
        void push_back(T child) { _root.push_back(child); }
        void pop_front() { _root.pop_front(); }
        void pop_back() { _root.pop_back(); }
        template <typename T, typename SFINAE = std::enable_if_t<std::is_base_of_v<interface, T>>>
        void remove(T child) { _root.remove(child); }
        void remove_first_element(std::string name) { return _root.remove_first_element(name); }

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
