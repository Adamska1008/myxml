#include <set>
#include <fmt/core.h>
#include "myxml/document.hpp"
#include "myxml/parser.hpp"
#include "myxml/xmlfile.hpp"

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

    std::shared_ptr<Element> Document::Elem(std::string_view name)
    {
        return this->root->Elem(name);
    }

    std::shared_ptr<Element> Document::FirstElem()
    {
        return this->root->FirstElem();
    }

    std::shared_ptr<Text> Document::FirstText()
    {
        return this->root->FirstText();
    }

    Document Document::Parse(std::string input)
    {
        return Parser(input).ParseDocument();
    }

    Document Document::ParseFile(std::string fileName)
    {
        auto f = XMLFile::Open(fileName);
        return Parser(f).ParseDocument();
    }
    std::string Document::ExportRaw() const
    {
        return this->declaration.ExportRaw() + this->root->ExportRaw();
    }

    std::string Document::ExportFormatted(int indentLevel, int indentSize) const
    {
        return this->declaration.ExportFormatted(indentLevel + 1, indentSize) + this->root->ExportFormatted(indentLevel + 1, indentSize);
    }

    void Document::SetEntityEncoding(bool flag)
    {
        this->root->SetEntityEncoding(flag);
    }

    void Document::SetPlatformSpecificNewline(bool flag)
    {
        this->root->SetPlatformSpecificNewline(flag);
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

    std::string Declaration::ExportRaw() const
    {
        std::string builder = fmt::format("<?xml version={}", this->version);
        if (this->encoding)
        {
            builder += " encoding=" + (*this->encoding);
        }
        if (this->standalone)
        {
            builder += " standalone=" + (*this->standalone);
        }
        return builder + "?>\n";
    }

    std::string Declaration::ExportFormatted(int indentLevel, int indentSize) const
    {
        return std::string(' ', indentLevel * indentSize) + this->ExportRaw();
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
