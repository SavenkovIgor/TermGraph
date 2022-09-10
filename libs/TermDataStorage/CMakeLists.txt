# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required (VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

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

    include (TestTools)
    create_test (
        NAME      TestDbWorks
        SOURCES   tests/DbWorksTest.cpp
        LIBRARIES CommonTools TermDataStorage Qt6::Core)

endif ()
