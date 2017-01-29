#ifndef GELF_LIB_PACKAGE_BUILDER_HPP
#define GELF_LIB_PACKAGE_BUILDER_HPP

#include "GELFConfig.hpp"
#include "Data.hpp"

namespace gelf
{
    const int g_uChunkHeaderMagicBytes = 0x0F1E;

#pragma pack(push)
#pragma pack(1)
    struct ChunkHeader
    {
        uint16_t m_uMagicBytes;
        uint64_t m_uMessageId;
        uint8_t m_uSequenceNumber;
        uint8_t m_uSequenceCount;
    };
#pragma pack(pop)

    class PackageBuilder
    {
    public:
        PackageBuilder( const gelf::Data& rData );
        PackageBuilder( const uint8_t* const pData, size_t uSize );

        PackageBuilder& setMessageId( uint64_t uMessageId );
        PackageBuilder& setChunkSize( size_t uSize );

        ChunkedData build();

    private:
        const uint8_t* const m_pOriginalData;
        size_t m_uOriginalSize;

        size_t m_uChunkSize;
        uint64_t m_uMessageId;
    };
}

#endif //GELF_LIB_PACKAGE_BUILDER_HPP
