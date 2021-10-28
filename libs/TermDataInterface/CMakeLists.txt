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

cmake_minimum_required (VERSION 3.16.0)

project (TermDataInterface VERSION 1.0.0 LANGUAGES CXX)

add_library (TermDataInterface INTERFACE)

target_include_directories (TermDataInterface INTERFACE "./include")

target_sources (TermDataInterface
    INTERFACE
        include/TermDataInterface/DataStorageErrors.h
        include/TermDataInterface/DataStorageInterface.h
        include/TermDataInterface/GroupData.h
        include/TermDataInterface/GroupUuid.h
        include/TermDataInterface/SafeUuid.h
        include/TermDataInterface/TermData.h
        include/TermDataInterface/TermUuid.h)

target_link_libraries (TermDataInterface INTERFACE CommonTools)

if (BUILD_TESTING)
    enable_testing ()
    find_package (GTest REQUIRED)
    find_package (Qt6 COMPONENTS Core REQUIRED)

    # TestUuids
    add_executable (TestUuids
        include/TermDataInterface/SafeUuid.h
        include/TermDataInterface/GroupUuid.h
        include/TermDataInterface/TermUuid.h
        tests/uuidstest.cpp)

    set_target_properties (TestUuids PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

    target_link_libraries (TestUuids
        CommonTools
        TermDataInterface
        Qt6::Core
        GTest::gtest
        GTest::gtest_main)

    add_test (NAME TestUuids COMMAND TestUuids CONFIGURATIONS Debug)

endif ()
