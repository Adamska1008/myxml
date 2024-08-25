#include <set>
#include "document.hpp"

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
            // Registered at iana.org
            static std::set<std::string, std::less<>> valid{
                "US-ASCII",
                "ISO-8859-1",
                "ISO-8859-2",
                "ISO-8859-3",
                "ISO-8859-4",
                "ISO-8859-5",
                "ISO-8859-6",
                "ISO-8859-7",
                "ISO-8859-8",
                "ISO-8859-9",
                "ISO-8859-10",
                "Shift_JIS",
                "EUC-JP",
                "ISO-2022-KR",
                "EUC-KR",
                "ISO-2022-JP",
                "ISO-2022-JP-2",
                "ISO-8859-6-E",
                "ISO-8859-6-I",
                "ISO-8859-8-E",
                "ISO-8859-8-I",
                "GB2312",
                "Big5",
                "KOI8-R",
            };
            return valid.count(encoding);
        }

        bool isValidXmlStandalone(std::string_view standalone)
        {
            return standalone == "yes" || standalone == "no";
        }
    }
}
