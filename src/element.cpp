#include <iostream>
#include "myxml/element.hpp"
#include "myxml/parser.hpp"
#include "myxml/xmlfile.hpp"

namespace myxml
{
    element::element(std::shared_ptr<element_impl> impl)
        : _impl(impl)
    {
    }

    element::element(std::string_view name)
        : element(element_impl::_new(name))
    {
    }

    std::string_view element::name()
    {
        return this->_impl->name;
    }

    element element::parse(std::string_view xml)
    {
        return element_impl::parse(xml);
    }

    element element::load(std::string_view path)
    {
        return element_impl::load(path);
    }

    std::string &element::operator[](const std::string &attr)
    {
        return (*_impl)[attr];
    }

    element element::first_elem()
    {
        return _impl->first<element_impl>();
    }

    element element::first_elem(std::string_view name)
    {
        return _impl->first_elem(name);
    }

    text element::first_text()
    {
        return _impl->first<text_impl>();
    }

    void element::print(std::ostream &os) const
    {
        _impl->print(os);
    }

    void element::entity_encoding(bool flag)
    {
        return _impl->entity_encoding(flag);
    }

    void element::platform_specific_newline(bool flag)
    {
        return _impl->platform_specific_newline(flag);
    }

    element_impl::element_impl(std::string_view name)
        : name(name) {}

    std::shared_ptr<element_impl> element_impl::_new(std::string_view name)
    {
        return std::shared_ptr<element_impl>(new element_impl(name));
    }

    std::shared_ptr<element_impl> element_impl::_new()
    {
        return std::shared_ptr<element_impl>(new element_impl());
    }

    std::shared_ptr<element_impl> element_impl::parse(std::string_view buf)
    {
        return parser(buf).parse_element();
    }

    std::shared_ptr<element_impl> element_impl::load(std::string_view path)
    {
        auto f = XMLFile::Open(path);
        return parser(f).parse_element();
    }

    void element_impl::extend_attributes(std::map<std::string, std::string> attris)
    {
        this->attributes.insert(attris.begin(), attris.end());
    }

    std::string &element_impl::operator[](const std::string &key)
    {
        return this->attributes[key];
    }

    // std::string element_impl::ExportRaw() const
    // {

    //     std::string builder = "<" + std::string(this->name);
    //     for (const auto &[key, value] : this->attributes)
    //     {
    //         builder += "" + key + "=\"" + value + "\"";
    //     }
    //     if (this->FirstChild() == nullptr)
    //     {
    //         builder += " />";
    //         return builder;
    //     }
    //     builder += ">";
    //     for (auto node = this->FirstChild(); node != nullptr; node = node->NextSibiling())
    //     {
    //         builder += node->ExportRaw();
    //     }
    //     builder += "</" + std::string(this->name) + ">";
    //     return builder;
    // }

    void element_impl::print(std::ostream &os) const
    {
        os << "<" << this->name;
        for (const auto &[key, value] : this->attributes)
        {
            os << "" << key << "=\"" << value << "\"";
        }
        if (this->first_child() == nullptr)
        {
            os << " />";
            return;
        }
        os << ">";
        for (auto node = this->first_child(); node != nullptr; node = node->next_sibiling())
        {
            node->print(os);
        }
        os << "</" << this->name << ">";
    }

    namespace literals
    {
        element operator""_elem(const char *literal, std::size_t len)
        {
            return element_impl::parse(std::string_view(literal, len));
        }
    }
}
