#ifndef GELF_LIB_UDP_PROTOCOL_HPP
#define GELF_LIB_UDP_PROTOCOL_HPP

#include <cinttypes>
#include <memory>
#include <string>

struct sockaddr_in;

namespace gelf
{
    struct ChunkedData;

    class UDPProtocol
    {
    public:
        bool send(const struct sockaddr_in& pSockAddr, const ChunkedData& rData );
        const std::string getError() const;

    private:
        std::string m_strError;
    };
}

#endif //GELF_LIB_UDP_PROTOCOL_HPP
