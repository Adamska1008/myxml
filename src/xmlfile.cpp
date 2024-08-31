// TODO: support windows

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "myxml/xmlfile.hpp"
#include "myxml/error.hpp"

namespace myxml
{
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
        std::size_t fileSize = fileInfo.st_size;
        xfile.mappedRegion = mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, xfile.fd, 0);
        if (xfile.mappedRegion == MAP_FAILED)
        {
            throw IOError();
        }
    }

    XMLFile::~XMLFile()
    {
    }
}