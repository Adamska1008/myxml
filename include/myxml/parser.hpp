#pragma once
#include "myxml/element.hpp"
#include "myxml/document.hpp"
#include "myxml/cdata.hpp"
#include "myxml/buffer.hpp"

namespace myxml
{
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
        std::shared_ptr<Buffer> buffer;

        std::optional<char> peek();
        std::optional<std::string_view> peekN(int);
        std::optional<char> afterN(int);
        // m characters after n characters
        std::optional<std::string_view> afterNM(int, int);
        std::optional<char> take();
        std::optional<std::string_view> takeN(int);
        void skipWhiteSpaces();
        std::tuple<std::size_t, std::size_t> currentLoc();

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
        /**
         * @throws `SyntaxError` if faild to find `<`
         */
        std::shared_ptr<Text> parseText();
        /**
         * @returns `std::nullopt` if not start with `<!CDATA[`
         */
        std::shared_ptr<CData> parseCData();
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
        std::optional<declaration> parseDeclaration();

    public:
        std::shared_ptr<Element> ParseElement();
        /**
         * @returns std::nullopt if no heading `<`
         * @throws `SyntaxError` if the heading character is `<` and the trailing characters are in incorrect format
         * @throws `UnexpectedEndOfInput` if missing name
         */
        std::optional<ElementTag> ParseElementTag();
        /**
         * @throws `UnexpectedEndOfInput`
         * @throws `SyntaxError`
         * @throws `SemanticError`
         */
        document ParseDocument();
        Parser() = delete;
        explicit Parser(std::string_view);
        explicit Parser(std::string &&);

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<Buffer, T>>>
        explicit Parser(std::shared_ptr<T> buffer)
            : buffer(buffer) {}
    };

    namespace util
    {
        bool isValidXmlChar(char ch);
    }
}
