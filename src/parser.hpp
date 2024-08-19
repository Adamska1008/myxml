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
        std::optional<char> peekChar();
        std::optional<char> nextChar();
        std::optional<std::string> parseElementName();
        std::optional<Text> parseText();

    public:
        std::optional<std::shared_ptr<Element>> ParseElement();
        std::optional<Tag> ParseTag();
        Parser() = delete;
        explicit Parser(std::string_view);
    };

    namespace util
    {
        bool isValidXmlChar(char ch);
    }
}
