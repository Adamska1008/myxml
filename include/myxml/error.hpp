#pragma once
#include <stdexcept>

namespace myxml
{
    class parse_error : public std::exception
    {
    private:
        virtual const char *prefix() const = 0;

    protected:
        std::string message;
        // store message after being concated with prefix
        mutable std::string fullMessage;
        std::size_t line;
        std::size_t column;

    public:
        parse_error(std::string message, std::size_t line, std::size_t column);

        virtual const char *what() const noexcept override;
    };

    /**
     * The input data do not conform to the expected grammar rule. Including:
     *  1. Missing or mismatch symbols. For example, missing a '>' in the end of a tag.
     *  2. Unexpected token. Encounter a token that is not expected in the context. For example: extra semicolon.
     *  ...
     */
    class syntax_error : public parse_error
    {
    private:
        virtual const char *prefix() const;

    public:
        syntax_error(std::string, std::size_t line, std::size_t column);
    };

    /**
     *
     */
    class semantic_error : public parse_error
    {
    private:
        virtual const char *prefix() const;

    public:
        semantic_error(std::string, std::size_t line, std::size_t column);
    };

    /**
     * e.g. EOF
     */
    class unexpected_eof : public parse_error
    {
    private:
        virtual const char *prefix() const;

    public:
        unexpected_eof(std::size_t line, std::size_t column);
    };

    class io_error : public std::exception
    {
    private:
        std::string message;

    public:
        io_error(std::string);

        virtual const char *what() const noexcept override;
    };
}