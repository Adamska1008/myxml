#include "error.hpp"

namespace myxml
{
    ParseError::ParseError(std::string message)
        : message(message)
    {
    }

    const char *ParseError::what() const noexcept
    {
        return this->message.c_str();
    }

    SyntaxError::SyntaxError(std::string message)
        : ParseError(message)
    {
    }

    const char *SyntaxError::what() const noexcept
    {
        this->fullMessage = "SyntaxError: " + std::string(ParseError::what());
        return this->fullMessage.c_str();
    }

    UnexpectedEndOfInput::UnexpectedEndOfInput()
        : ParseError("End of input")
    {
    }

    const char *UnexpectedEndOfInput::what() const noexcept
    {
        this->fullMessage = "UnexpectedEndOfInput: " + std::string(ParseError::what());
        return this->fullMessage.c_str();
    }
}