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

cmake_minimum_required (VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

project (CommonTools VERSION 1.0.0 LANGUAGES CXX)

find_package (Qt6 COMPONENTS Core REQUIRED)

if (ANDROID OR WASM)
    set (outcome_DIR "${CMAKE_BINARY_DIR}/conan-dependencies")
endif ()

find_package (outcome REQUIRED)

add_library (CommonTools
    INTERFACE
        include/CommonTools/GroupUuid.h
        include/CommonTools/HandyTypes.h
        include/CommonTools/JsonTools.h
        include/CommonTools/NetworkTools.h
        include/CommonTools/Platform.h
        include/CommonTools/SafeUuid.h
        include/CommonTools/TermUuid.h
        include/CommonTools/Validator.h)

target_include_directories (CommonTools INTERFACE "./include")

target_link_libraries (CommonTools INTERFACE Qt6::Core)
target_link_libraries (CommonTools INTERFACE outcome::outcome)

if (BUILD_TESTING)
    list (APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
    include (TestUtils)

    enable_testing ()
    find_package (Qt6 COMPONENTS Core REQUIRED)

    create_test (
        NAME      TestUuids
        SOURCES
            include/CommonTools/SafeUuid.h
            include/CommonTools/GroupUuid.h
            include/CommonTools/TermUuid.h
            tests/uuidstest.cpp
        LIBRARIES CommonTools TermDataStorage Qt6::Core)

    create_test (
        NAME      TestValidator
        SOURCES
            include/CommonTools/Validator.h
            tests/validatortest.cpp
        LIBRARIES CommonTools TermDataStorage Qt6::Core)

endif ()
