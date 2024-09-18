#include <ostream>
#include <fmt/core.h>
#include "myxml/cdata.hpp"

namespace myxml
{
    cdata::cdata(std::shared_ptr<cdata_impl> impl)
        : _impl(impl)
    {
    }

    cdata::cdata(std::string_view str)
        : _impl(std::make_shared<cdata_impl>(str))
    {
    }

    cdata::cdata(const char *str)
        : _impl(std::make_shared<cdata_impl>(std::string_view(str)))
    {
    }

    cdata::cdata(std::string str)
        : _impl(std::make_shared<cdata_impl>(str))
    {
    }

    void cdata::print(std::ostream &os) const
    {
        _impl->print(os);
    }

    cdata_impl::cdata_impl(std::string_view str)
        : _inner(str)
    {
    }

    cdata_impl::cdata_impl(std::string &&str)
        : _inner(str)
    {
    }

    void cdata_impl::print(std::ostream &os) const
    {
        if (auto f = std::get_if<formatted>(&_print_config.style))
        {
            // TODO: implement it
        }
        else
        {
            os << "<![CDATA[" << _inner << "]]>";
            return;
        }
    }
}
