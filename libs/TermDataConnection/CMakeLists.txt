#  TermGraph, build graph of knowledge.
#  Copyright © 2016-2020. Savenkov Igor. All rights reserved
#  Contacts: dev.savenkovigor@protonmail.com
#
#  This file is part of TermGraph.
#
#  TermGraph is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  TermGraph is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.

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

if (BUILD_TESTING)
    enable_testing ()

    create_test (
        NAME      TestConnectionWorks
        SOURCES   tests/ConnectionWorksTest.cpp
        LIBRARIES CommonTools TermDataStorage Qt6::Core)

endif ()
