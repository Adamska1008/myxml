#pragma once
#include <memory>
#include "myxml/node.hpp"

namespace myxml
{
    // contains an _impl
    // e.g. element, text, cdata
    class interface
    {
    public:
        virtual std::shared_ptr<node> impl() = 0;
    };
}