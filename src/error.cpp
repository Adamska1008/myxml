#include <fmt/core.h>
#include "myxml/error.hpp"

namespace myxml
{
    parse_error::parse_error(std::string message, std::size_t line, std::size_t column)
        : message(message), line(line), column(column)
    {
    }

    const char *parse_error::what() const noexcept
    {
        this->fullMessage = fmt::format("{}{}\nin line: {} column: {}",
                                        this->prefix(), this->message, this->line, this->column);
        return this->fullMessage.c_str();
    }

    const char *syntax_error::prefix() const
    {
        return "Syntax Error: ";
    }

    syntax_error::syntax_error(std::string, std::size_t line, std::size_t column)
        : parse_error(message, line, column)
    {
    }

    const char *semantic_error::prefix() const
    {
        return "Sematic Error: ";
    }

    semantic_error::semantic_error(std::string, std::size_t line, std::size_t column)
        : parse_error(message, line, column)
    {
    }

    const char *unexpected_eof::prefix() const
    {
        return "Unexpected End of Input: ";
    }

    unexpected_eof::unexpected_eof(std::size_t line, std::size_t column)
        : parse_error("End of input", line, column)
    {
    }

    io_error::io_error(std::string message)
        : message(message)
    {
    }

    const char *io_error::what() const noexcept
    {
        return message.c_str();
    }
}