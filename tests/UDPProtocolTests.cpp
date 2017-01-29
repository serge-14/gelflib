#include "gtest/gtest.h"

#include "UDPProtocol.hpp"
#include "MockServer.hpp"
#include "Data.hpp"

#include "GELF.hpp"
#include "InternalGELF.hpp"

namespace tests
{
    TEST( UDPProtocolTests, SimpleSend )
    {
        gelf::configure( "127.0.0.1", 5000 );

        gelf::ChunkedData chunkedData( "test" );
        gelf::UDPProtocol udpProtocol;

        ASSERT_TRUE( udpProtocol.send( gelf::getConfiguredSockAddr(), chunkedData ) );
        ASSERT_TRUE( udpProtocol.getError().empty() );
    }

    TEST( UDPProtocolTests, EndToEnd )
    {
        MockServer mockServer;
        mockServer.start( 5000, 1 );

        gelf::configure( "127.0.0.1", 5000 );

        gelf::ChunkedData chunkedData( "test" );
        gelf::UDPProtocol udpProtocol;

        ASSERT_TRUE( udpProtocol.send( gelf::getConfiguredSockAddr(), chunkedData ) );
        ASSERT_TRUE( udpProtocol.getError().empty() );

        mockServer.stop();

        ASSERT_EQ( mockServer.getPacketCount(), 1 );
        ASSERT_EQ( mockServer.getTotalBytes(), 4 );
    }

    TEST( UDPProtocolTests, EndToEnd_Chunked )
    {
        MockServer mockServer;
        mockServer.start( 5000, 2 );

        gelf::configure( "127.0.0.1", 5000 );

        gelf::ChunkedData chunkedData( "testda" );
        gelf::UDPProtocol udpProtocol;

        chunkedData.m_uChunkSize = 4;

        ASSERT_TRUE( udpProtocol.send( gelf::getConfiguredSockAddr(), chunkedData ) );
        ASSERT_TRUE( udpProtocol.getError().empty() );

        mockServer.stop();

        ASSERT_EQ( mockServer.getPacketCount(), 2 );
        ASSERT_EQ( mockServer.getTotalBytes(), 6 );
    }
}