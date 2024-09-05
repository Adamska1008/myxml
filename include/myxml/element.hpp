#pragma once
#include <memory>
#include <optional>
#include <string>
#include <map>

#include "myxml/text.hpp"
#include "myxml/cdata.hpp"
#include "myxml/printable.hpp"

namespace myxml
{

    enum class ClosingType
    {
        Closed,
        Closing,
    };

    class element;

    namespace literals
    {
        // Custom String Literal for Element
        element operator""_elem(const char *, std::size_t);
    }

    class element : public printable
    {

        friend element literals::operator""_elem(const char *, std::size_t);
        friend class document;

    private:
        std::shared_ptr<element_impl> _impl;
        element(std::shared_ptr<element_impl>);

    public:
        element() : _impl(nullptr) {}
        explicit element(std::string_view);

        static element parse(std::string_view);
        static element load(std::string_view);
        std::string &operator[](const std::string &);
        std::string_view name();

        element first_elem();
        element first_elem(std::string_view);
        text first_text();
        cdata first_cdata();

        /* Implement printable */
        virtual void print(std::ostream &) const override;
        virtual void entity_encoding(bool) override;
        virtual void platform_specific_newline(bool) override;
    };

    struct element_impl : public composite_node // public std::enable_shared_from_this<Element>, public Node
    {
    public:
        std::string _name;
        std::map<std::string, std::string, std::less<>> _attributes;

        /* Set initializer as private to avoid using Element without share_ptr*/

        virtual ~element_impl() = default;
        explicit element_impl(std::string_view name);
        element_impl() = default;
        /* Builder */
        // Wraps creating shared_ptr
        static std::shared_ptr<element_impl> _new(std::string_view name);
        static std::shared_ptr<element_impl> _new();
        static std::shared_ptr<element_impl> parse(std::string_view buf);
        static std::shared_ptr<element_impl> load(std::string_view path);

        /* Manipulate */
        void extend_attributes(std::map<std::string, std::string>);
        std::string &operator[](const std::string &);

        /* Implement Exportable */
        virtual void print(std::ostream &) const override;
    };
}
