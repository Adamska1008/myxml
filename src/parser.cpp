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
        if (auto head = this->peekChar().value(); !std::isalpha(head) && head != '_')
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

    std::optional<Text> Parser::parseText()
    {
        if (this->peekChar() == std::nullopt)
            return std::nullopt;
        std::size_t begin = this->offset;
        std::size_t len = 0;
        while (begin + len < this->buffer.length() &&
               this->buffer[begin + len] != '<')
        {
            len++;
        }
        if (this->buffer[begin + len] != '<')
        { // break due to length limit
            return std::nullopt;
        }
        this->offset += len;
        return Text(this->buffer.substr(begin, len));
    }

    std::optional<std::shared_ptr<Element>> Parser::ParseElement()
    {
        this->skipWhiteSpaces();
        if (this->peekChar() == std::nullopt || this->peekChar().value() != '<')
        {
            return std::nullopt;
        }
        this->nextChar();
        this->skipWhiteSpaces();
        std::shared_ptr<Element> elem;
        if (auto name = this->parseElementName(); name != std::nullopt)
        {
            elem = Element::New(name.value());
        }
        else
        {
            return std::nullopt;
        }
        this->skipWhiteSpaces();
        if (this->peekChar() == '/')
        {
            elem->SetClosingType(Element::ClosingType::Closed);
            this->nextChar();
            return elem;
        }
        else if (this->nextChar() != '>')
        {
            return std::nullopt;
        }
        if (auto text = this->parseText(); text != std::nullopt)
        {
            elem->SetText(text.value());
        }
        else
        {
            return std::nullopt;
        }
        this->nextChar();
        if (this->nextChar() != '/')
        {
            return std::nullopt;
        }
        if (auto end = this->parseElementName(); end == std::nullopt | end.value() != elem->GetName())
        {
            return std::nullopt;
        }
        if (this->nextChar() != '>')
        {
            return std::nullopt;
        }
        return elem;
    }

    Parser::Parser(std::string_view buffer)
        : buffer(buffer), offset(0) {}

    bool util::isValidXmlChar(char ch)
    {
        return std::isalnum(ch) || ch == '_' || ch == '-' || ch == '.' || ch == ':';
    }
}
