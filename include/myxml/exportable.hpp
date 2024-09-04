#pragma once
#include <string>

namespace myxml
{
    class ExportConfig
    {
    public:
        bool EntityEncoding;
        bool PlatformSpecificNewline;

        ExportConfig();
    };

    class exportable
    {
    protected:
        ExportConfig config;

    public:
        virtual ~exportable() = default;
        virtual std::string ExportRaw() const = 0;
        virtual std::string ExportFormatted(int indentLevel = 0, int indentSize = 4) const = 0;
        virtual void entity_encoding(bool) {};
        virtual void platform_specific_newline(bool) {};
    };
}