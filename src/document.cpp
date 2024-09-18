#include <set>
#include <iostream>
#include <fmt/core.h>
#include "myxml/document.hpp"
#include "myxml/parser.hpp"
#include "myxml/xmlfile.hpp"

namespace myxml
{
    void document::set_declaration(const declaration &decl)
    {
        this->_decl = decl;
    }

    void document::set_root(std::shared_ptr<element_impl> root)
    {
        this->_root = root;
    }

    declaration &document::get_declaration()
    {
        return this->_decl;
    }

    element &document::root()
    {
        return this->_root;
    }

    document document::parse(std::string_view input)
    {
        return parser(input).parse_document();
    }

    document document::load(std::string fileName)
    {
        auto f = xml_file::open_file(fileName);
        return parser(f).parse_document();
    }

    void document::print(std::ostream &os) const
    {
        os << this->_decl << this->_root;
    }
    void document::entity_encoding(bool flag)
    {
        this->_root.entity_encoding(flag);
    }

    void document::platform_specific_newline(bool flag)
    {
        this->_root.platform_specific_newline(flag);
    }

    std::optional<declaration> declaration::from_attrs(std::map<std::string, std::string> attrs)
    {
        if (!attrs.count("version") || !util::is_valid_xml_version(attrs["version"]))
        {
            return std::nullopt;
        }
        declaration declaration;
        declaration.version = attrs["version"];
        if (attrs.count("encoding"))
        {
            auto encoding = attrs["encoding"];
            if (!util::is_valid_xml_encoding(encoding))
            {
                return std::nullopt;
            }
            declaration.encoding = encoding;
        }
        if (attrs.count("standalone"))
        {
            auto standalone = attrs["standalone"];
            if (!util::is_valid_xml_standalone(standalone))
            {
                return std::nullopt;
            }
            declaration.standalone = standalone;
        }
        return declaration;
    }

    namespace util
    {
        bool is_valid_xml_version(std::string_view version)
        {
            return version == "1.0" || version == "1.1";
        }

        bool is_valid_xml_encoding(std::string_view encoding)
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

        bool is_valid_xml_standalone(std::string_view standalone)
        {
            return standalone == "yes" || standalone == "no";
        }
    }

    document literals::operator""_doc(const char *literal, std::size_t len)
    {
        return document::parse(std::string_view(literal, len));
    }
}
