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

project (TermDataStorage VERSION 1.0.0 LANGUAGES CXX)

find_package (Qt6 COMPONENTS Core Sql REQUIRED)

add_library (TermDataStorage STATIC)

target_link_libraries (TermDataStorage PRIVATE CommonTools)
target_link_libraries (TermDataStorage PRIVATE Qt6::Core Qt6::Sql)
target_link_libraries (TermDataStorage PUBLIC  TermDataInterface)

set_target_properties (TermDataStorage
    PROPERTIES
        AUTORCC ON
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

target_include_directories (TermDataStorage INTERFACE "./include")
target_include_directories (TermDataStorage PRIVATE ${PROJECT_SOURCE_DIR})

# Sql queries
target_sources (TermDataStorage
    PRIVATE
        include/TermDataStorage/LocalDataStorage.h
        queries/SqlQueries.qrc
        source/Database.cpp
        source/Database.h
        source/DbInfo.h
        source/DbTools.cpp
        source/DbTools.h
        source/LocalDataStorage.cpp
        source/SqlQueryBuilder.cpp
        source/SqlQueryBuilder.h
        source/tables/AppConfigTable.cpp
        source/tables/AppConfigTable.h
        source/tables/TermGroupTable.cpp
        source/tables/TermGroupTable.h
        source/tables/TermTable.cpp
        source/tables/TermTable.h)

if (BUILD_TESTING)
    enable_testing ()

    create_test (
        NAME      TestDbWorks
        SOURCES   tests/DbWorksTest.cpp
        LIBRARIES CommonTools TermDataStorage Qt6::Core)

endif ()
