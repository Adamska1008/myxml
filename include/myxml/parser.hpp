#pragma once
#include "myxml/element.hpp"
#include "myxml/document.hpp"

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

    class Parser
    {
    private:
        std::string buffer;
        std::size_t offset;

        void skipWhiteSpaces();
        std::optional<char> peekChar();
        std::optional<std::string> peekNextNChars(int);
        std::optional<char> nextChar();
        std::optional<std::string> nextNChars(int);

        /**
         * For all parsing method,
         * return std::nullopt means `not this one` and will not consume buffer.
         * Throw exception means `parsing error` and should stop immediately
         */

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
        /**
         * @throws `UnexpectedEndOfInput`
         * @throws `SyntaxError`
         * @throws `SemanticError`
         */
        std::shared_ptr<Element> parseElementWithHeader(ElementTag header);
        /**
         * @returns std::nullopt if not starts with `<?xml`
         * @throws `UnexpectedEndOfInput`
         * @throws `SyntaxError`
         * @throws `SemanticError`
         */
        std::optional<Declaration> parseDeclaration();

    public:
        std::optional<std::shared_ptr<Element>> ParseElement();
        /**
         * @returns std::nullopt if no heading `<`
         * @throws `SyntaxError` if the heading character is `<` and the trailing characters are in incorrect format
         * @throws `UnexpectedEndOfInput` if missing name
         */
        std::optional<ElementTag> ParseTag();
        /**
         * @throws `UnexpectedEndOfInput`
         * @throws `SyntaxError`
         * @throws `SemanticError`
         */
        Document ParseDocument();
        Parser() = delete;
        explicit Parser(std::string_view);
    };

    namespace util
    {
        bool isValidXmlChar(char ch);
    }
}