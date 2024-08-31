#include "myxml/buffer.hpp"

namespace myxml
{
    StringBuffer::StringBuffer(std::string_view inner)
        : inner(inner), offset(0)
    {
    }

    StringBuffer::StringBuffer(std::string &&inner)
        : inner(inner), offset(0)
    {
    }

    std::optional<char> StringBuffer::Peek() const
    {
        auto view = this->getView();
        if (offset >= view.length())
        {
            return std::nullopt;
        }
        return view[this->offset];
    }

    std::optional<std::string_view> StringBuffer::PeekN(int n) const
    {
        auto view = this->getView();
        if (offset + n > view.length())
        {
            return std::nullopt;
        }
        return view.substr(this->offset, n);
    }

    std::optional<char> StringBuffer::AfterN(int n) const
    {
        auto view = this->getView();
        if (offset + n > view.length())
        {
            return std::nullopt;
        }
        return view[this->offset + n];
    }

    std::optional<std::string_view> StringBuffer::AfterNM(int n, int m) const
    {
        auto view = this->getView();
        if (offset + n + m > view.length())
        {
            return std::nullopt;
        }
        return view.substr(offset + n, m);
    }

    std::optional<char> StringBuffer::Take()
    {
        auto view = this->getView();
        if (offset >= view.length())
        {
            return std::nullopt;
        }
        return view[this->offset++];
    }

    std::optional<std::string_view> StringBuffer::TakeN(int n)
    {
        auto view = this->getView();
        if (offset + n >= view.length())
        {
            return std::nullopt;
        }
        std::string_view it = view.substr(offset, n);
        offset += n;
        return it;
    }

    std::string_view StringBuffer::getView() const
    {
        if (std::holds_alternative<std::string>(this->inner))
        {
            return std::string_view(std::get<std::string>(this->inner));
        }
        else
        {
            return std::get<std::string_view>(this->inner);
        }
    }
}
