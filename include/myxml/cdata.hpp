#pragma once
#include "myxml/node.hpp"

namespace myxml
{
    class CData : public Node
    {
    private:
        std::string inner;

    public:
        explicit CData(std::string);

        virtual ~CData() = default;
        virtual std::string ExportRaw() const override;
        virtual std::string ExportFormatted(int indentLevel = 0, int indentSize = 4) const override;
        virtual void entity_encoding(bool) override;
    };
}