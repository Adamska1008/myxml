#include "parser.hpp"

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

    std::optional<std::string> Parser::parseIdent()
    {
        if (this->peekChar() == std::nullopt)
            return std::nullopt;
        std::size_t begin = this->offset;
        // validate heading character
        if (auto head = this->peekChar(); !head || (!std::isalpha(*head) && head != '_'))
        {
            return std::nullopt;
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

    std::optional<std::string> Parser::parseStringLiteral()
    {
        if (this->peekChar() != '"')
        {
            return std::nullopt;
        }
        std::size_t cur = this->offset; // this->offset points to `"`
        while (cur + 1 < this->buffer.length() && this->buffer[cur + 1] != '"')
        {
            cur++;
        }
        if (cur + 1 >= this->buffer.length())
        { // if jump out due to length limit
            return std::nullopt;
        }
        auto literal = this->buffer.substr(this->offset + 1, cur - this->offset);
        this->offset = cur + 2; // cur + 1 -> `"`
        return literal;
    }

    std::optional<std::pair<std::string, std::string>> Parser::parseAttribute()
    {
        this->skipWhiteSpaces();
        std::pair<std::string, std::string> attri;
        if (auto key = this->parseIdent(); key && this->nextChar() == '=')
        {
            attri.first = *key;
            if (auto value = this->parseStringLiteral(); value)
            {
                attri.second = *value;
                return attri;
            }
        }
        return std::nullopt;
    }

    std::optional<std::shared_ptr<Text>> Parser::parseText()
    {
        if (!this->peekChar())
        {
            return std::nullopt;
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
            return std::nullopt;
        }
        this->offset += len;
        return std::shared_ptr<Text>(new Text(this->buffer.substr(begin, len)));
    }

    std::optional<std::shared_ptr<Element>> Parser::parseElementWithHeader(ElementTag header)
    {
        auto elem = Element::New();
        elem->SetName(header.name);
        while (auto ch = this->peekChar())
        {
            switch (*ch)
            {
            case '<':
            {
                auto tag = this->ParseTag();
                switch (tag->type)
                {
                case ElementTag::ClosingType::Open:
                    if (auto child = this->parseElementWithHeader(*tag); child)
                    {
                        elem->InsertAtEnd(*child);
                    }
                    else
                    {
                        return std::nullopt;
                    }
                    break;
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
                        return std::nullopt;
                    }
                    if (!header.attris.empty())
                    {
                        elem->ExtendAttributes(header.attris);
                    }
                    return elem;
                default:
                    return std::nullopt;
                }
                break;
            }
            default:
                if (auto text = this->parseText(); text)
                {
                    elem->InsertAtEnd(*text);
                }
                else
                {
                    return std::nullopt;
                }
                break;
            }
        }
        return std::nullopt;
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
        }
        return std::nullopt;
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
        if (auto name = this->parseIdent(); name)
        {
            tag.name = *name;
        }
        else
        {
            return std::nullopt;
        }
        this->skipWhiteSpaces();
        while (auto attri = this->parseAttribute())
        {
            tag.attris.insert(*attri);
        }
        if (this->peekChar() == '/')
        {
            if (tag.type != ElementTag::ClosingType::Open)
            {
                return std::nullopt;
            }
            tag.type = ElementTag::ClosingType::Closed;
            this->nextChar();
        }
        if (this->nextChar() != '>')
        {
            return std::nullopt;
        }
        return tag;
    }

    std::optional<Document> Parser::ParseDocument()
    {
        return std::optional<Document>();
    }

    Parser::Parser(std::string_view buffer)
        : buffer(buffer), offset(0) {}

    bool util::isValidXmlChar(char ch)
    {
        return std::isalnum(ch) || ch == '_' || ch == '-' || ch == '.' || ch == ':';
    }
}
