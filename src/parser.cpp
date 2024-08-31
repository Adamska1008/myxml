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
            throw UnexpectedEndOfInput();
        // validate heading character
        if (auto head = this->peek(); !head || (!std::isalpha(*head) && head != '_'))
        {
            throw SyntaxError(fmt::format("element name which starts with {} is invalid.", *head));
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
            throw UnexpectedEndOfInput();
        }
        if (this->peek() != '"')
        {
            throw SyntaxError(fmt::format("expected '\"' at the beginning of string literal, find {}", *this->peek()));
        }
        this->take();
        std::size_t len = 0;
        while (this->afterN(len) != '"')
        {
            len++;
        }
        if (!this->afterN(len))
        { // if jump out due to length limit
            throw SyntaxError(fmt::format("missing closing double quote for string literal"));
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
            throw SyntaxError(fmt::format("expected '=' after attribute name"));
        }
        attr.first = key;
        auto value = this->parseStringLiteral();
        attr.second = value;
        return attr;
    }

    std::shared_ptr<Text> Parser::parseText()
    {
        if (!this->peek())
        {
            throw UnexpectedEndOfInput();
        }
        std::size_t len = 0;
        while (this->afterN(len) != '<')
        {
            len++;
        }
        if (!this->afterN(len))
        { // if jump out of while loop due to length limit
            throw SyntaxError(fmt::format("expected '<' after text"));
        }
        return std::shared_ptr<Text>(new Text(*this->takeN(len)));
    }

    std::optional<std::shared_ptr<CData>> Parser::parseCData()
    {
        if (this->peekN(9) != "<![CDATA[")
        {
            return std::nullopt;
        }
        this->takeN(9);
        std::size_t len = 0;
        while (this->afterNM(len, 3) != "]]>")
        {
            len++;
        }
        if (!this->afterN(len + 2))
        {
            throw SyntaxError(fmt::format("expected \"]]>\" after CDATA"));
        }
        auto it = std::string(*this->takeN(len));
        this->takeN(2);
        return std::make_shared<CData>(it);
    }

    std::optional<ElementTag> Parser::ParseTag()
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
                throw SyntaxError(fmt::format("unexpected ending '/' found in closing tag"));
            }
            tag.type = ElementTag::ClosingType::Closed;
            this->take();
        }
        if (this->take() != '>')
        {
            throw SyntaxError(fmt::format("expected '>' at the end of the tag"));
        }
        return tag;
    }

    std::shared_ptr<Element> Parser::parseElementWithHeader(ElementTag header)
    {
        auto elem = Element::New();
        elem->SetName(header.name);
        while (auto ch = this->peek())
        {
            switch (*ch)
            {
            case '<':
            {
                if (auto cdata = this->parseCData(); cdata)
                {
                    elem->InsertAtEnd(*cdata);
                    continue;
                }
                auto tag = this->ParseTag(); // impossible to be std::nullopt
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
                    auto child = Element::New();
                    child->SetName(tag->name);
                    if (!tag->attris.empty())
                    {
                        child->ExtendAttributes(tag->attris);
                    }
                    elem->InsertAtEnd(child);
                    break;
                }
                case ElementTag::ClosingType::Closing:
                    if (tag->name != elem->GetName())
                    {
                        throw SyntaxError(fmt::format("elem name in closing tag is mismatched with the header"));
                    }
                    if (!header.attris.empty())
                    {
                        elem->ExtendAttributes(header.attris);
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
        throw UnexpectedEndOfInput();
    }

    std::optional<std::shared_ptr<Element>> Parser::ParseElement()
    {
        this->skipWhiteSpaces();
        if (auto tag = this->ParseTag(); tag)
        {
            if (tag->type == ElementTag::ClosingType::Closed)
            {
                auto elem = Element::New();
                elem->SetName(tag->name);
                if (!tag->attris.empty())
                {
                    elem->ExtendAttributes(tag->attris);
                }
                return elem;
            }
            else if (tag->type == ElementTag::ClosingType::Open)
            {
                return this->parseElementWithHeader(*tag);
            }
            else // Closing </tag>
            {
                throw SyntaxError(fmt::format("unexpected closing tag"));
            }
        }
        else
        {
            return std::nullopt;
        }
    }

    std::optional<Declaration> Parser::parseDeclaration()
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
            throw SyntaxError(fmt::format("expected \"?>\" at end of xml declaration"));
        }
        if (auto decl = Declaration::BuildFromAttrs(attrs); decl)
        {
            return decl;
        }
        else
        {
            throw SemanticError(fmt::format("declaration has incorrect attributes"));
        }
    }

    Document Parser::ParseDocument()
    {
        Document document;
        if (auto decl = this->parseDeclaration(); decl)
        {
            document.SetDeclaration(*decl);
        }
        if (auto root = this->ParseElement(); root)
        {
            document.SetRoot(*root);
        }
        else
        {
            throw SemanticError(fmt::format("missing root element in xml document"));
        }
        return document;
    }

    Parser::Parser(std::string_view buffer)
    {
        this->buffer = std::make_shared<StringBuffer>(buffer);
    }

    Parser::Parser(std::string &&buffer)
    {
        this->buffer = std::make_shared<StringBuffer>(buffer);
    }

    bool util::isValidXmlChar(char ch)
    {
        return std::isalnum(ch) || ch == '_' || ch == '-' || ch == '.' || ch == ':';
    }
}
