#pragma once
#include <map>
#include "node.hpp"

// Declaration and Documant are both NOT Node
namespace myxml
{
    struct Declaration
    {
        std::string version;
        std::optional<std::string> encoding;
        std::optional<std::string> standalone;

        // return `std::nullopt` if declartion is in bad format
        // TODO: use exception to distinguish each of bad format
        static std::optional<Declaration> BuildFromAttrs(std::map<std::string, std::string> attrs);
    };

    class Document
    {
    private:
        Declaration declaration;
        std::shared_ptr<Element> root;

    public:
        /* Manipulate */
        void SetDeclaration(const Declaration &);
        const Declaration &GetDeclartion() const;
    };

    namespace util
    {
        bool isValidXmlVersion(std::string_view);
        bool isValidXmlEncoding(std::string_view);
        bool isValidXmlStandalone(std::string_view);
    }
}
