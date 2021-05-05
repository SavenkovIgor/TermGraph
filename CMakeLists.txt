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

project (
    TermGraph
    VERSION 2.0.0
    LANGUAGES CXX)

option (BUILD_TESTING "Build tests with project" ON)

if (ANDROID)
    set (BUILD_TESTING OFF)
endif ()

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (QML_PATH ${CMAKE_CURRENT_SOURCE_DIR}/qml)
set (QML_IMPORT_PATH "${QML_PATH}" CACHE STRING "" FORCE)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

if (ANDROID)
    set (ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    #    if (ANDROID_ABI STREQUAL "armeabi-v7a")
    #        set(ANDROID_EXTRA_LIBS
    #            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
    #            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
    #    endif()
endif ()

# If you want, android build correctly,
# This line must be before targets definition
find_package (Qt6 COMPONENTS Core Qml Quick QuickControls2 Sql Network Svg REQUIRED)

if (ANDROID)
    add_library (${PROJECT_NAME} SHARED)
else ()
    add_executable (${PROJECT_NAME})
endif ()

set_target_properties (${PROJECT_NAME} PROPERTIES
    AUTOUIC ON
    AUTOMOC ON
    AUTORCC ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON)

# Resources folder
target_sources (${PROJECT_NAME} PRIVATE resources/helpImages/helpImages.qrc resources/icons/icons.qrc)

# Qml folder
target_sources (${PROJECT_NAME} PRIVATE qml/qml.qrc)

# Sql queries
target_sources (${PROJECT_NAME} PRIVATE source/Database/sqlqueries.qrc)

# Some non compilable files
target_sources (
    ${PROJECT_NAME}
    PRIVATE android/AndroidManifest.xml
            android/res/drawable-hdpi/icon.png
            android/res/drawable-mdpi/icon.png
            android/res/drawable-ldpi/icon.png
            .devTools/goodcheck.yml
            .devTools/asterisklicense.yml
            .devTools/hashlicense.yml
            .devTools/includerules.yml
            .devTools/nodebug.yml
            .devTools/qmlimport.yml
            .devTools/cmakeFormat.json)

file (GLOB_RECURSE ALL_SOURCES source/*.h source/*.hpp source/*.cpp)

# Source folder
target_sources (${PROJECT_NAME} PRIVATE ${ALL_SOURCES})

source_group (Sql REGULAR_EXPRESSION ".*\.sql")
source_group (Cpp REGULAR_EXPRESSION "source\/.*\.(h|cpp)")
source_group (DevTools REGULAR_EXPRESSION ".devTools\/.*")
source_group (Qml REGULAR_EXPRESSION "qml\/")

target_compile_definitions (${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries (
    ${PROJECT_NAME}
    PRIVATE Qt6::Core
            Qt6::Quick
            Qt6::QuickControls2
            Qt6::Qml
            Qt6::Sql
            Qt6::Network
            Qt6::Svg)

if (BUILD_TESTING)
    add_subdirectory (tests)
endif ()
