#pragma once
#include <map>
#include "myxml/node.hpp"
#include "myxml/element.hpp"

// Declaration and Documant are both NOT Node
namespace myxml
{
    struct Declaration : public Exportable
    {
        std::string version;
        std::optional<std::string> encoding;
        std::optional<std::string> standalone;

        // return `std::nullopt` if declartion is in bad format
        // TODO: use exception to distinguish each of bad format
        static std::optional<Declaration> BuildFromAttrs(std::map<std::string, std::string> attrs);

        /* Exportable */
        virtual std::string ExportRaw() const;
        virtual std::string ExportFormatted(int indentLevel = 0, int indentSize = 4) const;
    };

    class Document : public Exportable
    {
    private:
        Declaration declaration;
        std::shared_ptr<Element> root;

    public:
        /* Manipulate */
        void SetDeclaration(const Declaration &);
        void SetRoot(std::shared_ptr<Element> root);

        /* Query */
        const Declaration &GetDeclartion() const;
        Declaration &GetDeclartion();
        const std::shared_ptr<Element> &GetRoot() const;
        std::shared_ptr<Element> GetRoot();
        std::shared_ptr<Element> Elem(std::string_view);
        std::shared_ptr<Element> FirstElem();
        std::shared_ptr<Text> FirstText();

        /** Load */
        static Document Parse(std::string);
        static Document ParseFile(std::string fileName);

        /* Exportable */
        virtual std::string ExportRaw() const;
        virtual std::string ExportFormatted(int indentLevel = 0, int indentSize = 4) const;
        virtual void SetEntityEncoding(bool);
        virtual void SetPlatformSpecificNewline(bool);
    };

    namespace util
    {
        bool isValidXmlVersion(std::string_view);
        bool isValidXmlEncoding(std::string_view);
        bool isValidXmlStandalone(std::string_view);
    }
}
