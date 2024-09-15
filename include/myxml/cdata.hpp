#pragma once
#include "myxml/node.hpp"
#include "myxml/printable.hpp"

namespace myxml
{
    class cdata_impl;

    class cdata : public printable
    {
        friend class element;

    private:
        std::shared_ptr<cdata_impl> _impl;

        cdata(std::shared_ptr<cdata_impl>);

    public:
        explicit cdata(std::string);
        explicit cdata(std::string_view);
        explicit cdata(const char *);

        virtual void entity_encoding(bool) override {}
        virtual void platform_specific_newline(bool) override {}
        virtual void print(std::ostream &) const override;
    };

    class cdata_impl : public node
    {
    public:
        std::string _inner;

        cdata_impl() {};
        explicit cdata_impl(std::string_view);
        explicit cdata_impl(std::string &&);
        virtual ~cdata_impl() = default;

        virtual void entity_encoding(bool) override {}
        virtual void platform_specific_newline(bool) override {}
        virtual void print(std::ostream &) const override;
    };
}