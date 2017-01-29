#include "PackageCompressor.hpp"

#include <zlib.h>

gelf::Data gelf::PackageCompressor::compress( const gelf::Data & rData )
{
    gelf::Data compressedData;

    uLong uMaxSize = compressBound( static_cast< uLong >( rData.m_uSize ) );
    compressedData.m_pData = new uint8_t[ uMaxSize ];

    if( compress2( compressedData.m_pData, &uMaxSize, rData.m_pData, static_cast< uLong >( rData.m_uSize ), Z_BEST_COMPRESSION ) == Z_OK )
    {
        compressedData.m_uSize = static_cast< size_t >( uMaxSize );
        return compressedData;
    }

    return gelf::Data();
}
