#pragma once
#include <string>
#include "logger.h"
namespace atl
{

    enum FileType
    {
        LOCAL_FILE
    };

    struct File
    {
        std::string file_path;
        atl::FileType file_type;
    };

    class FileReaderBase
    {
    public:
        FileReaderBase(const File& file): file_(file)
        {
            Logger::log_debug("FileReaderBase::FileReaderBase is runned .");
        }
        virtual std::string read() = 0;

    protected:
        atl::File file_;

    };

}
std::string read_file_content(const atl::File &);



