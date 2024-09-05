#pragma once
#include <string>
#include "myxml/node.hpp"

namespace myxml
{

    class text : public printable
    {
        friend class element;

    private:
        std::shared_ptr<text_impl> _impl;

        text(std::shared_ptr<text_impl>);

    public:
        text(std::string_view);
        text(std::string &&);

        /* Implement printable */
        virtual void print(std::ostream &) const override;
        virtual void entity_encoding(bool) override;
        virtual void platform_specific_newline(bool) override;
    };

    struct text_impl : public node
    {
        std::string inner;

        text_impl() = default;
        explicit text_impl(std::string_view str);

        virtual ~text_impl() = default;

        /* Implment Exportable*/
        virtual void print(std::ostream &) const override;
    };

    namespace util
    {
        const std::string_view platform_specific_newline();
    };
}