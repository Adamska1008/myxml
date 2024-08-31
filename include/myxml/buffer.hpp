#pragma once
#include <variant>
#include <string>
#include <string_view>
#include <optional>

namespace myxml
{
    /**
     * ADT. Used by Parser. Parser will consume the source.
     * Implement by StringBuffer and XMLFile
     */
    class Buffer
    {
    public:
        virtual ~Buffer() = default;
        virtual std::optional<char> Peek() const = 0;
        virtual std::optional<std::string_view> PeekN(int) const = 0;
        virtual std::optional<char> AfterN(int) const = 0;
        virtual std::optional<std::string_view> AfterNM(int, int) const = 0;
        virtual std::optional<char> Take() = 0;
        virtual std::optional<std::string_view> TakeN(int) = 0;
    };

    class StringBuffer : public Buffer
    {
    private:
        using stringVariant = std::variant<std::string, std::string_view>;

        stringVariant inner;
        std::size_t offset;
        std::string_view getView() const;

    public:
        StringBuffer(std::string_view);
        StringBuffer(std::string &&);

        virtual std::optional<char> Peek() const override;
        virtual std::optional<std::string_view> PeekN(int) const override;
        virtual std::optional<char> AfterN(int) const override;
        virtual std::optional<std::string_view> AfterNM(int, int) const override;
        virtual std::optional<char> Take() override;
        virtual std::optional<std::string_view> TakeN(int) override;
    };
}