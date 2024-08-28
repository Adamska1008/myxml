#include <algorithm>
#include <unordered_map>
#include "text.hpp"

namespace myxml
{
    NodeType Text::Type()
    {
        return NodeType::Text;
    }

    bool Text::IsType(NodeType type)
    {
        return type == NodeType::Text;
    }

    std::optional<std::shared_ptr<Element>> Text::AsElement()
    {
        return std::nullopt;
    }

    std::optional<std::shared_ptr<Text>> Text::AsText()
    {
        Text *test = dynamic_cast<Text *>(this);
        return std::dynamic_pointer_cast<Text>(this->shared_from_this());
    }

    Text::Text(std::string_view input)
        : encodeOnExport(true)
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

    bool Text::IsAllSpace() const
    {
        return std::all_of(this->inner.begin(), this->inner.end(), isspace);
    }

    void Text::SetEntityEncoding(bool flag)
    {
        this->encodeOnExport = flag;
    }

    std::string Text::ExportRaw() const
    {
        if (!this->encodeOnExport)
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
                if (auto it = entityMap.find(this->inner[i]); it != entityMap.end())
                {
                    builder += this->inner.substr(start, i - start);
                    builder += it->second;
                    start = i + 1;
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
}
