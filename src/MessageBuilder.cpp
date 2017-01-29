#include "MessageBuilder.hpp"

#include "Data.hpp"

#include <ctime>

gelf::MessageBuilder::MessageBuilder( Severity::Value eSeverity, const std::string& strShortMessage )
{
    field( "version", "1.1" );
    field( "short_message", strShortMessage );
    field( "level", eSeverity );
}

gelf::MessageBuilder::MessageBuilder( Severity::Value eSeverity, const char* strShortMessage )
{
    field( "version", "1.1" );
    field( "short_message", strShortMessage );
    field( "level", eSeverity );
}

gelf::MessageBuilder& gelf::MessageBuilder::fullMessage( const std::string& strFullMessage )
{
    return fullMessage( strFullMessage.c_str() );
}

gelf::MessageBuilder& gelf::MessageBuilder::fullMessage( const char* strFullMessage )
{
    return field( "full_message", strFullMessage );
}

gelf::MessageBuilder& gelf::MessageBuilder::lineNumber( size_t uLine )
{
    return field( "line", uLine );
}

gelf::MessageBuilder& gelf::MessageBuilder::fileName( const std::string& strFile )
{
    return fileName( strFile.c_str() );
}

gelf::MessageBuilder& gelf::MessageBuilder::fileName( const char* strFile )
{
    return field( "file", strFile );
}

gelf::MessageBuilder& gelf::MessageBuilder::setTime( time_t tTime )
{
    m_strTime = std::to_string( tTime );
    return field( "timestamp", tTime );
}

gelf::MessageBuilder & gelf::MessageBuilder::setTimeNow()
{
    return setTime( std::time( nullptr ) );
}

gelf::MessageBuilder& gelf::MessageBuilder::setHost( const std::string& strHost )
{
    m_strHost = strHost;
    return field( "host", strHost );
}

gelf::Message gelf::MessageBuilder::build()
{
    if( m_strHost.empty() )
    {
        setHost( "localhost" );
    }

    std::stringstream strStream;

    strStream << "{";

    for( size_t i = 0; i < m_vecContent.size(); i++ )
    {
        strStream << "\"";
        strStream << m_vecContent[ i ].first;
        strStream << "\": ";
        strStream << m_vecContent[ i ].second;
        strStream << ( i == m_vecContent.size() - 1 ? "" : "," );
    }

    strStream << "}";

    return Message( strStream.str(), std::hash< std::string >() ( m_strHost + m_strTime ) );
}

void gelf::MessageBuilder::addField( const char* strField, const char* strContent )
{
    GELF_ASSERT( strcmp( strField, "_id" ) == 0 );

    if( strcmp( strField, "_id" ) == 0 )
    {
        return;
    }

    m_vecContent.push_back( std::make_pair( strField, strContent ) );
}

inline char convertSpecial( const char& c, char& n )
{
    if( c == '\n' )
    {
        n = 'n';
        return true;
    }

    if( c == '\t' )
    {
        n = 't';
        return true;
    }

    if( c == '\r' )
    {
        n = 'r';
        return true;
    }

    if( c == '\f' )
    {
        n = 'f';
        return true;
    }

    if( c == '/' )
    {
        n = '/';
        return true;
    }

    if( c == '\\' )
    {
        n = '\\';
        return true;
    }

    if( c == '\b' )
    {
        n = 'b';
        return true;
    }

    if( c == '\"' )
    {
        n = '\"';
        return true;
    }

    n = c;
    return false;
}

template<>
gelf::MessageBuilder& gelf::MessageBuilder::field< const char* >( const char* strField, const char* const & rContent )
{
    std::stringstream strStream;
    strStream << "\"";

    size_t i = 0;
    while( rContent[ i ] != '\0' )
    {
        char c;

        strStream << ( convertSpecial( rContent[ i ], c ) ? "\\" : "" );
        strStream << c;
        i++;
    }

    strStream << "\"";

    addField( strField, strStream.str().c_str() );

    return *this;
}

template<>
gelf::MessageBuilder& gelf::MessageBuilder::field< std::string >( const char* strField, const std::string& rContent )
{
    return field( strField, rContent.c_str() );
}

inline gelf::Message::Message( const std::string & strMessage, uint64_t uHash ) :
    m_strMessage( strMessage ),
    m_uHash( uHash )
{

}

const gelf::Data gelf::Message::getData() const
{
    return gelf::Data( const_cast< uint8_t* >( reinterpret_cast<const uint8_t*>( m_strMessage.data() ) ), m_strMessage.size() );
}

const std::string& gelf::Message::getMessage() const
{
    return m_strMessage;
}

const uint64_t gelf::Message::getHashCode() const
{
    return m_uHash;
}
