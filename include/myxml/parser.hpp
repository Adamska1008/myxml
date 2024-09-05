#pragma once
#include "myxml/element.hpp"
#include "myxml/document.hpp"
#include "myxml/cdata.hpp"
#include "myxml/buffer.hpp"

namespace myxml
{
    struct element_tag
    {
        enum class closing_type
        {
            Open,   // <tag>
            Closed, // <tag/>
            Closing // </tag>
        };

        std::string name;
        element_tag::closing_type type = closing_type::Open;
        std::map<std::string, std::string> attrs;
    };

    class parser
    {
    private:
        std::shared_ptr<buffer> _buffer;

        std::optional<char> peek();
        std::optional<std::string_view> peek_n(int);
        std::optional<char> after_n(int);
        // m characters after n characters
        std::optional<std::string_view> after_n_m(int, int);
        std::optional<char> take();
        std::optional<std::string_view> take_n(int);
        void skip_whitespaces();
        std::tuple<std::size_t, std::size_t> cur_loc();

    public:
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
        std::string parse_ident();
        /**
         * Parse a string literal
         * @throws `UnexpectedEndOfInput`
         * @throws `SyntaxError` if missing any of `"`
         */
        std::string parse_str_literal();
        /**
         * @returns std::nullopt if find no attribute
         * @throws `UnexpectedEndOfInput`
         * @throws `SyntaxError` if the following chars do not confront to `key="value"` format
         */
        std::optional<std::pair<std::string, std::string>> parse_attribute();
        /**
         * @throws `SyntaxError` if faild to find `<`
         */
        std::shared_ptr<text_impl> parse_text();
        /**
         * @returns `std::nullopt` if not start with `<!CDATA[`
         */
        std::shared_ptr<cdata_impl> parse_cdata();
        /**
         * @throws `UnexpectedEndOfInput`
         * @throws `SyntaxError`
         * @throws `SemanticError`
         */
        std::shared_ptr<element_impl> parse_element_with_header(element_tag header);
        /**
         * @returns std::nullopt if not starts with `<?xml`
         * @throws `UnexpectedEndOfInput`
         * @throws `SyntaxError`
         * @throws `SemanticError`
         */
        std::optional<declaration> parse_declaration();

        std::shared_ptr<element_impl> parse_element();
        /**
         * @returns std::nullopt if no heading `<`
         * @throws `SyntaxError` if the heading character is `<` and the trailing characters are in incorrect format
         * @throws `UnexpectedEndOfInput` if missing name
         */
        std::optional<element_tag> parse_element_tag();
        /**
         * @throws `UnexpectedEndOfInput`
         * @throws `SyntaxError`
         * @throws `SemanticError`
         */
        document parse_document();
        parser() = delete;
        explicit parser(std::string_view);
        explicit parser(std::string &&);

        template <typename T, typename = std::enable_if_t<std::is_base_of_v<buffer, T>>>
        explicit parser(std::shared_ptr<T> buffer)
            : _buffer(buffer) {}
    };

    namespace util
    {
        bool is_valid_xml_char(char ch);
    }
}
