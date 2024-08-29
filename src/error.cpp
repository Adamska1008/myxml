#include "myxml/error.hpp"

namespace myxml
{
    ParseError::ParseError(std::string message)
        : message(message)
    {
    }

    const char *ParseError::what() const noexcept
    {
        this->fullMessage = this->prefix() + this->message;
        return this->fullMessage.c_str();
    }

    const char *SyntaxError::prefix() const
    {
        return "Syntax Error: ";
    }

    SyntaxError::SyntaxError(std::string message)
        : ParseError(message)
    {
    }

    const char *SemanticError::prefix() const
    {
        return "Sematic Error: ";
    }

    SemanticError::SemanticError(std::string message)
        : ParseError(message)
    {
    }

    const char *UnexpectedEndOfInput::prefix() const
    {
        return "Unexpected End of Input: ";
    }

    UnexpectedEndOfInput::UnexpectedEndOfInput()
        : ParseError("End of input")
    {
    }
}