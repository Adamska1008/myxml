#pragma once
#include <memory>
#include <string_view>
#include <optional>
#include "myxml/buffer.hpp"

namespace myxml
{
    class XMLFile : public buffer
    {
    private:
        XMLFile();

        /* mmap related*/
        int fd;
        std::size_t fileSize;
        char *inner;

        std::size_t offset;

        /* Implement buffer*/
        virtual std::tuple<const char *, std::size_t> base() const;

    public:
        static std::shared_ptr<XMLFile> Open(std::string_view fpath);
        // RAII
        ~XMLFile();
        // always copy shared_ptr instead of XMLFile
        XMLFile(const XMLFile &) = delete;
    };
}