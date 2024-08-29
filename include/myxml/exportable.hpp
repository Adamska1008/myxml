#pragma once
#include <string>

namespace myxml
{
    class Exportable
    {
    public:
        virtual ~Exportable() = default;
        virtual std::string ExportRaw() const = 0;
        virtual std::string ExportFormatted(int indentLevel = 0, int indentSize = 4) const = 0;
        virtual void SetEntityEncoding(bool) = 0;
    };
}