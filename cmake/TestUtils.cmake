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
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON)

    target_include_directories (${CREATE_TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
    target_link_libraries (${CREATE_TEST_NAME} GTest::gtest GTest::gtest_main)

    target_link_libraries (${CREATE_TEST_NAME} "${CREATE_TEST_LIBRARIES}")

    add_test (NAME ${CREATE_TEST_NAME} COMMAND ${CREATE_TEST_NAME} CONFIGURATIONS Debug)

endfunction ()
