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

    cdata::cdata(std::string &&str)
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
        os << "<![CDATA[" << _inner << "]]>\n";
    }
}
