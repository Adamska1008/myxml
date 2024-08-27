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
         * @throws `UnexpectedEndOfInput`
         * @throws `SyntaxError` if an invalid character occurs.
         */
        std::string parseIdent();
        /**
         * Parse a string literal
         * @throws `UnexpectedEndOfInput`
         * @throws `SyntaxError` if missing any of `"`
         */
        std::string parseStringLiteral();
        /**
         * @returns std::nullopt if find no attribute 
         * @throws `UnexpectedEndOfInput`
         * @throws `SyntaxError` if the following chars do not confront to `key="value"` format
         */
        std::optional<std::pair<std::string, std::string>> parseAttribute();
        std::shared_ptr<Text> parseText();
        // return the entire element
        // will consume buffer if failed
        std::shared_ptr<Element> parseElementWithHeader(ElementTag header);
        /**
         * @returns std::nullopt if not starts with `<?xml`
         * @throws `SyntaxError`
         * @throws `SemanticError`
         */
        std::optional<Declaration> parseDeclaration();

    public:
        // return and consume current element
        // will consume buffer if failed
        std::optional<std::shared_ptr<Element>> ParseElement();
        /**
         * @returns std::nullopt if no heading `<`
         * @throws `SyntaxError` if the heading character is `<` and the trailing characters are in incorrect format 
         */
        std::optional<ElementTag> ParseTag();
        Document ParseDocument();
        Parser() = delete;
        explicit Parser(std::string_view);
    };

    namespace util
    {
        bool isValidXmlChar(char ch);
    }
}
