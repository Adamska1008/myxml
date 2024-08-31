#pragma once
#include <variant>
#include <string>
#include <string_view>
#include <optional>
#include <tuple>

namespace myxml
{
    /**
     * ADT. Used by Parser. Parser will consume the source.
     * Implement by StringBuffer and XMLFile
     */
    class Buffer
    {
    private:
        std::size_t offset = 0;

        virtual std::tuple<const char *, std::size_t> base() const = 0;

    public:
        virtual ~Buffer() = default;
        virtual std::optional<char> Peek() const;
        virtual std::optional<std::string_view> PeekN(int) const;
        virtual std::optional<char> AfterN(int) const;
        virtual std::optional<std::string_view> AfterNM(int, int) const;
        virtual std::optional<char> Take();
        virtual std::optional<std::string_view> TakeN(int);
    };

    class StringBuffer : public Buffer
    {
    private:
        using stringVariant = std::variant<std::string, std::string_view>;

        stringVariant inner;
        std::string_view getView() const;

        /** Implement Buffer */
        virtual std::tuple<const char *, std::size_t> base() const override;

    public:
        StringBuffer(std::string_view);
        StringBuffer(std::string &&);
    };
}