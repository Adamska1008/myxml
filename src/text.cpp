#include <algorithm>
#include <unordered_map>
#include "myxml/text.hpp"

namespace myxml
{
    text::text(std::shared_ptr<text_impl> impl)
        : _impl(impl)
    {
    }

    text::text(std::string &&str)
    {
        _impl = std::make_shared<text_impl>();
        _impl->inner = str;
    }

    text::text(std::string_view str)
    {
        _impl = std::make_shared<text_impl>();
        _impl->inner = str;
    }

    void text::print(std::ostream &os) const
    {
        this->_impl->print(os);
    }

    void text::entity_encoding(bool flag)
    {
        this->_impl->entity_encoding(flag);
    }

    void text::platform_specific_newline(bool flag)
    {
        this->_impl->platform_specific_newline(flag);
    }

    text_impl::text_impl(std::string_view input)
    {
        if (_config.entity_encoding)
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

    void text_impl::print(std::ostream &os) const
    {
        if (!this->_config.entity_encoding && !this->_config.platform_specific_newline)
        {
            os << this->inner;
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
            for (std::size_t i = 0; i < len; i++)
            {
                if (this->_config.entity_encoding)
                {
                    if (auto it = entityMap.find(this->inner[i]); it != entityMap.end())
                    {
                        os << this->inner.substr(start, i - start);
                        os << it->second;
                        start = i + 1;
                    }
                }
                if (this->_config.platform_specific_newline)
                {
                    if (this->inner[i] == '\n')
                    {
                        os << this->inner.substr(start, i - start);
                        os << util::platform_specific_newline();
                        start = i + 1;
                    }
                }
            }
            os << this->inner.substr(start, len - start);
        }
    }

    namespace util
    {
        const std::string_view platform_specific_newline()
        {
#ifdef _WIN32
            return "\r\n";
#else
            return "\n";
#endif
        }
    }
}
