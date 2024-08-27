#pragma once
#include <stdexcept>

namespace myxml
{
    class ParseError : public std::exception
    {
    protected:
        std::string message;

    public:
        ParseError(std::string message);

        virtual const char *what() const noexcept override;
    };

    /**
     * The input data do not conform to the expected grammar rule. Including:
     *  1. Missing or mismatch symbols. For example, missing a '>' in the end of a tag.
     *  2. Unexpected token. Encounter a token that is not expected in the context. For example: extra semicolon.
     *  ...
     */
    class SyntaxError : public ParseError
    {
    private:
        // store message after being concated
        mutable std::string fullMessage;

    public:
        SyntaxError(std::string);

        virtual const char *what() const noexcept override;
    };

    /**
     * e.g. EOF
     */
    class UnexpectedEndOfInput : public ParseError
    {
    private:
        // store message after being concated
        mutable std::string fullMessage;

    public:
        UnexpectedEndOfInput();

        virtual const char *what() const noexcept override;
    };
}