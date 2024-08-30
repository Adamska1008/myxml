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

    class Exportable
    {
    protected:
        ExportConfig config;

    public:
        virtual ~Exportable() = default;
        virtual std::string ExportRaw() const = 0;
        virtual std::string ExportFormatted(int indentLevel = 0, int indentSize = 4) const = 0;
        virtual void SetEntityEncoding(bool) {};
        virtual void SetPlatformSpecificNewline(bool) {};
    };
}