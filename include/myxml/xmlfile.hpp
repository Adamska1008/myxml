#pragma once
#include <memory>
#include <string_view>
#include <optional>
#include "myxml/buffer.hpp"

namespace myxml
{
    class XMLFile : public Buffer
    {
    private:
        XMLFile();

        /* Mmap related*/
        int fd;
        std::size_t fileSize;
        char *inner;

        std::size_t offset;

        /* Implement Buffer*/
        virtual std::tuple<const char *, std::size_t> base() const;

    public:
        static XMLFile Open(std::string_view fpath);
        ~XMLFile();
    };
}