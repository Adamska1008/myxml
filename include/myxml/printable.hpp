#pragma once
#include <string>
#include <optional>

namespace myxml
{
    struct format_config
    {
        std::size_t indent_level;
        std::size_t indent_size;

        format_config deeper() { return {indent_level + 1, indent_size}; }
    };

    struct print_config
    {
    public:
        bool entity_encoding;
        bool platform_specific_newline;
        std::optional<format_config> fconfig;
        print_config();
    };

    class printable
    {
    protected:
        print_config config;

    public:
        virtual ~printable() = default;
        virtual void print(std::ostream &os) const = 0;
        // virtual std::string ExportRaw() const = 0;
        // virtual std::string ExportFormatted(int indentLevel = 0, int indentSize = 4) const = 0;
        virtual void entity_encoding(bool);
        virtual void platform_specific_newline(bool);

        friend std::ostream &operator<<(std::ostream &os, const printable &obj);
        std::string str();
    };
}