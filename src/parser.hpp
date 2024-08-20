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
        std::map<std::string, std::string> attris;
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
        // return and consume a ident
        // will not consume ident if failed
        std::optional<std::string> parseIdent();
        // return and consume a string `"..."`
        // will not consume string if failed
        std::optional<std::string> parseStringLiteral();
        // return and consume an attribute `key="value"`
        std::optional<std::pair<std::string, std::string>> parseAttribute();
        // return and consume pcdata
        // will not consume pcdate if failed
        std::optional<std::shared_ptr<Text>> parseText();
        // return the entire element
        // will consume buffer if failed
        std::optional<std::shared_ptr<Element>> parseElementWithHeader(Tag header);

    public:
        // return and consume current element
        // will consume buffer if failed
        std::optional<std::shared_ptr<Element>> ParseElement();
        // return and consume current tag
        // will consume buffer if failed
        std::optional<Tag> ParseTag();
        Parser() = delete;
        explicit Parser(std::string_view);
    };

    namespace util
    {
        bool isValidXmlChar(char ch);
    }
}
