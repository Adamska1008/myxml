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
    xml_file::xml_file()
        : _offset(0)
    {
    }

    std::shared_ptr<xml_file> xml_file::open(std::string_view fpath)
    {
        // can't use make_shared because XMLFile() is private
        auto xfile = std::shared_ptr<xml_file>(new xml_file());
        xfile->_fd = open(fpath.data(), O_RDONLY);
        if (xfile->_fd == -1)
        {
            throw io_error(fmt::format("failed to open file: {}", fpath));
        }
        struct stat fileInfo;
        if (fstat(xfile->_fd, &fileInfo) == -1)
        {
            throw io_error(fmt::format("failed to get info of file: {}", fpath));
        }
        xfile->_size = fileInfo.st_size;
        void *mappedRegion = mmap(nullptr, xfile->_size, PROT_READ, MAP_PRIVATE, xfile->_fd, 0);
        if (mappedRegion == MAP_FAILED)
        {
            throw io_error(fmt::format("failed to map memory for file: {}", fpath));
        }
        xfile->_mapped = static_cast<char *>(mappedRegion);
        return xfile;
    }

    xml_file::~xml_file()
    {
        close(this->_fd);
        munmap(static_cast<void *>(this->_mapped), this->_size);
    }

    std::tuple<const char *, std::size_t> xml_file::base() const
    {
        return {this->_mapped, this->_size};
    }
}