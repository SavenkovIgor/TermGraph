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

# TODO: use CMakePresets.json
# TODO: Split to qml in separate project plugins
# TODO: Optional sql build for WebAssembly
# TODO: Make network mode for application
# TODO: Make Android build

cmake_minimum_required (VERSION 3.16.0)

project (TermGraphPackage)

option (BUILD_TESTING "Build tests with project" ON)
option (BUILD_BACKEND "Build backend server"     OFF)

list (APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include (TestUtils)

if (ANDROID)
    set (BUILD_TESTING OFF)
endif ()

add_subdirectory (apps/TermGraphApp)
add_subdirectory (apps/TermGraphBack)

add_subdirectory (libs/CommonTools)
add_subdirectory (libs/TermDataInterface)
add_subdirectory (libs/TermDataStorage)
