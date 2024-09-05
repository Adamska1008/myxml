#include "myxml/buffer.hpp"

namespace myxml
{
    void buffer::update_loc(char ch)
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
        if (this->offset >= len)
        {
            return std::nullopt;
        }
        return ptr[this->offset];
    }

    std::optional<std::string_view> buffer::peek_n(int n) const
    {
        auto [ptr, len] = this->base();
        if (this->offset >= len)
        {
            return std::nullopt;
        }
        return std::string_view(ptr + this->offset, n);
    }

    std::optional<char> buffer::after_n(int n) const
    {
        auto [ptr, len] = this->base();
        if (this->offset + n > len)
        {
            return std::nullopt;
        }
        return ptr[this->offset + n];
    }

    std::optional<std::string_view> buffer::after_n_m(int n, int m) const
    {
        auto [ptr, len] = this->base();
        if (this->offset + n + m > len)
        {
            return std::nullopt;
        }
        return std::string_view(ptr + this->offset + n, m);
    }

    std::optional<char> buffer::take()
    {
        auto [ptr, len] = this->base();
        if (this->offset >= len)
        {
            return std::nullopt;
        }
        auto ch = ptr[this->offset++];
        this->update_loc(ch);
        return ch;
    }

    std::optional<std::string_view> buffer::take_n(int n)
    {
        auto [ptr, len] = this->base();
        if (offset + n >= len)
        {
            return std::nullopt;
        }
        std::string_view strv(ptr + this->offset, n);
        this->update_loc(strv);
        offset += n;
        return strv;
    }

    std::tuple<std::size_t, std::size_t> buffer::cur_loc()
    {
        return {this->line, this->column};
    }

    string_buffer::string_buffer(std::string_view inner)
        : inner(inner)
    {
    }

    string_buffer::string_buffer(std::string &&inner)
        : inner(inner)
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
