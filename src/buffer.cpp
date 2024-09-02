#include "myxml/buffer.hpp"

namespace myxml
{
    void Buffer::updateLocation(char ch)
    {
        if (ch == '\n')
        {
            this->column = 0;
            this->line++;
        }
        else
        {
            this->column++;
        }
    }

    void Buffer::updateLocation(std::string_view strv)
    {
        for (auto ch : strv)
        {
            this->updateLocation(ch);
        }
    }

    std::optional<char> Buffer::Peek() const
    {
        auto [ptr, len] = this->base();
        if (this->offset >= len)
        {
            return std::nullopt;
        }
        return ptr[this->offset];
    }

    std::optional<std::string_view> Buffer::PeekN(int n) const
    {
        auto [ptr, len] = this->base();
        if (this->offset >= len)
        {
            return std::nullopt;
        }
        return std::string_view(ptr + this->offset, n);
    }

    std::optional<char> Buffer::AfterN(int n) const
    {
        auto [ptr, len] = this->base();
        if (this->offset + n > len)
        {
            return std::nullopt;
        }
        return ptr[this->offset + n];
    }

    std::optional<std::string_view> Buffer::AfterNM(int n, int m) const
    {
        auto [ptr, len] = this->base();
        if (this->offset + n + m > len)
        {
            return std::nullopt;
        }
        return std::string_view(ptr + this->offset + n, m);
    }

    std::optional<char> Buffer::Take()
    {
        auto [ptr, len] = this->base();
        if (this->offset >= len)
        {
            return std::nullopt;
        }
        auto ch = ptr[this->offset++];
        this->updateLocation(ch);
        return ch;
    }

    std::optional<std::string_view> Buffer::TakeN(int n)
    {
        auto [ptr, len] = this->base();
        if (offset + n >= len)
        {
            return std::nullopt;
        }
        std::string_view strv(ptr + this->offset, n);
        this->updateLocation(strv);
        offset += n;
        return strv;
    }

    std::tuple<std::size_t, std::size_t> Buffer::CurrentLocation()
    {
        return {this->line, this->column};
    }

    StringBuffer::StringBuffer(std::string_view inner)
        : inner(inner)
    {
    }

    StringBuffer::StringBuffer(std::string &&inner)
        : inner(inner)
    {
    }

    StringBuffer::StringBuffer(const char *ptr)
        : StringBuffer(std::string_view(ptr))
    {
    }

    std::tuple<const char *, std::size_t> StringBuffer::base() const
    {
        auto view = this->getView();
        return {view.data(), view.length()};
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
