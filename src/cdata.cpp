#include <fmt/core.h>
#include "myxml/cdata.hpp"

namespace myxml
{
    CData::CData(std::string str)
        : inner(str)
    {
    }

    std::string CData::ExportRaw() const
    {
        return fmt::format("<![CDATA[{}]]>\n", this->inner);
    }

    std::string CData::ExportFormatted(int indentLevel, int indentSize) const
    {
        return std::string(indentLevel * indentSize, ' ') + this->ExportRaw();
    }

    void CData::SetEntityEncoding(bool)
    { // do nothing
    }
}
