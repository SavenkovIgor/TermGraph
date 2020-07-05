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

find_package (Qt5Test REQUIRED)

set (TEST_PROJECT_NAME ${PROJECT_NAME}_link_utils_test)

add_executable (${TEST_PROJECT_NAME})

include_directories (${CMAKE_SOURCE_DIR})

target_sources (
    ${TEST_PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/source/Helpers/linkutils.cpp
            ${CMAKE_SOURCE_DIR}/source/Helpers/linkutils.h
            ${CMAKE_SOURCE_DIR}/source/Helpers/intmatrix.cpp
            ${CMAKE_SOURCE_DIR}/source/Helpers/intmatrix.h
            ${CMAKE_SOURCE_DIR}/source/Helpers/strrange.cpp
            ${CMAKE_SOURCE_DIR}/source/Helpers/strrange.h
            linkutilstest.cpp)

add_test (NAME ${TEST_PROJECT_NAME} COMMAND ${TEST_PROJECT_NAME})

target_link_libraries (${TEST_PROJECT_NAME} PRIVATE Qt5::Test)
