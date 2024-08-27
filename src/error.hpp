#pragma once
#include <stdexcept>

namespace myxml
{
    class ParseError : public std::exception
    {
    private:
        virtual const char *prefix() const = 0;

    protected:
        std::string message;
        // store message after being concated with prefix
        mutable std::string fullMessage;

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
        virtual const char *prefix() const;

    public:
        SyntaxError(std::string);
    };

    /**
     *
     */
    class SemanticError : public ParseError
    {
    private:
        virtual const char *prefix() const;

    public:
        SemanticError(std::string);
    };

    /**
     * e.g. EOF
     */
    class UnexpectedEndOfInput : public ParseError
    {
    private:
        virtual const char *prefix() const;

    public:
        UnexpectedEndOfInput();
    };
}