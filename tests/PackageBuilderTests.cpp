#include "gtest/gtest.h"

#include "PackageBuilder.hpp"

namespace tests
{
    TEST( PackageBuilderTests, SimplePackage )
    {
        const char* pData = "";
        size_t uSize = 0;

        gelf::ChunkedData package = gelf::PackageBuilder( reinterpret_cast< const uint8_t* >( pData ), uSize )
            .build();

        ASSERT_EQ( package.m_uChunkSize, 0 );
        ASSERT_EQ( package.m_uSize, 0 );
        ASSERT_TRUE( package.m_pData != nullptr );
    }

    TEST( PackageBuilderTests, TestData )
    {
        const char* pData = "test data";
        size_t uSize = 9;

        gelf::ChunkedData package = gelf::PackageBuilder( reinterpret_cast< const uint8_t* >( pData ), uSize )
            .build();

        ASSERT_EQ( package.m_uChunkSize, 9 );
        ASSERT_EQ( package.m_uSize, 9 );

        for( size_t i = 0; i < package.m_uChunkSize; ++i )
        {
            ASSERT_EQ( package.m_pData[ i ], pData[ i ] );
        }
    }

    void AssertChunkedPackage( const gelf::ChunkedData& package, const char* pData, size_t uSize, uint64_t uMessageId, uint8_t uTargetChunkCount, uint8_t uTargetChunkSize )
    {
        ASSERT_EQ( package.m_uChunkSize, uTargetChunkSize );
        ASSERT_EQ( package.m_uSize, uSize + sizeof( gelf::ChunkHeader ) * uTargetChunkCount );

        size_t uDataIndex = 0;
        uint8_t uChunkId = 0;
        uint8_t* pChunk = package.m_pData;
        size_t uBytesRemaining = package.m_uSize;

        while( pChunk < package.m_pData + package.m_uSize )
        {
            gelf::ChunkHeader* pHeader = reinterpret_cast< gelf::ChunkHeader* >( pChunk );

            ASSERT_EQ( pChunk[ 0 ], 0x1e );
            ASSERT_EQ( pChunk[ 1 ], 0x0f );
            ASSERT_EQ( pHeader->m_uMagicBytes, gelf::g_uChunkHeaderMagicBytes );
            ASSERT_EQ( pHeader->m_uSequenceCount, uTargetChunkCount );
            ASSERT_EQ( pHeader->m_uSequenceNumber, uChunkId );
            ASSERT_EQ( pHeader->m_uMessageId, uMessageId );

            uBytesRemaining -= sizeof( gelf::ChunkHeader );
            pChunk += sizeof( gelf::ChunkHeader );

            size_t uSizeToRead = std::min( uBytesRemaining, package.m_uChunkSize - sizeof( gelf::ChunkHeader ) );

            for( size_t i = 0; i < uSizeToRead; i++ )
            {
                ASSERT_EQ( pChunk[ i ], pData[ uDataIndex ] );
                ++uDataIndex;
            }

            uBytesRemaining -= uSizeToRead;
            pChunk += uSizeToRead;

            ++uChunkId;
        }

        ASSERT_EQ( uChunkId, uTargetChunkCount );
    }

    TEST( PackageBuilderTests, ChunkedData )
    {
        const char* pData = "XrUUa2a9qGornk0kxJ1twKfc8zFQN1o1bih3LU9N62PbayotafvkZ12j703RaXWr";
        size_t uSize = 64;

        uint64_t uMessageId = std::hash<const char* >() ( pData );
        uint8_t uTargetChunkCount = 4;
        uint8_t uTargetChunkSize = 32;

        gelf::ChunkedData package = gelf::PackageBuilder( reinterpret_cast< const uint8_t* >( pData ), uSize )
            .setMessageId( uMessageId )
            .setChunkSize( uTargetChunkSize )
            .build();

        AssertChunkedPackage( package, pData, uSize, uMessageId, uTargetChunkCount, uTargetChunkSize );
    }

    TEST( PackageBuilderTests, ChunkedData_SmallChunk )
    {
        const char* pData = "XrUUa2a9qGornk0kxJ1twKfc8zFQN1o1bih3LU9N62PbayotafvkZ12j703RaXWr";
        size_t uSize = 64;

        uint64_t uMessageId = std::hash<const char* >() ( pData );
        uint8_t uTargetChunkCount = 64;
        uint8_t uTargetChunkSize = 13;

        gelf::ChunkedData package = gelf::PackageBuilder( reinterpret_cast< const uint8_t* >( pData ), uSize )
            .setMessageId( uMessageId )
            .setChunkSize( uTargetChunkSize )
            .build();

        AssertChunkedPackage( package, pData, uSize, uMessageId, uTargetChunkCount, uTargetChunkSize );
    }

    TEST( PackageBuilderTests, ChunkedData_LastSmall )
    {
        const char* pData = "niZtsGPlDEXNvAsJTrZwVgBHCxFnaQwrO";
        size_t uSize = 33;

        uint64_t uMessageId = std::hash<const char* >() ( pData );
        uint8_t uTargetChunkCount = 2;
        uint8_t uTargetChunkSize = 32;

        gelf::ChunkedData package = gelf::PackageBuilder( reinterpret_cast< const uint8_t* >( pData ), uSize )
            .setMessageId( uMessageId )
            .setChunkSize( uTargetChunkSize )
            .build();

        AssertChunkedPackage( package, pData, uSize, uMessageId, uTargetChunkCount, uTargetChunkSize );
    }

    TEST( PackageBuilderTests, ChunkedData_ThreeEqual )
    {
        const char* pData = "XrUUa2a9qGornk0kxJ1twKfc8zFQN1o1bih3LU9N62PbayotafvkZ12j703R";
        size_t uSize = 60;

        uint64_t uMessageId = std::hash<const char* >() ( pData );
        uint8_t uTargetChunkCount = 3;
        uint8_t uTargetChunkSize = 32;

        gelf::ChunkedData package = gelf::PackageBuilder( reinterpret_cast< const uint8_t* >( pData ), uSize )
            .setMessageId( uMessageId )
            .setChunkSize( uTargetChunkSize )
            .build();

        AssertChunkedPackage( package, pData, uSize, uMessageId, uTargetChunkCount, uTargetChunkSize );
    }

    TEST( PackageBuilderTests, ChunkedData_MoreThen128Packages )
    {
        const char* pData = nullptr;
        size_t uSize = 32 * 129;

        uint64_t uMessageId = 0;
        uint8_t uTargetChunkCount = 3;
        uint8_t uTargetChunkSize = 32;

        gelf::ChunkedData package = gelf::PackageBuilder( reinterpret_cast< const uint8_t* >( pData ), uSize )
            .setMessageId( uMessageId )
            .setChunkSize( uTargetChunkSize )
            .build();

        ASSERT_EQ( package.m_uChunkSize, 0 );
        ASSERT_EQ( package.m_uSize, 0 );
        ASSERT_TRUE( package.m_pData == nullptr );
    }

    TEST( PackageBuilderTests, ChunkedDataWithChunkSize )
    {
        const char* pData = "niZtsGPlDEXNvAsJTrZwVgBHCxFnaQwr";
        size_t uSize = 32;

        gelf::ChunkedData package = gelf::PackageBuilder( reinterpret_cast< const uint8_t* >( pData ), uSize )
            .setChunkSize( 32 )
            .build();

        ASSERT_EQ( package.m_uChunkSize, 32 );
        ASSERT_EQ( package.m_uSize, uSize );

        for( size_t i = 0; i < package.m_uChunkSize; ++i )
        {
            ASSERT_EQ( package.m_pData[ i ], pData[ i ] );
        }
    }

    TEST( PackageBuilderTests, ChunkedDataWithOneChunk )
    {
        const char* pData = "uXkxl0oRLB24wGry";
        size_t uSize = 16;

        gelf::ChunkedData package = gelf::PackageBuilder( reinterpret_cast< const uint8_t* >( pData ), uSize )
            .setChunkSize( 32 )
            .build();

        ASSERT_EQ( package.m_uChunkSize, 16 );
        ASSERT_EQ( package.m_uSize, 16 );

        for( size_t i = 0; i < package.m_uChunkSize; i++ )
        {
            ASSERT_EQ( package.m_pData[ i ], pData[ i ] );
        }
    }
}
