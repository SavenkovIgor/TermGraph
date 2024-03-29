# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


add_library (Graph INTERFACE)
set_target_properties (Graph
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)
target_sources (Graph
    INTERFACE
        include/Graph/Edge.hpp
        include/Graph/Forest.hpp
        include/Graph/Graph.hpp
        include/Graph/GraphData.hpp
        include/Graph/Node.hpp)

target_include_directories (Graph INTERFACE "./include")


if (BUILD_TESTING)
    enable_testing ()

    find_package (Qt6 REQUIRED COMPONENTS Core)
    if (NOT GTest_FOUND)
        find_package (GTest REQUIRED)
    endif ()

    include (GoogleTest)

    add_executable (GraphTest)
    set_target_properties (GraphTest
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)
    target_sources (GraphTest
        PRIVATE
            test/edgetest.cpp
            test/foresttest.cpp
            test/graphtest.cpp
            test/nodetest.cpp)

    target_link_libraries (GraphTest GTest::gtest GTest::gtest_main)
    target_link_libraries (GraphTest Graph Qt6::Core)

    gtest_add_tests (TARGET GraphTest)
endif ()
