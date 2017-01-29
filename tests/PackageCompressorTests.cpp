#include "gtest/gtest.h"

#include "PackageCompressor.hpp"

namespace tests
{
    TEST( PackageCompressorTests, SimpleCompression )
    {
        gelf::Data data( reinterpret_cast< uint8_t* >( (char*) "kC904jMCZWJe8rsF9vqQXTLr69fqPSB7BTvLwofQQwU2AMT1hcmUHujl1GPSASE9kC904jMCZWJe8rsF9vqQXTLr69fqPSB7BTvLwofQQwU2AMT1hcmUHujl1GPSASE9" ), 128 );
        gelf::PackageCompressor compresser;

        gelf::Data compressedData = compresser.compress( data );

        ASSERT_EQ( compressedData.m_uSize, 75 );
    }
}
