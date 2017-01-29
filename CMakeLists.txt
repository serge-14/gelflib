cmake_minimum_required (VERSION 3.5.1)
set(PROJECT_NAME_STR gelflib)
project(${PROJECT_NAME_STR} CXX)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    find_package(Threads REQUIRED)
    add_definitions(-Wall -fvisibility=default -ansi -Wno-deprecated -pthread)
endif()

if(MSVC)
    #vc 2012 fix for vararg templates
    set(MSVC_COMPILER_DEFS "-D_VARIADIC_MAX=10 -DGELF_STATIC")
endif()

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})

set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")

find_package(ZLIB REQUIRED)

file(GLOB GELFLIB_PUBLIC_HDRS ${COMMON_INCLUDES}/*.hpp)
file(GLOB GELFLIB_PRIVATE_HDRS ${PROJECT_SOURCE_DIR}/src/*.hpp)
file(GLOB GELFLIB_SRCS ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME_STR} SHARED ${GELFLIB_SRCS} ${GELFLIB_PUBLIC_HDRS} ${GELFLIB_PRIVATE_HDRS})

add_library(${PROJECT_NAME_STR}static STATIC ${GELFLIB_SRCS} ${GELFLIB_PUBLIC_HDRS} ${GELFLIB_PRIVATE_HDRS})
target_compile_definitions(${PROJECT_NAME_STR}static PUBLIC GELF_STATIC )

include_directories(${ZLIB_INCLUDE_DIR})
include_directories(${COMMON_INCLUDES})
target_link_libraries (${PROJECT_NAME_STR} ${ZLIB_LIBRARY})
target_compile_features(${PROJECT_NAME_STR} PRIVATE cxx_range_for)
target_compile_features(${PROJECT_NAME_STR}static PRIVATE cxx_range_for)

if(WIN32)
    target_link_libraries (${PROJECT_NAME_STR} ws2_32.lib)
    set_target_properties(${PROJECT_NAME_STR} PROPERTIES COMPILE_FLAGS "/wd4251")
endif (WIN32)

install(TARGETS ${PROJECT_NAME_STR} ${PROJECT_NAME_STR}static
    RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
    ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
install(FILES ${GELFLIB_PUBLIC_HDRS} DESTINATION "${INSTALL_INC_DIR}")

add_subdirectory(${EXT_PROJECTS_DIR}/gtest)

#-------------------
# Example
#-------------------

add_definitions(${MSVC_COMPILER_DEFS})
set(PROJECT_EXAMPLE example)
add_executable(${PROJECT_EXAMPLE} example/main.cpp)
target_compile_features(${PROJECT_EXAMPLE} PRIVATE cxx_range_for)

target_link_libraries (${PROJECT_EXAMPLE} ${PROJECT_NAME_STR}static)
target_link_libraries (${PROJECT_EXAMPLE} ${ZLIB_LIBRARY})

if(WIN32 OR MINGW)
    target_link_libraries (${PROJECT_EXAMPLE} ws2_32.lib)
endif()

#-------------------
# Test
#-------------------
link_directories(${PROJECT_NAME_STR}static)
add_definitions(${MSVC_COMPILER_DEFS})
enable_testing()
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES})
include_directories(${GTEST_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src)

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest)
target_compile_features(${PROJECT_TEST_NAME} PRIVATE cxx_range_for)

if(NOT WIN32 OR MINGW)
    target_link_libraries(${PROJECT_TEST_NAME}
        ${GTEST_LIBS_DIR}/libgtest.a
        ${GTEST_LIBS_DIR}/libgtest_main.a
        )
    target_link_libraries(${PROJECT_TEST_NAME} ${CMAKE_THREAD_LIBS_INIT})
else()
    target_link_libraries(${PROJECT_TEST_NAME}
        debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
        optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
        )
    target_link_libraries(${PROJECT_TEST_NAME}
        debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
        optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
        )

    target_link_libraries (${PROJECT_TEST_NAME} ws2_32.lib)
    set_target_properties(${PROJECT_TEST_NAME} PROPERTIES COMPILE_FLAGS "/wd4251")

    add_custom_command(
            TARGET ${PROJECT_TEST_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                    ${ZLIB_ROOT}/bin/zlibd.dll
                  ${CMAKE_CURRENT_BINARY_DIR}/zlibd.dll)

    add_custom_command(
            TARGET ${PROJECT_TEST_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                    ${ZLIB_ROOT}/bin/zlib.dll
                  ${CMAKE_CURRENT_BINARY_DIR}/zlib.dll)
endif()

target_link_libraries (${PROJECT_TEST_NAME} ${ZLIB_LIBRARY})
target_link_libraries (${PROJECT_TEST_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (${PROJECT_TEST_NAME} ${PROJECT_NAME_STR}static)


add_test(test1 ${PROJECT_TEST_NAME})


