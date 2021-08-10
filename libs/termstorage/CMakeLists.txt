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

cmake_minimum_required (VERSION 3.16.0)

project (TermStorage VERSION 1.0.0 LANGUAGES CXX)

find_package (Qt6 COMPONENTS Core Sql REQUIRED)

add_library (${PROJECT_NAME} STATIC)

target_link_libraries (${PROJECT_NAME}
    PRIVATE
        CommonTools
    PUBLIC
        Qt6::Core
        Qt6::Sql
        TermDataInterface)

set_target_properties (${PROJECT_NAME}
    PROPERTIES
        AUTORCC ON
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

target_include_directories (${PROJECT_NAME} PUBLIC "./include")
target_include_directories (${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})

# Sql queries
target_sources (${PROJECT_NAME}
    PRIVATE
        include/termstorage/appconfigtable.h
        include/termstorage/database.h
        include/termstorage/localdatabasestorage.h
        include/termstorage/termgrouptable.h
        include/termstorage/termtable.h
        queries/sqlqueries.qrc
        source/database.cpp
        source/dbinfo.h
        source/dbtools.cpp
        source/dbtools.h
        source/localdatabasestorage.cpp
        source/sqlquerybuilder.cpp
        source/sqlquerybuilder.h
        source/tables/appconfigtable.cpp
        source/tables/termgrouptable.cpp
        source/tables/termtable.cpp)

enable_testing ()
find_package (GTest REQUIRED)

# DB_WORKS_TEST
add_executable (DB_WORKS_TEST tests/dbworkstest.cpp)

set_target_properties (DB_WORKS_TEST PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

#target_include_directories (DB_WORKS_TEST PRIVATE ${GTest_INCLUDE_DIRS})
target_link_libraries (DB_WORKS_TEST
    CommonTools
    TermStorage
    Qt6::Core
    GTest::gtest
    GTest::gtest_main)

