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

cmake_minimum_required (VERSION 3.16.0)

project (TermGraphBack VERSION 0.1.0 LANGUAGES CXX)

find_package (Qt6 COMPONENTS Core REQUIRED)
find_package (restinio REQUIRED)

add_executable (${PROJECT_NAME})

set_target_properties (${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

## Source folder
file (GLOB_RECURSE ALL_SOURCES source/*.h source/*.hpp source/*.cpp)
target_sources (${PROJECT_NAME} PRIVATE ${ALL_SOURCES})

source_group (Cpp REGULAR_EXPRESSION "source\/.*\.(h|cpp)")

target_link_libraries (${PROJECT_NAME} PRIVATE CommonTools TermDataInterface TermDataStorage)
target_link_libraries (${PROJECT_NAME} PRIVATE Qt6::Core restinio::restinio)
