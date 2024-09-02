#include <fmt/core.h>
#include "myxml/error.hpp"

namespace myxml
{
    ParseError::ParseError(std::string message, std::size_t line, std::size_t column)
        : message(message), line(line), column(column)
    {
    }

    const char *ParseError::what() const noexcept
    {
        this->fullMessage = fmt::format("{}{}\nin line: {} column: {}",
                                        this->prefix(), this->message, this->line, this->column);
        return this->fullMessage.c_str();
    }

    const char *SyntaxError::prefix() const
    {
        return "Syntax Error: ";
    }

    SyntaxError::SyntaxError(std::string, std::size_t line, std::size_t column)
        : ParseError(message, line, column)
    {
    }

    const char *SemanticError::prefix() const
    {
        return "Sematic Error: ";
    }

    SemanticError::SemanticError(std::string, std::size_t line, std::size_t column)
        : ParseError(message, line, column)
    {
    }

    const char *UnexpectedEndOfInput::prefix() const
    {
        return "Unexpected End of Input: ";
    }

    UnexpectedEndOfInput::UnexpectedEndOfInput(std::size_t line, std::size_t column)
        : ParseError("End of input", line, column)
    {
    }

    IOError::IOError(std::string message)
        : message(message)
    {
    }

    const char *IOError::what() const noexcept
    {
        return message.c_str();
    }
}