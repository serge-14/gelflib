#ifndef GELF_LIB_INTERNAL_GELF_HPP
#define GELF_LIB_INTERNAL_GELF_HPP

#include <cinttypes>
#include <string>

struct sockaddr_in;

namespace gelf
{
    struct sockaddr_in& getConfiguredSockAddr();
    struct sockaddr_in getSockAddr( const std::string & strHost, uint16_t uPort );
}

#endif //GELF_LIB_INTERNAL_GELF_HPP