#pragma once
#include "element.hpp"
#include "document.hpp"

namespace myxml
{
    // No effect currently. Just use it to mark what is a tag.
    struct Tag
    {
    };

    struct ElementTag
    {
        enum class ClosingType
        {
            Open,   // <tag>
            Closed, // <tag/>
            Closing // </tag>
        };

        std::string name;
        ElementTag::ClosingType type = ClosingType::Open;
        std::map<std::string, std::string> attris;
    };

    struct ProcessingInstruction
    {
    };

    class Parser
    {
    private:
        std::string buffer;
        std::size_t offset;

        /**
         * TODO:
         * Define Exceptions , so for all parsing method,
         * return std::nullopt means `not this one`,
         * and throw exception means `parsing error`
         */

        void skipWhiteSpaces();
        std::optional<char> peekChar();
        std::optional<std::string> peekNextNChars(int);
        std::optional<char> nextChar();
        std::optional<std::string> nextNChars(int);

        /**
         * Parse an identity.
         * @return if find no identity, return `std::nullptr`
         */
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
        std::optional<std::shared_ptr<Element>> parseElementWithHeader(ElementTag header);
        // return the declartion
        // will not consume buffer if failed
        std::optional<Declaration> parseDeclaration();

    public:
        // return and consume current element
        // will consume buffer if failed
        std::optional<std::shared_ptr<Element>> ParseElement();
        // return and consume current tag
        // will consume buffer if failed
        std::optional<ElementTag> ParseTag();
        // return and consume whole document
        // will consume buffer if failed
        std::optional<Document> ParseDocument();
        Parser() = delete;
        explicit Parser(std::string_view);
    };

    namespace util
    {
        bool isValidXmlChar(char ch);
    }
}
