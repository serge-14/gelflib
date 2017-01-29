#ifndef GELF_LIB_GELFCONFIG_HPP
#define GELF_LIB_GELFCONFIG_HPP

#include <cassert>

#ifdef GELF_DIAGNOSTIC
#define GELF_ASSERT(x) assert(x)
#else
#define GELF_ASSERT(x) ((void)0)
#endif

#endif //GELF_LIB_GELFCONFIG_HPP