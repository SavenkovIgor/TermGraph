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
# TODO: Optional sql build for WebAssembly
# TODO: Make network mode for application
# TODO: Make Android build

cmake_minimum_required (VERSION 3.16.0)

project (TermGraphPackage)

list (APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set (WASM "True")
endif ()

# Build options
option (BUILD_APPLICATION "Build native application" ON)
option (BUILD_BACKEND     "Build backend server"     OFF)
option (BUILD_FRONTEND    "Build frontend side"      OFF)

option (BUILD_TESTING     "Build tests with project" OFF)

# Applications subdirs
if (BUILD_APPLICATION)
    add_subdirectory (libs/QmlTools)
    add_subdirectory (libs/QmlStyleInfo)
    add_subdirectory (libs/QmlAtoms)
    add_subdirectory (libs/QmlMolecules)
    add_subdirectory (libs/QmlPages)
    add_subdirectory (libs/QrcHelpImages)
    add_subdirectory (libs/QrcIcons)
    if (NOT WASM)
        add_subdirectory (libs/TermDataConnection)
    endif ()

    add_subdirectory (apps/TermGraphApp)
endif ()

if (BUILD_BACKEND)
    add_subdirectory (apps/TermGraphBack)
endif ()

if (BUILD_FRONTEND)
    add_subdirectory (libs/QmlTools)
    add_subdirectory (libs/QmlStyleInfo)
    add_subdirectory (libs/QmlAtoms)
    add_subdirectory (libs/QmlMolecules)
    add_subdirectory (libs/QmlPages)
    add_subdirectory (libs/QrcHelpImages)
    add_subdirectory (libs/QrcIcons)

    add_subdirectory (apps/TermGraphFront)
endif ()


add_subdirectory (libs/CommonTools)

if (BUILD_APPLICATION OR BUILD_BACKEND)
    add_subdirectory (libs/TermDataInterface)

    if (NOT WASM)
        add_subdirectory (libs/TermDataStorage)
    endif ()
endif ()
