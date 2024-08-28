#pragma once
#include <string>
#include "node.hpp"

namespace myxml
{
    class Text : public Node, public std::enable_shared_from_this<Text>
    {
    private:
        std::string inner;
        bool encodeOnExport;

    public:
        explicit Text(std::string_view str);

        // may used in Export
        bool IsAllSpace() const;
        void SetEntityEncoding(bool);

        /* implement Node */
        virtual NodeType Type() override;
        virtual bool IsType(NodeType) override;
        virtual std::optional<std::shared_ptr<Element>> AsElement() override;
        virtual std::optional<std::shared_ptr<Text>> AsText() override;

        /* Implment Exportable*/
        virtual std::string ExportRaw() const override;
        virtual std::string ExportFormatted(int indentLevel = 0, int indentSize = 4) const override;
    };
}
