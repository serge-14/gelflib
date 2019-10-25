#include "gtest/gtest.h"

#include "MessageBuilder.hpp"

namespace tests
{
    TEST( MessageBuilderTests, EmergencyLevel )
    {
        std::string strValue = gelf::MessageBuilder( gelf::Severity::Emergency, "EmergencyLevel" )
            .build().getMessage();

        ASSERT_STREQ( strValue.c_str(), "{\"version\": \"1.1\",\"short_message\": \"EmergencyLevel\",\"level\": 0,\"host\": \"localhost\"}" );
    }

    TEST( MessageBuilderTests, EmergencyLevelThroughStdString )
    {
        std::string strValue = gelf::MessageBuilder( gelf::Severity::Emergency, std::string( "EmergencyLevel" ) )
            .build().getMessage();

        ASSERT_STREQ( strValue.c_str(), "{\"version\": \"1.1\",\"short_message\": \"EmergencyLevel\",\"level\": 0,\"host\": \"localhost\"}" );
    }

    TEST( MessageBuilderTests, AlertLevel )
    {
        std::string strValue = gelf::MessageBuilder( gelf::Severity::Alert, "AlertLevel" )
            .build().getMessage();

        ASSERT_STREQ( strValue.c_str(), "{\"version\": \"1.1\",\"short_message\": \"AlertLevel\",\"level\": 1,\"host\": \"localhost\"}" );
    }

    TEST( MessageBuilderTests, CriticalLevel )
    {
        std::string strValue = gelf::MessageBuilder( gelf::Severity::Critical, "CriticalLevel" )
            .build().getMessage();

        ASSERT_STREQ( strValue.c_str(), "{\"version\": \"1.1\",\"short_message\": \"CriticalLevel\",\"level\": 2,\"host\": \"localhost\"}" );
    }

    TEST( MessageBuilderTests, ErrorLevel )
    {
        std::string strValue = gelf::MessageBuilder( gelf::Severity::Error, "ErrorLevel" )
            .build().getMessage();

        ASSERT_STREQ( strValue.c_str(), "{\"version\": \"1.1\",\"short_message\": \"ErrorLevel\",\"level\": 3,\"host\": \"localhost\"}" );
    }

    TEST( MessageBuilderTests, WarningLevel )
    {
        std::string strValue = gelf::MessageBuilder( gelf::Severity::Warning, "WarningLevel" )
            .build().getMessage();

        ASSERT_STREQ( strValue.c_str(), "{\"version\": \"1.1\",\"short_message\": \"WarningLevel\",\"level\": 4,\"host\": \"localhost\"}" );
    }

    TEST( MessageBuilderTests, NoticeLevel )
    {
        std::string strValue = gelf::MessageBuilder( gelf::Severity::Notice, "NoticeLevel" )
            .build().getMessage();

        ASSERT_STREQ( strValue.c_str(), "{\"version\": \"1.1\",\"short_message\": \"NoticeLevel\",\"level\": 5,\"host\": \"localhost\"}" );
    }

    TEST( MessageBuilderTests, InformationalLevel )
    {
        std::string strValue = gelf::MessageBuilder( gelf::Severity::Informational, "InformationalLevel" )
            .build().getMessage();

        ASSERT_STREQ( strValue.c_str(), "{\"version\": \"1.1\",\"short_message\": \"InformationalLevel\",\"level\": 6,\"host\": \"localhost\"}" );
    }

    TEST( MessageBuilderTests, DebugLevel )
    {
        std::string strValue = gelf::MessageBuilder( gelf::Severity::Debug, "DebugLevel" )
            .build().getMessage();

        ASSERT_STREQ( strValue.c_str(), "{\"version\": \"1.1\",\"short_message\": \"DebugLevel\",\"level\": 7,\"host\": \"localhost\"}" );
    }

    TEST( MessageBuilderTests, FullMessage )
    {
        std::string strValue = gelf::MessageBuilder( gelf::Severity::Debug, "DebugLevel" )
            .fullMessage( "This is full message" )
            .build().getMessage();

        ASSERT_STREQ( strValue.c_str(), "{\"version\": \"1.1\",\"short_message\": \"DebugLevel\",\"level\": 7,\"full_message\": \"This is full message\",\"host\": \"localhost\"}" );
    }

    TEST( MessageBuilderTests, FullMessageHandleSpecilaSymbols )
    {
        std::string strValue = gelf::MessageBuilder( gelf::Severity::Debug, "DebugLevel" )
            .fullMessage( "Special characters: \", \\, /, \b, \f, \n, \r and \t" )
            .build().getMessage();

        ASSERT_STREQ( strValue.c_str(), "{\"version\": \"1.1\",\"short_message\": \"DebugLevel\",\"level\": 7,\"full_message\": \"Special characters: \\\", \\\\, \\/, \\b, \\f, \\n, \\r and \\t\",\"host\": \"localhost\"}" );
    }

    TEST( MessageBuilderTests, LineNumber )
    {
        std::string strValue = gelf::MessageBuilder( gelf::Severity::Debug, "DebugLevel" )
            .lineNumber( 25 )
            .build().getMessage();

        ASSERT_STREQ( strValue.c_str(), "{\"version\": \"1.1\",\"short_message\": \"DebugLevel\",\"level\": 7,\"line\": 25,\"host\": \"localhost\"}" );
    }

    TEST( MessageBuilderTests, FileName )
    {
        std::string strValue = gelf::MessageBuilder( gelf::Severity::Debug, "DebugLevel" )
            .fileName( "test.cpp" )
            .build().getMessage();

        ASSERT_STREQ( strValue.c_str(), "{\"version\": \"1.1\",\"short_message\": \"DebugLevel\",\"level\": 7,\"file\": \"test.cpp\",\"host\": \"localhost\"}" );
    }

    TEST( MessageBuilderTests, FileNameAndLineNumber )
    {
        std::string strValue = gelf::MessageBuilder( gelf::Severity::Debug, "DebugLevel" )
            .fileName( "test.cpp" )
            .lineNumber( 25 )
            .build().getMessage();

        ASSERT_STREQ( strValue.c_str(), "{\"version\": \"1.1\",\"short_message\": \"DebugLevel\",\"level\": 7,\"file\": \"test.cpp\",\"line\": 25,\"host\": \"localhost\"}" );
    }

    TEST( MessageBuilderTests, SetTime )
    {
        std::time_t result = 1356000;

        std::string strValue = gelf::MessageBuilder( gelf::Severity::Debug, "DebugLevel" )
            .setTime( result )
            .build().getMessage();

        ASSERT_STREQ( strValue.c_str(), "{\"version\": \"1.1\",\"short_message\": \"DebugLevel\",\"level\": 7,\"timestamp\": 1356000,\"host\": \"localhost\"}" );
    }

    TEST( MessageBuilderTests, SetTimeNow )
    {
        std::time_t result = std::time( nullptr );

        std::string strValue = gelf::MessageBuilder( gelf::Severity::Debug, "DebugLevel" )
            .setTimeNow()
            .build().getMessage();

        char buffer[ 256 ];
        snprintf( buffer, 256, "{\"version\": \"1.1\",\"short_message\": \"DebugLevel\",\"level\": 7,\"timestamp\": %ld,\"host\": \"localhost\"}", result );

        ASSERT_STREQ( strValue.c_str(), buffer );
    }

    TEST( MessageBuilderTests, AdditionalField )
    {
        std::string strValue = gelf::MessageBuilder( gelf::Severity::Debug, "DebugLevel" )
            .additionalField( "facility", "application" )
            .additionalField( "number", 255 )
            .build().getMessage();

        ASSERT_STREQ( strValue.c_str(), "{\"version\": \"1.1\",\"short_message\": \"DebugLevel\",\"level\": 7,\"_facility\": \"application\",\"_number\": 255,\"host\": \"localhost\"}" );
    }

    TEST( MessageBuilderTests, AdditionalField_IgnoreIdField )
    {
        std::string strValue = gelf::MessageBuilder( gelf::Severity::Debug, "DebugLevel" )
            .additionalField( "id", 2 )
            .build().getMessage();

        ASSERT_STREQ( strValue.c_str(), "{\"version\": \"1.1\",\"short_message\": \"DebugLevel\",\"level\": 7,\"host\": \"localhost\"}" );
    }

    TEST( MessageBuilderTests, DifferentHash )
    {
        gelf::Message message1 = gelf::MessageBuilder( gelf::Severity::Debug, "DebugLevel" )
            .setTime( 25 )
            .build();

        gelf::Message message2 = gelf::MessageBuilder( gelf::Severity::Informational, "InformationalLevel" )
            .setTime( 26 )
            .build();

        ASSERT_NE( message1.getHashCode(), message2.getHashCode() );
    }

    TEST( MessageBuilderTests, DifferentHash_Host )
    {
        gelf::Message message1 = gelf::MessageBuilder( gelf::Severity::Debug, "DebugLevel" )
            .setHost( "127.0.0.1" )
            .setTime( 25 )
            .build();

        gelf::Message message2 = gelf::MessageBuilder( gelf::Severity::Informational, "InformationalLevel" )
            .setHost( "127.0.0.2" )
            .setTime( 25 )
            .build();

        ASSERT_NE( message1.getHashCode(), message2.getHashCode() );
    }
}