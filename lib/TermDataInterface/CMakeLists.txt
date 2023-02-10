# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (TermDataInterface LANGUAGES CXX)

find_package (Qt6 REQUIRED COMPONENTS Core)

add_library (TermDataInterface
    INTERFACE
        include/TermDataInterface/DataStorageInterface.h
        include/TermDataInterface/GroupSummary.h
        include/TermDataInterface/StaticGroupData.h
        include/TermDataInterface/GroupValidator.h
        include/TermDataInterface/TermData.h
        include/TermDataInterface/TermValidator.h)

target_include_directories (TermDataInterface INTERFACE "./include")

target_link_libraries (TermDataInterface INTERFACE CommonTools)
target_link_libraries (TermDataInterface INTERFACE Qt6::Core)

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

    set_target_properties (TermDataInterfaceTest
        PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

    target_link_libraries (TermDataInterfaceTest GTest::gtest GTest::gtest_main)
    target_link_libraries (TermDataInterfaceTest CommonTools TermDataInterface Qt6::Core)

    gtest_add_tests (TARGET TermDataInterfaceTest)
endif ()
