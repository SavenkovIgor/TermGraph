# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


find_package (Qt6 REQUIRED COMPONENTS Core Sql)

add_library (TermDataStorage STATIC)

set_target_properties (TermDataStorage
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

target_link_libraries (TermDataStorage PRIVATE CommonTools)
target_link_libraries (TermDataStorage PRIVATE Qt6::Core Qt6::Sql)
target_link_libraries (TermDataStorage PUBLIC  TermDataInterface)

set_target_properties (TermDataStorage PROPERTIES AUTORCC ON)

target_include_directories (TermDataStorage INTERFACE "./include")
target_include_directories (TermDataStorage PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources (TermDataStorage
    INTERFACE
        include/TermDataStorage/LocalDataStorage.h
    PRIVATE
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

    find_package (Qt6 REQUIRED COMPONENTS Core)
    if (NOT GTest_FOUND)
        find_package (GTest REQUIRED)
    endif ()

    include (GoogleTest)

    add_executable (TermDataStorageTest)
    set_target_properties (TermDataStorageTest
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

    target_sources (TermDataStorageTest PRIVATE test/DbWorksTest.cpp)

    target_link_libraries (TermDataStorageTest GTest::gtest GTest::gtest_main)
    target_link_libraries (TermDataStorageTest CommonTools TermDataStorage Qt6::Core)

    gtest_add_tests (TARGET TermDataStorageTest)
endif ()
