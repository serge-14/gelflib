#include "GELF.hpp"
#include "InternalGELF.hpp"
#include "GELFConfig.hpp"

#if defined(WIN32)
#define NOMINMAX
#include <winsock.h>
#elif defined(__linux__)
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#define SOCKET int
#define closesocket(x) close(x)
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#endif

#include "MessageBuilder.hpp"
#include "Data.hpp"
#include "UDPProtocol.hpp"
#include "PackageBuilder.hpp"
#include "PackageCompressor.hpp"

struct Configuration
{
    std::string strHost;
    uint16_t uPort;
    struct sockaddr_in server_addr;
};

static Configuration* g_pConfiguration = nullptr;

bool gelf::initialize()
{
#if defined(WIN32)
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD( 2, 2 );

    err = WSAStartup( wVersionRequested, &wsaData );
    if( err != 0 )
    {
        return false;
    }
#endif
    g_pConfiguration = new Configuration();
    g_pConfiguration->strHost = "127.0.0.1";
    g_pConfiguration->uPort = 9099;

    return g_pConfiguration != nullptr;
}

void gelf::destroy()
{
    GELF_ASSERT( g_pConfiguration != nullptr );

    delete g_pConfiguration;
    g_pConfiguration = nullptr;
#if defined(WIN32)
    WSACleanup();
#endif
}

void gelf::configure( const std::string& strHost, uint16_t uPort )
{
    GELF_ASSERT( g_pConfiguration != nullptr );

    g_pConfiguration->strHost = strHost;
    g_pConfiguration->uPort = uPort;
    g_pConfiguration->server_addr = getSockAddr( strHost, uPort );
}

const std::string& gelf::getConfiguredHost()
{
    GELF_ASSERT( g_pConfiguration != nullptr );
    return g_pConfiguration->strHost;
}

const uint16_t gelf::getConfiguredPort()
{
    GELF_ASSERT( g_pConfiguration != nullptr );
    return g_pConfiguration->uPort;
}

struct sockaddr_in& gelf::getConfiguredSockAddr()
{
    GELF_ASSERT( g_pConfiguration != nullptr );
    return g_pConfiguration->server_addr;
}

struct sockaddr_in gelf::getSockAddr( const std::string & strHost, uint16_t uPort )
{
    sockaddr_in server_addr;

    struct hostent* pHost = ( struct hostent * ) gethostbyname( strHost.c_str() );

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons( uPort );
    server_addr.sin_addr = *( ( struct in_addr * )pHost->h_addr );
    memset( &( server_addr.sin_zero ), 0, 8 );

    return server_addr;
}

bool internalPost( const gelf::Message& rMessage, const struct sockaddr_in& pSockAddr )
{
    gelf::PackageCompressor packageCompressor;
    gelf::Data compressedData = packageCompressor.compress( rMessage.getData() );

    gelf::ChunkedData chunkedData = gelf::PackageBuilder( compressedData )
        .setChunkSize( 8192 )
        .setMessageId( rMessage.getHashCode() )
        .build();

    gelf::UDPProtocol udpProtocol;
    return udpProtocol.send( pSockAddr, chunkedData );
}

bool gelf::post( const gelf::Message& rMessage )
{
    return ::internalPost( rMessage, gelf::getConfiguredSockAddr() );
}

bool gelf::post( const gelf::Message& rMessage, const std::string& strHost, uint16_t uPort )
{
    return ::internalPost( rMessage, gelf::getSockAddr( strHost, uPort ) );
}
