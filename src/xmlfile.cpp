// TODO: support windows

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "myxml/xmlfile.hpp"
#include "myxml/error.hpp"

namespace myxml
{
    XMLFile::XMLFile()
        : offset(0)
    {
    }

    XMLFile XMLFile::Open(std::string_view fpath)
    {
        XMLFile xfile;
        xfile.fd = open(fpath.data(), O_RDONLY);
        if (xfile.fd == -1)
        {
            throw IOError();
        }
        struct stat fileInfo;
        if (fstat(xfile.fd, &fileInfo) == -1)
        {
            throw IOError();
        }
        xfile.fileSize = fileInfo.st_size;
        void *mappedRegion = mmap(nullptr, xfile.fileSize, PROT_READ, MAP_PRIVATE, xfile.fd, 0);
        if (mappedRegion == MAP_FAILED)
        {
            throw IOError();
        }
        xfile.inner = static_cast<char *>(mappedRegion);
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