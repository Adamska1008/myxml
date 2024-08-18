#pragma once
#include "element.hpp"

namespace myxml
{
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
    };

    namespace util
    {
        bool isValidXmlChar(char ch);
    }
}
