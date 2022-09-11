# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later

# Usage example
# create_test (NAME test SOURCES main.cpp LIBRARIES Qt6::Core)

function (create_test)

    find_package (GTest REQUIRED)


    set (options "")
    set (oneValueArgs NAME)
    set (multiValueArgs SOURCES LIBRARIES)
    cmake_parse_arguments (CREATE_TEST "${options}" "${oneValueArgs}"
                          "${multiValueArgs}" ${ARGN})


    add_executable ("${CREATE_TEST_NAME}" "${CREATE_TEST_SOURCES}")

    set_target_properties ("${CREATE_TEST_NAME}"
        PROPERTIES
            AUTOMOC ON
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON)

    target_include_directories (${CREATE_TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
    target_link_libraries (${CREATE_TEST_NAME} GTest::gtest GTest::gtest_main)

    target_link_libraries (${CREATE_TEST_NAME} "${CREATE_TEST_LIBRARIES}")

    add_test (NAME ${CREATE_TEST_NAME} COMMAND ${CREATE_TEST_NAME} CONFIGURATIONS Debug)

endfunction ()
