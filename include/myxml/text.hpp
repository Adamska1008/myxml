#pragma once
#include <string>
#include "myxml/node.hpp"

namespace myxml
{
    class Text : public Node
    {
    private:
        std::string inner;

    public:
        explicit Text(std::string_view str);

        virtual ~Text() = default; 

        // may used in Export
        bool IsAllSpace() const;

        /* Implment Exportable*/
        virtual std::string ExportRaw() const override;
        virtual std::string ExportFormatted(int indentLevel = 0, int indentSize = 4) const override;
    };

    namespace util
    {
        const char *const platformSpecificNewline();
    };
}