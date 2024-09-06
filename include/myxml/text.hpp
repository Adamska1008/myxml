#pragma once
#include <string>
#include "myxml/node.hpp"
#include "myxml/interface.hpp"

namespace myxml
{

    struct text_impl : public node
    {
        std::string inner;

        text_impl() = default;
        explicit text_impl(std::string_view str);

        virtual ~text_impl() = default;

        /* Implment Exportable*/
        virtual void print(std::ostream &) const override;
    };

    class text : public printable, public interface
    {
        friend class element;

    private:
        std::shared_ptr<text_impl> _impl;

        text(std::shared_ptr<text_impl>);

    public:
        text(const std::string &);
        text(std::string &&);

        std::string trimmed();
        text trim();

        /* Implement printable */
        virtual void print(std::ostream &) const override;
        virtual void entity_encoding(bool) override;
        virtual void platform_specific_newline(bool) override;

        /* Implement interface */
        virtual std::shared_ptr<node> impl() { return _impl; }
    };

    namespace util
    {
        const std::string_view platform_specific_newline();
    };
}