#pragma once
#include "element.hpp"

namespace myxml
{
    struct Tag
    {
        enum class ClosingType
        {
            Open,   // <tag>
            Closed, // <tag/>
            Closing // </tag>
        };

        std::string name;
        Tag::ClosingType type = ClosingType::Open;
    };

    class Parser
    {
    private:
        std::string buffer;
        std::size_t offset;

        void skipWhiteSpaces();
        // return and not consume current character
        std::optional<char> peekChar();
        // return and consume current character
        std::optional<char> nextChar();
        // return and consume a string literal
        std::optional<std::string> parseElementName();
        // return and consume pcdata
        std::optional<std::shared_ptr<Text>> parseText();

    public:
        // return and consume current element
        std::optional<std::shared_ptr<Element>> ParseElement();
        // return and consume current tag
        std::optional<Tag> ParseTag();
        Parser() = delete;
        explicit Parser(std::string_view);
    };

    namespace util
    {
        bool isValidXmlChar(char ch);
    }
}
