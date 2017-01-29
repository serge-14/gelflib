// Contents of DLLDefines.h
#ifndef _GELFLIB_DLL_DEFINES_
#define _GELFLIB_DLL_DEFINES_

/* Cmake will define MyLibrary_EXPORTS on Windows when it
configures to build a shared library. If you are going to use
another build system on windows or create the visual studio
projects by hand you need to define MyLibrary_EXPORTS when
building a DLL on windows.
*/
// We are using the Visual Studio Compiler and building Shared libraries

#if defined (_WIN32) & !defined(GELF_STATIC)
  #if defined(gelflib_EXPORTS)
    #define  GELFLIB_EXPORT __declspec(dllexport)
  #else
    #define  GELFLIB_EXPORT __declspec(dllimport)
  #endif /* MyLibrary_EXPORTS */
#else /* defined (_WIN32) */
 #define GELFLIB_EXPORT
#endif

#endif /* _GELFLIB_DLL_DEFINES_ */
