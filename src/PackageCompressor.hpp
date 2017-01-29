#ifndef GELF_LIB_PACKAGE_COMPRESSOR_HPP
#define GELF_LIB_PACKAGE_COMPRESSOR_HPP

#include "Data.hpp"

namespace gelf
{
    class PackageCompressor
    {
    public:
        gelf::Data compress( const gelf::Data& rData );
    };
}

#endif //GELF_LIB_PACKAGE_COMPRESSOR_HPP