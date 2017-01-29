#include "PackageBuilder.hpp"
#include "Data.hpp"

#include <algorithm>

gelf::PackageBuilder::PackageBuilder( const gelf::Data& rData ) :
    m_pOriginalData( rData.m_pData ),
    m_uOriginalSize( rData.m_uSize ),
    m_uChunkSize( 0 ),
    m_uMessageId( 0 )
{
}

gelf::PackageBuilder::PackageBuilder( const uint8_t* const pData, size_t uSize ) :
    m_pOriginalData( pData ),
    m_uOriginalSize( uSize ),
    m_uChunkSize( 0 ),
    m_uMessageId( 0 )
{
}

gelf::PackageBuilder& gelf::PackageBuilder::setMessageId( uint64_t uMessageId )
{
    m_uMessageId = uMessageId;
    return *this;
}


gelf::PackageBuilder& gelf::PackageBuilder::setChunkSize( size_t uSize )
{
    GELF_ASSERT( uSize > 12 );

    m_uChunkSize = uSize;
    return *this;
}

gelf::ChunkedData gelf::PackageBuilder::build()
{   
    ChunkedData data;

    uint8_t uChunkCount = 0;
    size_t uDataChunkSize = m_uOriginalSize;

    if( m_uChunkSize == 0 || m_uOriginalSize <= m_uChunkSize )
    {
        uChunkCount = 1;

        data.m_uSize = m_uOriginalSize;
        data.m_uChunkSize = m_uOriginalSize;
    }
    else
    {
        uDataChunkSize = m_uChunkSize - sizeof( gelf::ChunkHeader );

        size_t uFullChunkCount = m_uOriginalSize / uDataChunkSize;
        size_t uLastChunkSize = m_uOriginalSize - uFullChunkCount * uDataChunkSize;
        size_t uRealChunkCount = uFullChunkCount + ( uLastChunkSize != 0 ? 1 : 0 );

        GELF_ASSERT( uRealChunkCount <= 128 );

        if( uRealChunkCount > 128 )
        {
            return data;
        }

        uChunkCount = static_cast< uint8_t >( uRealChunkCount );

        data.m_uSize = uFullChunkCount * m_uChunkSize + ( uLastChunkSize != 0 ? ( uLastChunkSize + sizeof( gelf::ChunkHeader ) ) : 0 );
        data.m_uChunkSize = m_uChunkSize;
    }

    data.m_pData = new uint8_t[ data.m_uSize ];

    const uint8_t* pSrcData = m_pOriginalData;
    uint8_t* pDestData = data.m_pData;
    
    size_t uBytesRemaining = m_uOriginalSize;

    for( uint8_t i = 0; i < uChunkCount; i++ )
    {
        if( uChunkCount != 1 )
        {
            gelf::ChunkHeader* pHeader = reinterpret_cast< gelf::ChunkHeader* >( pDestData );

            pHeader->m_uMagicBytes = gelf::g_uChunkHeaderMagicBytes;
            pHeader->m_uMessageId = m_uMessageId;
            pHeader->m_uSequenceCount = uChunkCount;
            pHeader->m_uSequenceNumber = i;

            pDestData += sizeof( gelf::ChunkHeader );
        }

        size_t uSizeToCopy = std::min( uBytesRemaining, uDataChunkSize );

        memcpy( pDestData, pSrcData, uSizeToCopy );

        uBytesRemaining -= uSizeToCopy;
        pDestData += uSizeToCopy;
        pSrcData += uSizeToCopy;
    }

    return data;
}
