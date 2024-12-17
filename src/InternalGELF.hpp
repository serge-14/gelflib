#ifndef GELF_LIB_INTERNAL_GELF_HPP
#define GELF_LIB_INTERNAL_GELF_HPP

#include <cinttypes>
#include <string>

struct sockaddr_in;

namespace gelf
{
    struct sockaddr_in& getConfiguredSockAddr();
    bool getSockAddr( const std::string & strHost, uint16_t uPort, struct sockaddr_in * server_addr );
}

#endif //GELF_LIB_INTERNAL_GELF_HPP