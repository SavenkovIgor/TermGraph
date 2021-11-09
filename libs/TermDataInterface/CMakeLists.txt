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

# TODO: Maybe move uuids with tests to commonTools
# TODO: Why includes missed in project tree

cmake_minimum_required (VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

project (TermDataInterface VERSION 1.0.0 LANGUAGES CXX)

add_library (TermDataInterface
    INTERFACE
        include/TermDataInterface/DataStorageErrors.h
        include/TermDataInterface/DataStorageInterface.h
        include/TermDataInterface/GroupData.h
        include/TermDataInterface/GroupUuid.h
        include/TermDataInterface/SafeUuid.h
        include/TermDataInterface/TermData.h
        include/TermDataInterface/TermUuid.h)

target_include_directories (TermDataInterface INTERFACE "./include")

target_link_libraries (TermDataInterface INTERFACE CommonTools)

if (BUILD_TESTING)
    enable_testing ()
    find_package (Qt6 COMPONENTS Core REQUIRED)

    create_test (
        NAME      TestUuids
        SOURCES
            include/TermDataInterface/SafeUuid.h
            include/TermDataInterface/GroupUuid.h
            include/TermDataInterface/TermUuid.h
            tests/uuidstest.cpp
        LIBRARIES CommonTools TermDataStorage Qt6::Core)

endif ()
