#pragma once
#include "myxml/node.hpp"
#include "myxml/printable.hpp"

namespace myxml
{
    class cdata_impl;

    class cdata : public printable
    {
    private:
        std::shared_ptr<cdata_impl> _impl;

        cdata(std::shared_ptr<cdata_impl>);

    public:
        cdata(std::string &&);
        cdata(std::string_view);

        virtual void entity_encoding(bool) override {}
        virtual void platform_specific_newline(bool) override {}
        virtual void print(std::ostream &) const override;
    };

    class cdata_impl : public Node
    {
    public:
        std::string inner;

        cdata_impl() {};
        explicit cdata_impl(std::string_view);
        explicit cdata_impl(std::string &&);
        virtual ~cdata_impl() = default;

        virtual void entity_encoding(bool) override {}
        virtual void platform_specific_newline(bool) override {}
        virtual void print(std::ostream &) const override;
    };
}