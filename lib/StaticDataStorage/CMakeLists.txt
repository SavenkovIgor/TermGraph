# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (StaticDataStorage LANGUAGES CXX)

find_package (Qt6 REQUIRED COMPONENTS Core)

add_library (StaticDataStorage STATIC)

target_link_libraries (StaticDataStorage PUBLIC Qt6::Core)
target_link_libraries (StaticDataStorage PUBLIC CommonTools)
target_link_libraries (StaticDataStorage PUBLIC TermDataInterface)

set_target_properties (StaticDataStorage PROPERTIES AUTORCC ON)

target_include_directories (StaticDataStorage INTERFACE "./include")
target_include_directories (StaticDataStorage PRIVATE ${PROJECT_SOURCE_DIR})

target_sources (StaticDataStorage
    INTERFACE
        include/StaticDataStorage/StaticDataStorage.h
    PRIVATE
        data/StaticData.qrc
        source/StaticDataStorage.cpp)

if (BUILD_TESTING)
    enable_testing ()

    find_package (Qt6 REQUIRED COMPONENTS Core)
    if (NOT GTest_FOUND)
        find_package (GTest REQUIRED)
    endif ()

    include (GoogleTest)

    add_executable (StaticDataStorageTest)
    target_sources (StaticDataStorageTest PRIVATE test/StaticStorageTest.cpp)

    target_link_libraries (StaticDataStorageTest GTest::gtest GTest::gtest_main)
    target_link_libraries (StaticDataStorageTest StaticDataStorage)

    gtest_add_tests (TARGET StaticDataStorageTest)
endif ()
