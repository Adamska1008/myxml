#pragma once
#include <variant>
#include <string>
#include <string_view>
#include <optional>
#include <tuple>

namespace myxml
{
    /**
     * ADT. Used by Parser.
     * Implement by StringBuffer and XMLFile
     */
    class Buffer
    {
    private:
        std::size_t offset = 0;
        std::size_t line = 0;
        std::size_t column = 0;

        // @returns {pointer to data, data length}
        virtual std::tuple<const char *, std::size_t> base() const = 0;
        // update line and column
        void updateLocation(char);
        // update line and column
        void updateLocation(std::string_view);

    public:
        virtual ~Buffer() = default;
        std::optional<char> Peek() const;
        std::optional<std::string_view> PeekN(int) const;
        std::optional<char> AfterN(int) const;
        std::optional<std::string_view> AfterNM(int, int) const;
        std::optional<char> Take();
        std::optional<std::string_view> TakeN(int);
        // @returns {line, column}
        std::tuple<std::size_t, std::size_t> CurrentLocation();
    };

    class StringBuffer : public Buffer
    {
    private:
        std::variant<std::string, std::string_view> inner;
        std::string_view getView() const;

        /** Implement Buffer */
        virtual std::tuple<const char *, std::size_t> base() const override;

    public:
        StringBuffer(std::string_view);
        StringBuffer(std::string &&);
        StringBuffer(const char *);
    };
}