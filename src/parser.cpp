#include <cassert>
#include <fmt/core.h>
#include "myxml/parser.hpp"
#include "myxml/error.hpp"

namespace myxml
{
    void parser::skip_whitespaces()
    {
        while (this->peek() && std::isspace(*this->peek()))
        {
            this->take();
        }
    }

    std::tuple<std::size_t, std::size_t> parser::cur_loc()
    {
        return this->_buffer->cur_loc();
    }

    std::optional<char> parser::peek()
    {
        return this->_buffer->peek();
    }

    std::optional<std::string_view> parser::peek_n(int n)
    {
        return this->_buffer->peek_n(n);
    }

    std::optional<char> parser::after_n(int n)
    {
        return this->_buffer->after_n(n);
    }

    std::optional<std::string_view> parser::after_n_m(int n, int m)
    {
        return this->_buffer->after_n_m(n, m);
    }

    std::optional<char> parser::take()
    {
        return this->_buffer->take();
    }

    std::optional<std::string_view> parser::take_n(int n)
    {
        return this->_buffer->take_n(n);
    }

    std::string parser::parse_ident()
    {
        if (this->peek() == std::nullopt)
        {
            auto [line, col] = this->cur_loc();
            throw unexpected_eof(line, col);
        }
        // validate heading character
        if (auto head = this->peek(); !head || (!std::isalpha(*head) && head != '_'))
        {
            auto [line, col] = this->cur_loc();
            throw syntax_error(fmt::format("element name which starts with {} is invalid.", *head), line, col);
        }
        std::size_t len = 0;
        while (this->after_n(len) && util::is_valid_xml_char(*this->after_n(len)))
        {
            len++;
        }
        return std::string(*this->take_n(len));
    }

    std::string parser::parse_str_literal()
    {
        if (!this->peek())
        {
            auto [line, col] = this->cur_loc();
            throw unexpected_eof(line, col);
        }
        if (this->peek() != '"')
        {
            auto [line, col] = this->cur_loc();
            throw syntax_error(fmt::format("expected '\"' at the beginning of string literal, find {}", *this->peek()), line, col);
        }
        this->take();
        std::size_t len = 0;
        while (this->after_n(len) != '"')
        {
            len++;
        }
        if (!this->after_n(len))
        { // if jump out due to length limit
            auto [line, col] = this->cur_loc();
            throw syntax_error(fmt::format("missing closing double quote for string literal"), line, col);
        }
        auto it = this->take_n(len);
        this->take(); // skip "
        return std::string(*it);
    }

    std::optional<std::pair<std::string, std::string>> parser::parse_attribute()
    {
        this->skip_whitespaces();
        std::pair<std::string, std::string> attr;
        std::string key;
        try
        {
            key = this->parse_ident();
        }
        catch (syntax_error e)
        { // Only SyntaxError in parseIdent is incorrect heading character
            return std::nullopt;
        }
        catch (unexpected_eof e)
        { // There must be `>` or else after all attributes
            throw e;
        }
        if (this->take() != '=')
        {
            auto [line, col] = this->cur_loc();
            throw syntax_error(fmt::format("expected '=' after attribute name"), line, col);
        }
        attr.first = key;
        auto value = this->parse_str_literal();
        attr.second = value;
        return attr;
    }

    std::shared_ptr<text_impl> parser::parse_text()
    {
        if (!this->peek())
        {
            auto [line, col] = this->cur_loc();
            throw unexpected_eof(line, col);
        }
        std::size_t len = 0;
        while (this->after_n(len) != '<')
        {
            len++;
        }
        if (!this->after_n(len))
        { // if jump out of while loop due to length limit
            auto [line, col] = this->cur_loc();
            throw syntax_error(fmt::format("expected '<' after text"), line, col);
        }
        return std::shared_ptr<text_impl>(new text_impl(*this->take_n(len)));
    }

    std::shared_ptr<cdata_impl> parser::parse_cdata()
    {
        if (this->peek_n(9) != "<![CDATA[")
        {
            return nullptr;
        }
        this->take_n(9);
        std::size_t len = 0;
        while (this->after_n_m(len, 3) != "]]>")
        {
            len++;
        }
        if (!this->after_n(len + 2))
        {
            auto [line, col] = this->cur_loc();
            throw syntax_error(fmt::format("expected \"]]>\" after CDATA"), line, col);
        }
        auto it = std::string(*this->take_n(len));
        this->take_n(2);
        return std::make_shared<cdata_impl>(it);
    }

    std::optional<element_tag> parser::parse_element_tag()
    {
        if (this->take() != '<')
        {
            return std::nullopt;
        }
        element_tag tag;
        if (this->peek() == '/')
        {
            tag.type = element_tag::closing_type::Closing;
            this->take();
        }
        this->skip_whitespaces();
        auto name = this->parse_ident();
        tag.name = name;
        this->skip_whitespaces();
        while (auto attr = this->parse_attribute())
        {
            tag.attrs.insert(*attr);
        }
        this->skip_whitespaces();
        if (this->peek() == '/')
        {
            if (tag.type != element_tag::closing_type::Open)
            {
                auto [line, col] = this->cur_loc();
                throw syntax_error(fmt::format("unexpected ending '/' found in closing tag"), line, col);
            }
            tag.type = element_tag::closing_type::Closed;
            this->take();
        }
        if (this->take() != '>')
        {
            auto [line, col] = this->cur_loc();
            throw syntax_error(fmt::format("expected '>' at the end of the tag"), line, col);
        }
        return tag;
    }

    std::shared_ptr<element_impl> parser::parse_element_with_header(element_tag header)
    {
        auto elem = element_impl::_new();
        elem->name = header.name;
        while (auto ch = this->peek())
        {
            switch (*ch)
            {
            case '<':
            {
                if (auto cdata = this->parse_cdata(); cdata)
                {
                    elem->push_back(cdata);
                    continue;
                }
                auto tag = this->parse_element_tag(); // impossible to be std::nullopt
                assert(tag);
                switch (tag->type)
                {
                case element_tag::closing_type::Open:
                {
                    auto child = this->parse_element_with_header(*tag);
                    elem->push_back(child);
                    break;
                }
                case element_tag::closing_type::Closed:
                {
                    auto child = element_impl::_new();
                    child->name = tag->name;
                    if (!tag->attrs.empty())
                    {
                        child->extend_attributes(tag->attrs);
                    }
                    elem->push_back(child);
                    break;
                }
                case element_tag::closing_type::Closing:
                    if (tag->name != elem->name)
                    {
                        auto [line, col] = this->cur_loc();
                        throw syntax_error(fmt::format("elem name in closing tag is mismatched with the header"), line, col);
                    }
                    if (!header.attrs.empty())
                    {
                        elem->extend_attributes(header.attrs);
                    }
                    return elem;
                default:
                    assert(false && "Invalid ElementTag Type");
                }
                break;
            }
            default:
                auto text = this->parse_text();
                elem->push_back(text);
                break;
            }
        }
        auto [line, col] = this->cur_loc();
        throw unexpected_eof(line, col);
    }

    std::shared_ptr<element_impl> parser::parse_element()
    {
        this->skip_whitespaces();
        if (auto tag = this->parse_element_tag(); tag)
        {
            if (tag->type == element_tag::closing_type::Closed)
            {
                auto elem = element_impl::_new();
                elem->name = tag->name;
                if (!tag->attrs.empty())
                {
                    elem->extend_attributes(tag->attrs);
                }
                return elem;
            }
            else if (tag->type == element_tag::closing_type::Open)
            {
                return this->parse_element_with_header(*tag);
            }
            else // Closing </tag>
            {
                auto [line, col] = this->cur_loc();
                throw syntax_error(fmt::format("unexpected closing tag"), line, col);
            }
        }
        else
        {
            return nullptr;
        }
    }

    std::optional<declaration> parser::parse_declaration()
    {
        if (this->peek_n(5) != "<?xml")
        {
            return std::nullopt;
        }
        this->take_n(5);
        std::map<std::string, std::string> attrs;
        while (auto attr = this->parse_attribute())
        {
            attrs.insert(*attr);
        }
        this->skip_whitespaces();
        if (this->take_n(2) != "?>")
        {
            auto [line, col] = this->cur_loc();
            throw syntax_error(fmt::format("expected \"?>\" at end of xml declaration"), line, col);
        }
        if (auto decl = declaration::from_attrs(attrs); decl)
        {
            return decl;
        }
        else
        {
            auto [line, col] = this->cur_loc();
            throw semantic_error(fmt::format("declaration has incorrect attributes"), line, col);
        }
    }

    document parser::parse_document()
    {
        document document;
        if (auto decl = this->parse_declaration(); decl)
        {
            document.set_declaration(*decl);
        }
        if (auto root = this->parse_element(); root)
        {
            document.set_root(root);
        }
        else
        {
            auto [line, col] = this->cur_loc();
            throw semantic_error(fmt::format("missing root element in xml document"), line, col);
        }
        return document;
    }

    parser::parser(std::string_view buffer)
        : _buffer(std::make_shared<string_buffer>(buffer))
    {
    }

    parser::parser(std::string &&buffer)
        : _buffer(std::make_shared<string_buffer>(buffer))
    {
    }

    bool util::is_valid_xml_char(char ch)
    {
        return std::isalnum(ch) || ch == '_' || ch == '-' || ch == '.' || ch == ':';
    }
}
