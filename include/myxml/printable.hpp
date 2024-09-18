#pragma once
#include <string>
#include <optional>
#include <variant>

namespace myxml
{
    struct compacted
    {
    };

    struct formatted
    {
        int indent_size;
        int indent_level;

        formatted deeper() { return {indent_size, indent_level + 1}; }
    };

    using print_style = std::variant<compacted, formatted>;

    struct print_config
    {
    public:
        bool entity_encoding;
        bool platform_specific_newline;
        print_style style;
        print_config();
    };

    class printable
    {
    protected:
        print_config _print_config;

    public:
        virtual ~printable() = default;
        virtual void print(std::ostream &os) const = 0;
        virtual void entity_encoding(bool);
        virtual void platform_specific_newline(bool);

        friend std::ostream &operator<<(std::ostream &os, const printable &obj);
        std::string str();
    };
}