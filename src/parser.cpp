#include <cassert>
#include <fmt/core.h>
#include "myxml/parser.hpp"
#include "myxml/error.hpp"

namespace myxml
{
    void Parser::skipWhiteSpaces()
    {
        while (this->offset < this->buffer.length() && std::isspace(this->buffer[this->offset]))
        {
            this->offset++;
        }
    }

    std::optional<char> Parser::peekChar()
    {
        if (this->offset < this->buffer.length())
        {
            return this->buffer[this->offset];
        }
        else
        {
            return std::nullopt;
        }
    }

    std::optional<std::string> Parser::peekNextNChars(int n)
    {
        if (this->offset + n - 1 < this->buffer.length())
        {
            return this->buffer.substr(this->offset, n);
        }
        else
        {
            return std::nullopt;
        }
    }

    std::optional<char> Parser::nextChar()
    {
        if (auto peek = this->peekChar(); peek.has_value())
        {
            this->offset++;
            return peek;
        }
        else
            return std::nullopt;
    }

    std::optional<std::string> Parser::nextNChars(int n)
    {
        auto nchars = this->buffer.substr(this->offset, n);
        this->offset += n;
        return nchars;
    }

    std::string Parser::parseIdent()
    {
        if (this->peekChar() == std::nullopt)
            throw UnexpectedEndOfInput();
        std::size_t begin = this->offset;
        // validate heading character
        if (auto head = this->peekChar(); !head || (!std::isalpha(*head) && head != '_'))
        {
            throw SyntaxError(fmt::format("element name which starts with {} is invalid.", *head));
        }
        std::size_t len = 0;
        while (begin + len < this->buffer.length() &&
               util::isValidXmlChar(this->buffer[begin + len]))
        {
            len++;
        }
        this->offset += len;
        return this->buffer.substr(begin, len);
    }

    std::string Parser::parseStringLiteral()
    {
        if (!this->peekChar())
        {
            throw UnexpectedEndOfInput();
        }
        if (this->peekChar() != '"')
        {
            throw SyntaxError(fmt::format("expected '\"' at the beginning of string literal, find {}", *this->peekChar()));
        }
        std::size_t cur = this->offset; // this->offset points to `"`
        while (cur + 1 < this->buffer.length() && this->buffer[cur + 1] != '"')
        {
            cur++;
        }
        if (cur + 1 >= this->buffer.length())
        { // if jump out due to length limit
            throw SyntaxError(fmt::format("missing closing double quote for string literal"));
        }
        auto literal = this->buffer.substr(this->offset + 1, cur - this->offset);
        this->offset = cur + 2; // cur + 1 -> `"`
        return literal;
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
        if (this->nextChar() != '=')
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
        if (!this->peekChar())
        {
            throw UnexpectedEndOfInput();
        }
        std::size_t begin = this->offset;
        std::size_t len = 0;
        while (begin + len < this->buffer.length() &&
               this->buffer[begin + len] != '<')
        {
            len++;
        }
        if (this->buffer[begin + len] != '<')
        { // if jump out of while loop due to length limit
            throw SyntaxError(fmt::format("expected '<' after text"));
        }
        this->offset += len;
        return std::shared_ptr<Text>(new Text(this->buffer.substr(begin, len)));
    }

    std::optional<ElementTag> Parser::ParseTag()
    {
        if (this->nextChar() != '<')
        {
            return std::nullopt;
        }
        ElementTag tag;
        if (this->peekChar() == '/')
        {
            tag.type = ElementTag::ClosingType::Closing;
            this->nextChar();
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
        if (this->peekChar() == '/')
        {
            if (tag.type != ElementTag::ClosingType::Open)
            {
                throw SyntaxError(fmt::format("unexpected ending '/' found in closing tag"));
            }
            tag.type = ElementTag::ClosingType::Closed;
            this->nextChar();
        }
        if (this->nextChar() != '>')
        {
            throw SyntaxError(fmt::format("expected '>' at the end of the tag"));
        }
        return tag;
    }

    std::shared_ptr<Element> Parser::parseElementWithHeader(ElementTag header)
    {
        auto elem = Element::New();
        elem->SetName(header.name);
        while (auto ch = this->peekChar())
        {
            switch (*ch)
            {
            case '<':
            {
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
                        throw SyntaxError(fmt::format(""));
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
        if (this->peekNextNChars(5) != "<?xml")
        {
            return std::nullopt;
        }
        this->nextNChars(5);
        std::map<std::string, std::string> attrs;
        while (auto attr = this->parseAttribute())
        {
            attrs.insert(*attr);
        }
        this->skipWhiteSpaces();
        if (this->nextNChars(2) != "?>")
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
        : buffer(buffer), offset(0) {}

    bool util::isValidXmlChar(char ch)
    {
        return std::isalnum(ch) || ch == '_' || ch == '-' || ch == '.' || ch == ':';
    }
}
