// TODO: support windows
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fmt/core.h>

#include "myxml/xmlfile.hpp"
#include "myxml/error.hpp"

namespace myxml
{
    XMLFile::XMLFile()
        : offset(0)
    {
    }

    std::shared_ptr<XMLFile> XMLFile::Open(std::string_view fpath)
    {
        // can't use make_shared because XMLFile() is private
        auto xfile = std::shared_ptr<XMLFile>(new XMLFile());
        xfile->fd = open(fpath.data(), O_RDONLY);
        if (xfile->fd == -1)
        {
            throw io_error(fmt::format("failed to open file: {}", fpath));
        }
        struct stat fileInfo;
        if (fstat(xfile->fd, &fileInfo) == -1)
        {
            throw io_error(fmt::format("failed to get info of file: {}", fpath));
        }
        xfile->fileSize = fileInfo.st_size;
        void *mappedRegion = mmap(nullptr, xfile->fileSize, PROT_READ, MAP_PRIVATE, xfile->fd, 0);
        if (mappedRegion == MAP_FAILED)
        {
            throw io_error(fmt::format("failed to map memory for file: {}", fpath));
        }
        xfile->inner = static_cast<char *>(mappedRegion);
        return xfile;
    }

    XMLFile::~XMLFile()
    {
        close(this->fd);
        munmap(static_cast<void *>(this->inner), this->fileSize);
    }

    std::tuple<const char *, std::size_t> XMLFile::base() const
    {
        return {this->inner, this->fileSize};
    }
}