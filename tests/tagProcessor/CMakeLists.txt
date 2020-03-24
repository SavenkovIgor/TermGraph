#  TermGraph, build graph of knowledge.
#  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

add_executable (${PROJECT_NAME}_tag_processor_test)

include_directories (${CMAKE_SOURCE_DIR})

target_sources (${PROJECT_NAME}_tag_processor_test
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source/Helpers/tagprocessor.cpp
        ${CMAKE_SOURCE_DIR}/source/Helpers/tagprocessor.h
        ${CMAKE_SOURCE_DIR}/source/Helpers/intmatrix.cpp
        ${CMAKE_SOURCE_DIR}/source/Helpers/intmatrix.h

        tagprocessortest.cpp
)

add_test (
    NAME    ${PROJECT_NAME}_tag_processor_test
    COMMAND ${PROJECT_NAME}_tag_processor_test
)

target_link_libraries (${PROJECT_NAME}_tag_processor_test PRIVATE Qt5::Test)
