#pragma once
#include <memory>
#include <string_view>

namespace myxml
{
    class XMLFile
    {
    private:
        XMLFile();

        int fd;
        void *mappedRegion;

    public:
        static XMLFile Open(std::string_view fpath);
        ~XMLFile();
    };
}