#include "UDPProtocol.hpp"
#include "Data.hpp"

#if defined(WIN32)
#define NOMINMAX
#include <winsock.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#define SOCKET int
#define closesocket(x) close(x)
#endif

#include <algorithm>

bool gelf::UDPProtocol::send( const struct sockaddr_in& pSockAddr, const ChunkedData & rData )
{
    SOCKET sock;

    if( ( sock = socket( AF_INET, SOCK_DGRAM, 0 ) ) == -1 )
    {
        m_strError = "Could not create socket";
        return false;
    }

    size_t uTotalBytesSent = 0;
    uint8_t* pData = rData.m_pData;

    while( uTotalBytesSent < rData.m_uSize )
    {
        int iBytesToSend = static_cast< int >( std::min( rData.m_uChunkSize, rData.m_uSize - uTotalBytesSent ) );
        int iBytesSent = sendto( sock, reinterpret_cast< const char* >( pData ), iBytesToSend, 0, ( struct sockaddr * )&pSockAddr, sizeof( struct sockaddr ) );

        if( iBytesSent != iBytesToSend )
        {
            m_strError = "Was not able to send data";
            return false;
        }

        uTotalBytesSent += iBytesSent;
    }

    closesocket( sock );

    return true;
}

const std::string gelf::UDPProtocol::getError() const
{
    return  m_strError;
}
