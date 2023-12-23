# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (TermDataInterface LANGUAGES CXX)

find_package (Qt6 REQUIRED COMPONENTS Core)

add_library (TermDataInterface STATIC)

target_link_libraries (TermDataInterface PUBLIC CommonTools Text)
target_link_libraries (TermDataInterface PUBLIC Qt6::Core)

target_include_directories (TermDataInterface INTERFACE "./include")
target_include_directories (TermDataInterface PRIVATE ${PROJECT_SOURCE_DIR})

target_sources (TermDataInterface
    INTERFACE
        include/TermDataInterface/DataStorageInterface.h
        include/TermDataInterface/GroupSummary.h
        include/TermDataInterface/StaticGroupData.h
        include/TermDataInterface/GroupValidator.h
        include/TermDataInterface/TermData.h
        include/TermDataInterface/TermValidator.h
    PRIVATE
        source/groupsummary.cpp
        source/staticgroupdata.cpp
        source/termdata.cpp)

if (BUILD_TESTING)
    enable_testing ()

    find_package (Qt6 REQUIRED COMPONENTS Core)
    if (NOT GTest_FOUND)
        find_package (GTest REQUIRED)
    endif ()

    include (GoogleTest)

    add_executable (TermDataInterfaceTest)
    target_sources (TermDataInterfaceTest
        PRIVATE
            test/termdatatest.cpp
            test/groupsummarytest.cpp
            test/staticgroupdatatest.cpp)

    target_link_libraries (TermDataInterfaceTest GTest::gtest GTest::gtest_main)
    target_link_libraries (TermDataInterfaceTest CommonTools Text TermDataInterface Qt6::Core)

    gtest_add_tests (TARGET TermDataInterfaceTest)
endif ()
