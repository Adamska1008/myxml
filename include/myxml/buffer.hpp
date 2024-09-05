#pragma once
#include <variant>
#include <string>
#include <string_view>
#include <optional>
#include <tuple>

namespace myxml
{
    /**
     * ADT. Used by parser.
     * Implement by StringBuffer and XMLFile
     */
    class buffer
    {
    private:
        std::size_t offset = 0;
        std::size_t line = 0;
        std::size_t column = 0;

        // @returns {pointer to data, data length}
        virtual std::tuple<const char *, std::size_t> base() const = 0;
        // update line and column
        void update_loc(char);
        // update line and column
        void update_loc(std::string_view);

    public:
        virtual ~buffer() = default;
        std::optional<char> peek() const;
        std::optional<std::string_view> peek_n(int) const;
        std::optional<char> after_n(int) const;
        std::optional<std::string_view> after_n_m(int, int) const;
        std::optional<char> take();
        std::optional<std::string_view> take_n(int);
        // @returns {line, column}
        std::tuple<std::size_t, std::size_t> cur_loc();
    };

    class string_buffer : public buffer
    {
    private:
        std::variant<std::string, std::string_view> inner;
        std::string_view view() const;

        /** Implement Buffer */
        virtual std::tuple<const char *, std::size_t> base() const override;

    public:
        string_buffer(std::string_view);
        string_buffer(std::string &&);
        string_buffer(const char *);
    };
}