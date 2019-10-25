#include "MockServer.hpp"

#include <cstring>

#if defined(WIN32)
#define NOMINMAX
#include <winsock.h>
#define socket_len int
#elif defined(__linux__) || defined(__APPLE__)
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#define SOCKET int
#define closesocket(x) close(x)
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#define socket_len unsigned int
#endif

void MockServer::start( uint16_t uPort, size_t uTargetPacketCount )
{
    m_uTargetPacketCount = uTargetPacketCount;

    if( ( m_iSocket = socket( AF_INET, SOCK_DGRAM, 0 ) ) == -1 )
    {
        exit( 1 );
    }

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons( uPort );
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset( &( server_addr.sin_zero ), 0, 8 );


    if( bind( m_iSocket, ( struct sockaddr * )&server_addr, sizeof( struct sockaddr ) ) == -1 )
    {
        exit( 1 );
    }

    m_bExit = false;
    m_uBytesRead = 0;
    m_uPacketCount = 0;

    m_thread = std::thread( [ this ] ()
    {
        socket_len addr_len;
        char recv_data[ 1024 ];
        struct sockaddr_in client_addr;

        addr_len = sizeof( struct sockaddr );

        while( !m_bExit )
        {
            int iBytes = recvfrom( m_iSocket, recv_data, 1024, 0, ( struct sockaddr * )&client_addr, &addr_len );

            if( iBytes > 0 )
            {
                m_uBytesRead += iBytes;
                m_uPacketCount++;
            }

            if( m_uPacketCount == m_uTargetPacketCount )
            {
                m_cvMessagesRecived.notify_one();
            }
        }
    } );
}

void MockServer::stop()
{
    {
        std::unique_lock<std::mutex> lk( m_mMutex );
        m_cvMessagesRecived.wait_for( lk, std::chrono::milliseconds( 200 ) );
    }

    m_bExit = true;
    closesocket( m_iSocket );

    m_thread.join();
}

size_t MockServer::getPacketCount() const
{
    return m_uPacketCount;
}

size_t MockServer::getTotalBytes() const
{
    return m_uBytesRead;
}
