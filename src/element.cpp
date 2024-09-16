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
        return this->_impl->_name;
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

    std::optional<element> element::first_elem()
    {
        if (auto ptr = _impl->first<element_impl>())
        {
            return element(ptr);
        }
        else
        {
            return std::nullopt;
        }
    }

    std::optional<element> element::first_elem(std::string_view name)
    {
        if (auto ptr = _impl->first_elem(name); ptr)
        {
            return element(ptr);
        }
        else
        {
            return std::nullopt;
        }
    }

    std::optional<text> element::first_text()
    {
        if (auto ptr = _impl->first<text_impl>(); ptr)
        {
            return text(ptr);
        }
        else
        {
            return std::nullopt;
        }
    }

    std::optional<cdata> element::first_cdata()
    {
        if (auto ptr = _impl->first<cdata_impl>(); ptr)
        {
            return cdata(ptr);
        }
        else
        {
            return std::nullopt;
        }
    }

    std::optional<element> element::parent()
    {
        if (_impl->_parent)
        {
            return element(_impl->_parent->as<element_impl>());
        }
        else
        {
            return std::nullopt;
        }
    }

    void element::remove_first(std::string name)
    {
        for (auto child = _impl->first_child(); child != nullptr; child = _impl->next_sibiling())
        {
            if (auto elem = child->as<element_impl>(); elem && elem->_name == name)
            {
                _impl->unlink(child);
            }
        }
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
        : _name(name) {}

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
        auto f = xml_file::open_file(path);
        return parser(f).parse_element();
    }

    void element_impl::extend_attributes(std::map<std::string, std::string> attris)
    {
        this->_attributes.insert(attris.begin(), attris.end());
    }

    std::string &element_impl::operator[](const std::string &key)
    {
        return this->_attributes[key];
    }

    void element_impl::print(std::ostream &os) const
    {
        os << "<" << this->_name;
        if (!_attributes.empty())
        {
            os << " ";
            for (const auto &[key, value] : this->_attributes)
            {
                os << "" << key << "=\"" << value << "\"";
            }
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
        os << "</" << this->_name << ">";
    }

    namespace literals
    {
        element operator""_elem(const char *literal, std::size_t len)
        {
            return element_impl::parse(std::string_view(literal, len));
        }
    }
}
