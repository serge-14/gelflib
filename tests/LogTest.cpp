#include "gtest/gtest.h"

#include "GELF.hpp"
#include "MessageBuilder.hpp"
#include "MockServer.hpp"

namespace tests
{
    TEST( LogTest, EndToEnd )
    {
        MockServer mockServer;
        mockServer.start( 5000, 1 );

        const char* host = "127.0.0.1";

        bool result = gelf::post( 
            gelf::MessageBuilder( gelf::Severity::Informational, "test message" )
                .additionalField( "test", "yes!" ).build(),
            host, 5000 );

        mockServer.stop();

        ASSERT_TRUE( result );
        ASSERT_EQ( mockServer.getPacketCount(), 1 );
        ASSERT_EQ( mockServer.getTotalBytes(), 82 );
    }

    TEST( LogTest, DefaultConfiguration )
    {
        MockServer mockServer;
        mockServer.start( 5000, 1 );

        gelf::configure( "127.0.0.1", 5000 );

        gelf::MessageBuilder message( gelf::Severity::Informational, "test message" );
        message.additionalField( "test", "yes!" );

        bool result = gelf::post( message.build() );

        mockServer.stop();

        ASSERT_TRUE( result );
        ASSERT_EQ( mockServer.getPacketCount(), 1 );
        ASSERT_EQ( mockServer.getTotalBytes(), 82 );
    }
}