#pragma once
#include <memory>
#include <string_view>
#include <optional>
#include "myxml/buffer.hpp"

namespace myxml
{
    class xml_file : public buffer
    {
    private:
        xml_file();

        /* mmap related*/
        int _fd;
        std::size_t _size;
        char *_mapped;
        std::size_t _offset;

        /* Implement buffer*/
        virtual std::tuple<const char *, std::size_t> base() const;

    public:
        static std::shared_ptr<xml_file> open(std::string_view fpath);
        // RAII
        ~xml_file();
        // always copy shared_ptr instead of XMLFile
        xml_file(const xml_file &) = delete;
    };
}