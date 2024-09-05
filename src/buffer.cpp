#include "myxml/buffer.hpp"

namespace myxml
{
    void buffer::update_loc(char ch)
    {
        if (ch == '\n')
        {
            _column = 0;
            _line++;
        }
        else
        {
            _column++;
        }
    }

    void buffer::update_loc(std::string_view strv)
    {
        for (auto ch : strv)
        {
            this->update_loc(ch);
        }
    }

    std::optional<char> buffer::peek() const
    {
        auto [ptr, len] = this->base();
        if (_offset >= len)
        {
            return std::nullopt;
        }
        return ptr[_offset];
    }

    std::optional<std::string_view> buffer::peek_n(int n) const
    {
        auto [ptr, len] = this->base();
        if (_offset >= len)
        {
            return std::nullopt;
        }
        return std::string_view(ptr + _offset, n);
    }

    std::optional<char> buffer::after_n(int n) const
    {
        auto [ptr, len] = this->base();
        if (_offset + n > len)
        {
            return std::nullopt;
        }
        return ptr[_offset + n];
    }

    std::optional<std::string_view> buffer::after_n_m(int n, int m) const
    {
        auto [ptr, len] = this->base();
        if (_offset + n + m > len)
        {
            return std::nullopt;
        }
        return std::string_view(ptr + _offset + n, m);
    }

    std::optional<char> buffer::take()
    {
        auto [ptr, len] = this->base();
        if (_offset >= len)
        {
            return std::nullopt;
        }
        auto ch = ptr[_offset++];
        this->update_loc(ch);
        return ch;
    }

    std::optional<std::string_view> buffer::take_n(int n)
    {
        auto [ptr, len] = this->base();
        if (_offset + n >= len)
        {
            return std::nullopt;
        }
        std::string_view strv(ptr + _offset, n);
        this->update_loc(strv);
        _offset += n;
        return strv;
    }

    std::tuple<std::size_t, std::size_t> buffer::cur_loc()
    {
        return {_line, _column};
    }

    string_buffer::string_buffer(std::string_view inner)
        : _inner(inner)
    {
    }

    string_buffer::string_buffer(std::string &&inner)
        : _inner(inner)
    {
    }

    string_buffer::string_buffer(const char *ptr)
        : string_buffer(std::string_view(ptr))
    {
    }

    std::tuple<const char *, std::size_t> string_buffer::base() const
    {
        auto view = this->view();
        return {view.data(), view.length()};
    }

    std::string_view string_buffer::view() const
    {
        if (std::holds_alternative<std::string>(_inner))
        {
            return std::string_view(std::get<std::string>(_inner));
        }
        else
        {
            return std::get<std::string_view>(_inner);
        }
    }
}
