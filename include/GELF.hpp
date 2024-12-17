#ifndef GELF_LIB_GELF_HPP
#define GELF_LIB_GELF_HPP

#include <cinttypes>
#include <string>

#include "DllDefines.hpp"
#include "MessageBuilder.hpp"

namespace gelf
{
    /**
     * Initializes internal library resources. Only after calling that method, other methods can be used.
     * @see gelf::destroy()
     * @return Returns true if library were initialized successfully, otherwise false.
     */
    GELFLIB_EXPORT bool initialize();

    /**
     * Destroys internal library resources.
     * @see gelf::initialize()
     */
    GELFLIB_EXPORT void destroy();

    /**
     * Posts a message to configured server.
     * @param rMessage A message to be sent
     * @return Returns true if message were sent successfully, otherwise false.
     */
    GELFLIB_EXPORT bool post( const gelf::Message& rMessage );

    /**
     * Posts a message.
     * @param rMessage A message to be sent
     * @param strHost Host IP address
     * @param uPort A port
     * @return Returns true if message were sent successfully, otherwise false.
     */
    GELFLIB_EXPORT bool post( const gelf::Message& rMessage, const std::string & strHost, uint16_t uPort );

    /**
     * Configures default server
     * @param strHost Host IP address
     * @param uPort A port
     * @return Returns true if message were sent successfully, otherwise false.
     */
    GELFLIB_EXPORT bool configure( const std::string& strHost, uint16_t uPort );

    /**
     * Gets configured host IP address
     * @return Returns string with configured IP address
     */
    GELFLIB_EXPORT const std::string& getConfiguredHost();

    /**
     * Gets configured port
     * @return Returns a port
     */
    GELFLIB_EXPORT const uint16_t getConfiguredPort();
}

#endif //GELF_LIB_GLEF_HPP
