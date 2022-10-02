# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later

# TODO: use CMakePresets.json
# TODO: Join Qml modules to one CMakeLists file
# TODO: Make Android build
# TODO: Make big cleanup at qml imports

cmake_minimum_required (VERSION 3.22.0)

project (TermGraphPackage)

list (APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set (WASM "True")
endif ()

# Build options
option (BUILD_APPLICATION "Build native application" ON)
option (BUILD_BACKEND     "Build backend server"     OFF)
option (BUILD_FRONTEND    "Build frontend side"      OFF)

option (BUILD_TESTING     "Build tests with project" ON)

# Applications subdirs
if (BUILD_APPLICATION)
    add_subdirectory (libs/QmlTools)
    add_subdirectory (libs/QmlTheme)
    add_subdirectory (libs/QmlAtoms)
    add_subdirectory (libs/QmlMolecules)
    add_subdirectory (libs/QmlPages)
    add_subdirectory (libs/QrcHelpImages)
    add_subdirectory (libs/TermDataConnection)

    add_subdirectory (apps/TermGraphApp)
endif ()

if (BUILD_BACKEND)
    add_subdirectory (apps/TermGraphBack)
endif ()

if (BUILD_FRONTEND)
    add_subdirectory (libs/QmlTools)
    add_subdirectory (libs/QmlTheme)
    add_subdirectory (libs/QmlAtoms)
    add_subdirectory (libs/QmlMolecules)
    add_subdirectory (libs/QmlPages)
    add_subdirectory (libs/QrcHelpImages)

    add_subdirectory (apps/TermGraphFront)
endif ()


add_subdirectory (libs/CommonTools)

if (BUILD_APPLICATION OR BUILD_BACKEND)
    add_subdirectory (libs/TermDataInterface)

    if (NOT WASM)
        add_subdirectory (libs/TermDataStorage)
    endif ()
endif ()
