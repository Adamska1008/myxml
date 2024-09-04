#include <cassert>
#include <fmt/core.h>
#include "myxml/parser.hpp"
#include "myxml/error.hpp"

namespace myxml
{
    void Parser::skipWhiteSpaces()
    {
        while (this->peek() && std::isspace(*this->peek()))
        {
            this->take();
        }
    }

    std::tuple<std::size_t, std::size_t> Parser::currentLoc()
    {
        return this->buffer->CurrentLocation();
    }

    std::optional<char> Parser::peek()
    {
        return this->buffer->Peek();
    }

    std::optional<std::string_view> Parser::peekN(int n)
    {
        return this->buffer->PeekN(n);
    }

    std::optional<char> Parser::afterN(int n)
    {
        return this->buffer->AfterN(n);
    }

    std::optional<std::string_view> Parser::afterNM(int n, int m)
    {
        return this->buffer->AfterNM(n, m);
    }

    std::optional<char> Parser::take()
    {
        return this->buffer->Take();
    }

    std::optional<std::string_view> Parser::takeN(int n)
    {
        return this->buffer->TakeN(n);
    }

    std::string Parser::parseIdent()
    {
        if (this->peek() == std::nullopt)
        {
            auto [line, col] = this->currentLoc();
            throw UnexpectedEndOfInput(line, col);
        }
        // validate heading character
        if (auto head = this->peek(); !head || (!std::isalpha(*head) && head != '_'))
        {
            auto [line, col] = this->currentLoc();
            throw SyntaxError(fmt::format("element name which starts with {} is invalid.", *head), line, col);
        }
        std::size_t len = 0;
        while (this->afterN(len) && util::isValidXmlChar(*this->afterN(len)))
        {
            len++;
        }
        return std::string(*this->takeN(len));
    }

    std::string Parser::parseStringLiteral()
    {
        if (!this->peek())
        {
            auto [line, col] = this->currentLoc();
            throw UnexpectedEndOfInput(line, col);
        }
        if (this->peek() != '"')
        {
            auto [line, col] = this->currentLoc();
            throw SyntaxError(fmt::format("expected '\"' at the beginning of string literal, find {}", *this->peek()), line, col);
        }
        this->take();
        std::size_t len = 0;
        while (this->afterN(len) != '"')
        {
            len++;
        }
        if (!this->afterN(len))
        { // if jump out due to length limit
            auto [line, col] = this->currentLoc();
            throw SyntaxError(fmt::format("missing closing double quote for string literal"), line, col);
        }
        auto it = this->takeN(len);
        this->take(); // skip "
        return std::string(*it);
    }

    std::optional<std::pair<std::string, std::string>> Parser::parseAttribute()
    {
        this->skipWhiteSpaces();
        std::pair<std::string, std::string> attr;
        std::string key;
        try
        {
            key = this->parseIdent();
        }
        catch (SyntaxError e)
        { // Only SyntaxError in parseIdent is incorrect heading character
            return std::nullopt;
        }
        catch (UnexpectedEndOfInput e)
        { // There must be `>` or else after all attributes
            throw e;
        }
        if (this->take() != '=')
        {
            auto [line, col] = this->currentLoc();
            throw SyntaxError(fmt::format("expected '=' after attribute name"), line, col);
        }
        attr.first = key;
        auto value = this->parseStringLiteral();
        attr.second = value;
        return attr;
    }

    std::shared_ptr<text_impl> Parser::parseText()
    {
        if (!this->peek())
        {
            auto [line, col] = this->currentLoc();
            throw UnexpectedEndOfInput(line, col);
        }
        std::size_t len = 0;
        while (this->afterN(len) != '<')
        {
            len++;
        }
        if (!this->afterN(len))
        { // if jump out of while loop due to length limit
            auto [line, col] = this->currentLoc();
            throw SyntaxError(fmt::format("expected '<' after text"), line, col);
        }
        return std::shared_ptr<text_impl>(new text_impl(*this->takeN(len)));
    }

    std::shared_ptr<CData> Parser::parseCData()
    {
        if (this->peekN(9) != "<![CDATA[")
        {
            return nullptr;
        }
        this->takeN(9);
        std::size_t len = 0;
        while (this->afterNM(len, 3) != "]]>")
        {
            len++;
        }
        if (!this->afterN(len + 2))
        {
            auto [line, col] = this->currentLoc();
            throw SyntaxError(fmt::format("expected \"]]>\" after CDATA"), line, col);
        }
        auto it = std::string(*this->takeN(len));
        this->takeN(2);
        return std::make_shared<CData>(it);
    }

    std::optional<ElementTag> Parser::ParseElementTag()
    {
        if (this->take() != '<')
        {
            return std::nullopt;
        }
        ElementTag tag;
        if (this->peek() == '/')
        {
            tag.type = ElementTag::ClosingType::Closing;
            this->take();
        }
        this->skipWhiteSpaces();
        auto name = this->parseIdent();
        tag.name = name;
        this->skipWhiteSpaces();
        while (auto attr = this->parseAttribute())
        {
            tag.attris.insert(*attr);
        }
        this->skipWhiteSpaces();
        if (this->peek() == '/')
        {
            if (tag.type != ElementTag::ClosingType::Open)
            {
                auto [line, col] = this->currentLoc();
                throw SyntaxError(fmt::format("unexpected ending '/' found in closing tag"), line, col);
            }
            tag.type = ElementTag::ClosingType::Closed;
            this->take();
        }
        if (this->take() != '>')
        {
            auto [line, col] = this->currentLoc();
            throw SyntaxError(fmt::format("expected '>' at the end of the tag"), line, col);
        }
        return tag;
    }

    std::shared_ptr<element_impl> Parser::parseElementWithHeader(ElementTag header)
    {
        auto elem = element_impl::_new();
        elem->name = header.name;
        while (auto ch = this->peek())
        {
            switch (*ch)
            {
            case '<':
            {
                if (auto cdata = this->parseCData(); cdata)
                {
                    elem->InsertAtEnd(cdata);
                    continue;
                }
                auto tag = this->ParseElementTag(); // impossible to be std::nullopt
                assert(tag);
                switch (tag->type)
                {
                case ElementTag::ClosingType::Open:
                {
                    auto child = this->parseElementWithHeader(*tag);
                    elem->InsertAtEnd(child);
                    break;
                }
                case ElementTag::ClosingType::Closed:
                {
                    auto child = element_impl::_new();
                    child->name = tag->name;
                    if (!tag->attris.empty())
                    {
                        child->extend_attributes(tag->attris);
                    }
                    elem->InsertAtEnd(child);
                    break;
                }
                case ElementTag::ClosingType::Closing:
                    if (tag->name != elem->name)
                    {
                        auto [line, col] = this->currentLoc();
                        throw SyntaxError(fmt::format("elem name in closing tag is mismatched with the header"), line, col);
                    }
                    if (!header.attris.empty())
                    {
                        elem->extend_attributes(header.attris);
                    }
                    return elem;
                default:
                    assert(false && "Invalid ElementTag Type");
                }
                break;
            }
            default:
                auto text = this->parseText();
                elem->InsertAtEnd(text);
                break;
            }
        }
        auto [line, col] = this->currentLoc();
        throw UnexpectedEndOfInput(line, col);
    }

    std::shared_ptr<element_impl> Parser::ParseElement()
    {
        this->skipWhiteSpaces();
        if (auto tag = this->ParseElementTag(); tag)
        {
            if (tag->type == ElementTag::ClosingType::Closed)
            {
                auto elem = element_impl::_new();
                elem->name = tag->name;
                if (!tag->attris.empty())
                {
                    elem->extend_attributes(tag->attris);
                }
                return elem;
            }
            else if (tag->type == ElementTag::ClosingType::Open)
            {
                return this->parseElementWithHeader(*tag);
            }
            else // Closing </tag>
            {
                auto [line, col] = this->currentLoc();
                throw SyntaxError(fmt::format("unexpected closing tag"), line, col);
            }
        }
        else
        {
            return nullptr;
        }
    }

    std::optional<declaration> Parser::parseDeclaration()
    {
        if (this->peekN(5) != "<?xml")
        {
            return std::nullopt;
        }
        this->takeN(5);
        std::map<std::string, std::string> attrs;
        while (auto attr = this->parseAttribute())
        {
            attrs.insert(*attr);
        }
        this->skipWhiteSpaces();
        if (this->takeN(2) != "?>")
        {
            auto [line, col] = this->currentLoc();
            throw SyntaxError(fmt::format("expected \"?>\" at end of xml declaration"), line, col);
        }
        if (auto decl = declaration::from_attrs(attrs); decl)
        {
            return decl;
        }
        else
        {
            auto [line, col] = this->currentLoc();
            throw SemanticError(fmt::format("declaration has incorrect attributes"), line, col);
        }
    }

    document Parser::ParseDocument()
    {
        document document;
        if (auto decl = this->parseDeclaration(); decl)
        {
            document.set_declaration(*decl);
        }
        if (auto root = this->ParseElement(); root)
        {
            document.set_root(root);
        }
        else
        {
            auto [line, col] = this->currentLoc();
            throw SemanticError(fmt::format("missing root element in xml document"), line, col);
        }
        return document;
    }

    Parser::Parser(std::string_view buffer)
        : buffer(std::make_shared<StringBuffer>(buffer))
    {
    }

    Parser::Parser(std::string &&buffer)
        : buffer(std::make_shared<StringBuffer>(buffer))
    {
    }

    bool util::isValidXmlChar(char ch)
    {
        return std::isalnum(ch) || ch == '_' || ch == '-' || ch == '.' || ch == ':';
    }
}
