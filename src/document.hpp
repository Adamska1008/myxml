#pragma once
#include "node.hpp"

namespace myxml
{
    class Declaration : public Node
    {
    private:
        std::string version;
        std::string encoding;

    public:
        /* Implement Node*/
        virtual NodeType Type();
        virtual bool isType(NodeType);
        virtual std::optional<std::shared_ptr<Element>> AsElement();
        virtual std::optional<std::shared_ptr<Text>> AsText();

        /* Implement Exportable */
        virtual std::string ExportRaw() const;
        virtual std::string ExportFormatted(int indentLevel = 0, int indentSize = 4) const;
    };

    class Document : public Node
    {
    private:
        Declaration declaration;
        std::shared_ptr<Element> root;

    public:
        
    };
}
