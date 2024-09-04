#include <sstream>
#include "myxml/printable.hpp"

namespace myxml
{
    print_config::print_config()
        : entity_encoding(true),
          platform_specific_newline(false),
          fconfig(std::nullopt)
    {
    }

    std::ostream &operator<<(std::ostream &os, const printable &obj)
    {
        obj.print(os);
        return os;
    }

    void printable::entity_encoding(bool flag)
    {
        this->config.entity_encoding = flag;
    }

    void printable::platform_specific_newline(bool flag)
    {
        this->config.platform_specific_newline = flag;
    }

    std::string printable::str()
    {
        std::stringstream sstream;
        sstream << (*this);
        return sstream.str();
    }
}
