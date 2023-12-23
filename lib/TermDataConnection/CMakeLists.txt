# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (TermDataConnection LANGUAGES CXX)

find_package (Qt6 REQUIRED COMPONENTS Core Network)

add_library (TermDataConnection STATIC)

target_link_libraries (TermDataConnection PRIVATE CommonTools)
target_link_libraries (TermDataConnection PRIVATE Qt6::Core Qt6::Network)
target_link_libraries (TermDataConnection PUBLIC  TermDataInterface)

target_include_directories (TermDataConnection INTERFACE "./include")
target_include_directories (TermDataConnection PRIVATE ${PROJECT_SOURCE_DIR})

# Sql queries
target_sources (TermDataConnection
    PRIVATE
        include/TermDataConnection/DataStorageConnection.h
        include/TermDataConnection/NetWrapper.h
        source/DataStorageConnection.cpp
        source/NetWrapper.cpp)
