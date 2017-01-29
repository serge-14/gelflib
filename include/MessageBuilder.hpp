#ifndef GELF_LIB_MESSAGE_BUILDER_HPP
#define GELF_LIB_MESSAGE_BUILDER_HPP

#include <string>
#include <vector>
#include <sstream>

#include "DllDefines.hpp"
#include "GELFConfig.hpp"

namespace gelf
{
    /**
     * Message severity
     */
    struct Severity
    {
        enum Value
        {
            Emergency = 0,
            Alert,
            Critical,
            Error,
            Warning,
            Notice,
            Informational,
            Debug
        };
    };

    class MessageBuilder;
    struct Data;

    /**
     * A Message class
     */
    class GELFLIB_EXPORT Message
    {
        friend class MessageBuilder;
    public:

        /**
         * Gets copy of data structure
         */
        const Data getData() const;

        /**
         * Gets message represented as string
         */
        const std::string& getMessage() const;

        /**
         * Get hash of the message
         */
        const uint64_t getHashCode() const;

    private:
        Message( const std::string& strMessage, uint64_t uHash );

        std::string m_strMessage;
        uint64_t m_uHash;
    };

    /**
     * A message builder class
     */
    class GELFLIB_EXPORT MessageBuilder
    {
    public:
        /**
         * Constructor to message builder with STL string
         */
        MessageBuilder( Severity::Value eSeverity, const std::string& strShortMessage );

        /**
         * Constructor to message builder with null-terminated string
         */
        MessageBuilder( Severity::Value eSeverity, const char* strShortMessage );

        /**
         * Sets full message with STL string
         */
        MessageBuilder& fullMessage( const std::string& strFullMessage );

        /**
         * Sets full message with  null-terminated string
         */
        MessageBuilder& fullMessage( const char* strFullMessage );

        /**
         * Sets a file name with STL string
         */
        MessageBuilder& fileName( const std::string& strFile );

        /**
         * Sets a file name with null-terminated string
         */
        MessageBuilder& fileName( const char* strFile );

        /**
         * Sets a line number
         */
        MessageBuilder& lineNumber( size_t uLine );

        /**
         * Sets source host
         */
        MessageBuilder& setHost( const std::string& strHost );

        /**
         * Sets custom timestamp
         */
        MessageBuilder& setTime( time_t tTime );

        /**
         * Sets timestamp to current time. Will be set by default when building a message
         */
        MessageBuilder& setTimeNow();


        /**
         * Adds additional field
         */
        template< typename T >
        MessageBuilder& additionalField( const std::string& strField, const T& rContent );

        /**
         * Builds message
         */
        Message build();

    private:
        template< typename T >
        MessageBuilder& field( const std::string& strField, const T& rContent );

        template< typename T >
        MessageBuilder& field( const char* strField, const T& rContent );

        template< std::size_t N >
        MessageBuilder& field( const char* strField, const char( &rContent )[ N ] );

        void addField( const char* strField, const char* strContent );

        std::vector<std::pair<std::string, std::string>> m_vecContent;
        std::string m_strHost;
        std::string m_strTime;
    };

    template< typename T >
    inline MessageBuilder & MessageBuilder::additionalField( const std::string & strField, const T& rContent )
    {
        return field( "_" + strField, rContent );
    }

    template< typename T >
    inline MessageBuilder & MessageBuilder::field( const std::string& strField, const T& rContent )
    {
        return field( strField.c_str(), rContent );
    }

    template< typename T >
    inline MessageBuilder& MessageBuilder::field( const char* strField, const T& rContent )
    {
        std::stringstream strStream;
        strStream << rContent;

        addField( strField, strStream.str().c_str() );

        return *this;
    }

    template< std::size_t N >
    inline MessageBuilder& MessageBuilder::field( const char* strField, const char( &rContent )[ N ] )
    {
        return field( strField, static_cast< const char* >( rContent ) );
    }

    template<>
    MessageBuilder& MessageBuilder::field< const char* >( const char* strField, const char* const & rContent );

    template<>
    MessageBuilder& MessageBuilder::field< std::string >( const char* strField, const std::string& rContent );
}

#endif //GELF_LIB_MESSAGE_BUILDER_HPP
