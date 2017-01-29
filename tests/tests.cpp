// tests.cpp : Defines the entry point for the console application.
//

#include "gtest/gtest.h"

#include "GELF.hpp"

GTEST_API_ int main( int argc, char** argv )
{
    gelf::initialize();

    std::cout << "Running main() from gmock_main.cc\n";
    testing::InitGoogleTest( &argc, argv );
    int exitCode = RUN_ALL_TESTS();

    gelf::destroy();

    return exitCode;
}
