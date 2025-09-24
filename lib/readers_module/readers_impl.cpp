#include "readers_impl.hpp"
#include "logger.h"
#include <stdexcept>


#include <FS.h>   
#include <LittleFS.h> 
namespace atl
{
    LocalFileReader::LocalFileReader(const File& path)
        : FileReaderBase(file_)
    {
        if (!LittleFS.begin()) {
            Logger::log_error("Filesystem mount failed");
            throw std::runtime_error("Filesystem not mounted");
        }
        Logger::log_info("Filesystem mounted successfully");
    }

    std::string LocalFileReader::read() {
        fs::File file = LittleFS.open(file_.file_path.c_str(), "r");
        if (!file) {
            Logger::log_error("Failed to open file: " + std::string(file_.file_path));
            throw std::runtime_error("Failed to open file");
        }

        size_t size = file.size();
        if (size == 0) {
            Logger::log_warning("Empty file: " + std::string(file_.file_path));
            return nullptr;
        }
        char* buffer = nullptr;

        buffer = new char[size + 1];
        if (!buffer) {
            file.close();
            Logger::log_error("Memory allocation failed");
            throw std::runtime_error("Memory allocation failed");
        }

        file.readBytes(buffer, size);
        buffer[size] = '\0';


        std::string file_content(buffer);

        Logger::log_info("File read successfully: " + file_.file_path);


        if (buffer) 
        {
            delete[] buffer;
        }
        
        file.close();
        return file_content;
    }
}

std::string read_file_content(const atl::File& file_info){
    
    atl::FileReaderBase* reader;
    
    if(file_info.file_type == atl::FileType::LOCAL_FILE)
    {
        reader = new atl::LocalFileReader(file_info);
    }

    return reader->read();
}

