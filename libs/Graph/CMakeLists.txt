# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (Graph VERSION 1.0.0 LANGUAGES CXX)

add_library (Graph
    INTERFACE
        include/Graph/edge.hpp
        include/Graph/forest.hpp
        include/Graph/graph.hpp
        include/Graph/graphdata.hpp
        include/Graph/node.hpp)

target_include_directories (Graph INTERFACE "./include")


if (BUILD_TESTING)
    enable_testing ()

    find_package (Qt6 REQUIRED COMPONENTS Core)
    if (NOT GTest_FOUND)
        find_package (GTest REQUIRED)
    endif ()

    include (GoogleTest)

    add_executable (GraphTest)
    target_sources (GraphTest
        PRIVATE
            tests/edgetest.cpp
            tests/foresttest.cpp
            tests/graphtest.cpp
            tests/nodetest.cpp)

    set_target_properties (GraphTest
        PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

    target_link_libraries (GraphTest GTest::gtest GTest::gtest_main)
    target_link_libraries (GraphTest Graph Qt6::Core)

    gtest_add_tests (TARGET GraphTest)
endif ()
