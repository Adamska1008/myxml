#include <set>
#include "document.hpp"
#include "parser.hpp"

namespace myxml
{
    void Document::SetDeclaration(const Declaration &declaration)
    {
        this->declaration = declaration;
    }

    void Document::SetRoot(std::shared_ptr<Element> root)
    {
        this->root = root;
    }

    const Declaration &Document::GetDeclartion() const
    {
        return this->declaration;
    }

    Declaration &Document::GetDeclartion()
    {
        return this->declaration;
    }

    const std::shared_ptr<Element> &Document::GetRoot() const
    {
        return this->root;
    }

    std::shared_ptr<Element> Document::GetRoot()
    {
        return this->root;
    }

    std::optional<Document> Document::Parse(std::string input)
    {
        return Parser(input).ParseDocument();
    }

    std::optional<Declaration> Declaration::BuildFromAttrs(std::map<std::string, std::string> attrs)
    {
        if (!attrs.count("version") || !util::isValidXmlVersion(attrs["version"]))
        {
            return std::nullopt;
        }
        Declaration declaration;
        declaration.version = attrs["version"];
        if (attrs.count("encoding"))
        {
            auto encoding = attrs["encoding"];
            if (!util::isValidXmlEncoding(encoding))
            {
                return std::nullopt;
            }
            declaration.encoding = encoding;
        }
        if (attrs.count("standalone"))
        {
            auto standalone = attrs["standalone"];
            if (!util::isValidXmlStandalone(standalone))
            {
                return std::nullopt;
            }
            declaration.standalone = standalone;
        }
        return declaration;
    }

    namespace util
    {
        bool isValidXmlVersion(std::string_view version)
        {
            return version == "1.0" || version == "1.1";
        }

        bool isValidXmlEncoding(std::string_view encoding)
        {
            // FIXME: not cover all valid encoding
            static std::set<std::string, std::less<>> valid{
                "UTF-8",
                "UTF-16",
                "UTF-32",
                "GBK",
            };
            return valid.count(encoding);
        }

        bool isValidXmlStandalone(std::string_view standalone)
        {
            return standalone == "yes" || standalone == "no";
        }
    }
}
