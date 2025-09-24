#pragma once

#include "readers_base.hpp"
namespace atl {

    class LocalFileReader : public atl::FileReaderBase
    {
    public:
        LocalFileReader(const atl::File&);
        
        std::string read() override;
    };


}