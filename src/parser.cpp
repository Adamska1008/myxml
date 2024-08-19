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

    std::optional<std::string> Parser::parseElementName()
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

    std::optional<std::shared_ptr<Element>> Parser::parseElementWithHeader(Tag header)
    {
        this->skipWhiteSpaces();
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
                case Tag::ClosingType::Open:
                    if (auto child = this->parseElementWithHeader(*tag); child)
                    {
                        elem->InsertAtEnd(*child);
                    }
                    else
                    {
                        return std::nullopt;
                    }
                    break;
                case Tag::ClosingType::Closed:
                {
                    auto child = Element::New();
                    child->SetName(tag->name);
                    elem->InsertAtEnd(child);
                    break;
                }
                case Tag::ClosingType::Closing:
                    if (tag->name != elem->GetName())
                    {
                        return std::nullopt;
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
            if (tag->type == Tag::ClosingType::Closed)
            {
                auto elem = Element::New();
                elem->SetName(tag->name);
                return elem;
            }
            else if (tag->type == Tag::ClosingType::Closing)
            {
                return std::nullopt;
            }
            return this->parseElementWithHeader(*tag);
        }
        else
        {
            return std::nullopt;
        }
    }

    std::optional<Tag> Parser::ParseTag()
    {
        if (this->nextChar() != '<')
        {
            return std::nullopt;
        }
        Tag tag;
        if (this->peekChar() == '/')
        {
            tag.type = Tag::ClosingType::Closing;
            this->nextChar();
        }
        this->skipWhiteSpaces();
        if (auto name = this->parseElementName(); name)
        {
            tag.name = *name;
        }
        else
        {
            return std::nullopt;
        }
        this->skipWhiteSpaces();
        if (this->peekChar() == '/')
        {
            if (tag.type != Tag::ClosingType::Open)
            {
                return std::nullopt;
            }
            tag.type = Tag::ClosingType::Closed;
            this->nextChar();
        }
        if (this->nextChar() != '>')
        {
            return std::nullopt;
        }
        return tag;
    }

    Parser::Parser(std::string_view buffer)
        : buffer(buffer), offset(0) {}

    bool util::isValidXmlChar(char ch)
    {
        return std::isalnum(ch) || ch == '_' || ch == '-' || ch == '.' || ch == ':';
    }
}
