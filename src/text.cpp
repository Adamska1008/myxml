#include <algorithm>
#include <unordered_map>
#include "myxml/text.hpp"

namespace myxml
{

    Text::Text(std::string_view input)
    {
        if (config.EntityEncoding)
        {
            // entity encoding
            static std::map<std::string, char, std::less<>> entityMap = {
                {"&lt;", '<'},
                {"&gt;", '>'},
                {"&amp;", '&'},
                {"&quot;", '"'},
                {"&apos;", '\''},
            };
            std::size_t len = input.length();
            std::size_t start = 0; // start of current segment

            for (std::size_t i = 0; i < len; i++)
            {
                // Newline Normalization
                if (input[i] == '\r')
                {
                    this->inner += input.substr(start, i - start);
                    if (i + 1 < len && input[i + 1] == '\n')
                    {
                        i += 1;
                    }
                    this->inner += '\n';
                    start = i + 1;
                }
                // Entity Decoding
                if (input[i] == '&')
                {
                    if (auto semicolonPos = input.find(';', i); semicolonPos != std::string::npos)
                    {
                        std::string_view entity = input.substr(i, semicolonPos - i + 1);
                        if (auto it = entityMap.find(entity); it != entityMap.end())
                        {
                            this->inner += input.substr(start, i - start); // append unmodified segment
                            this->inner += it->second;                     // append decoded character
                            i = semicolonPos;                              // skip past the entity
                            start = semicolonPos + 1;                      // update last unappend position
                        }
                    }
                }
            }
            this->inner += input.substr(start, len - start); // append the remaining
        }
        else
        {
            this->inner = input;
        }
    }

    bool Text::IsAllSpace() const
    {
        return std::all_of(this->inner.begin(), this->inner.end(), isspace);
    }

    std::string Text::ExportRaw() const
    {
        if (!this->config.EntityEncoding && !this->config.PlatformSpecificNewline)
        {
            return this->inner;
        }
        else
        {
            static std::unordered_map<char, std::string> entityMap = {
                {'<', "&lt;"},
                {'>', "&gt;"},
                {'&', "&amp;"},
                {'"', "&quot;"},
                {'\'', "&apos;"},
            };
            std::size_t start = 0; // start of current segement
            std::size_t len = this->inner.length();
            std::string builder;
            for (std::size_t i = 0; i < len; i++)
            {
                if (this->config.EntityEncoding)
                {
                    if (auto it = entityMap.find(this->inner[i]); it != entityMap.end())
                    {
                        builder += this->inner.substr(start, i - start);
                        builder += it->second;
                        start = i + 1;
                    }
                }
                if (this->config.PlatformSpecificNewline)
                {
                    if (this->inner[i] == '\n')
                    {
                        builder += this->inner.substr(start, i - start);
                        builder += util::platformSpecificNewline();
                        start = i + 1;
                    }
                }
            }
            builder += this->inner.substr(start, len - start);
            return builder;
        }
    }

    std::string Text::ExportFormatted(int indentLevel, int indentSize) const
    {
        // TODO: better implementation
        return std::string(indentLevel * indentSize, ' ') + this->inner + '\n';
    }

    namespace util
    {
        const char *const platformSpecificNewline()
        {
#ifdef _WIN32
            return "\r\n";
#else
            return "\n";
#endif
        }
    }
}
