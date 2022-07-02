# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required (VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

project (TermDataConnection VERSION 1.0.0 LANGUAGES CXX)

find_package (Qt6 COMPONENTS Core Network REQUIRED)

add_library (TermDataConnection STATIC)

target_link_libraries (TermDataConnection PRIVATE CommonTools)
target_link_libraries (TermDataConnection PRIVATE Qt6::Core Qt6::Network)
target_link_libraries (TermDataConnection PUBLIC  TermDataInterface)

set_target_properties (TermDataConnection
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

target_include_directories (TermDataConnection INTERFACE "./include")
target_include_directories (TermDataConnection PRIVATE ${PROJECT_SOURCE_DIR})

# Sql queries
target_sources (TermDataConnection
    PRIVATE
        include/TermDataConnection/DataStorageConnection.h
        include/TermDataConnection/NetworkThread.h
        source/DataStorageConnection.cpp
        source/NetworkThread.cpp)

#if (BUILD_TESTING)
#    enable_testing ()

#    include (TestUtils)
#    create_test (
#        NAME      TestConnectionWorks
#        SOURCES   tests/ConnectionWorksTest.cpp
#        LIBRARIES CommonTools TermDataConnection Qt6::Core Qt6::Network)

#endif ()
