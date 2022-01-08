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

project (TermGraph VERSION 2.0.0 LANGUAGES CXX)

find_package (Qt6 COMPONENTS Core Gui Qml Quick QuickControls2 Network REQUIRED)

qt_add_executable (TermGraph MANUAL_FINALIZATION)

set_target_properties (TermGraph
    PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)
#        QT_QML_MODULE_VERSION 1.0
#        QT_QML_MODULE_URI     Api)

# Define target properties for Android with Qt 6 as:
if (ANDROID)
    set_target_properties (TermGraph
        PROPERTIES QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android)
endif ()

# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

# qt6_qml_type_registration(TermGraph)

# Resources
target_sources (TermGraph PRIVATE "qml/qml.qrc")

# Android files
target_sources (TermGraph PRIVATE android/AndroidManifest.xml)

# Source folder
file (GLOB_RECURSE ALL_SOURCES source/*.h source/*.hpp source/*.cpp)
target_sources (TermGraph PRIVATE ${ALL_SOURCES})

target_include_directories (TermGraph PRIVATE ${PROJECT_SOURCE_DIR})


target_compile_definitions (TermGraph PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

set (QML_IMPORT_PATH
        ${CMAKE_CURRENT_BINARY_DIR}
        ${ToolsModule_BINARY_DIR}
        ${StyleInfoModule_BINARY_DIR}
        ${AtomsModule_BINARY_DIR}
        ${MoleculesModule_BINARY_DIR}
        ${PagesModule_BINARY_DIR}
    CACHE STRING "" FORCE)

# C++ libs
target_link_libraries (TermGraph PRIVATE CommonTools TermDataInterface TermDataStorage TermDataConnection)

# Qrc libs
target_link_libraries (TermGraph PRIVATE HelpImagesResource IconsResource)

# Qml libs
target_link_libraries (TermGraph
    PRIVATE
        ToolsModuleplugin
        StyleInfoModuleplugin
        AtomsModuleplugin
        MoleculesModuleplugin
        PagesModuleplugin)

target_link_libraries (TermGraph
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Qml
        Qt6::Network)

qt_finalize_executable (TermGraph)


if (BUILD_TESTING)
    include (TestUtils)
    create_test (
        NAME      TestTextUtils
        SOURCES
            source/helpers/text/textutils.cpp
            source/helpers/text/textutils.h
            tests/textutilstest.cpp
        LIBRARIES Qt6::Core)

    create_test (
        NAME      TestFsWorks
        SOURCES
            source/helpers/fsworks.cpp
            source/helpers/fsworks.h
            tests/fsworkstest.cpp
        LIBRARIES Qt6::Core)

    create_test (
        NAME      TestNode
        SOURCES
            source/model/base/node.hpp
            tests/nodetest.cpp
        LIBRARIES Qt6::Core)

    create_test (
        NAME      TestEdge
        SOURCES
            source/model/base/node.hpp
            source/model/base/edge.hpp
            tests/edgetest.cpp
        LIBRARIES Qt6::Core)

    create_test (
        NAME      TestGraph
        SOURCES
            source/model/base/node.hpp
            source/model/base/edge.hpp
            source/model/base/graph.hpp
            source/model/base/graphdata.hpp
            tests/graphtest.cpp
        LIBRARIES Qt6::Core)

    create_test (
        NAME      TestForest
        SOURCES
            source/model/base/node.hpp
            source/model/base/edge.hpp
            source/model/base/graph.hpp
            source/model/base/graphdata.hpp
            source/model/base/forest.hpp
            source/model/enums.h
            tests/foresttest.cpp
        LIBRARIES Qt6::Core)

    create_test (
        NAME      TestGraphicItem
        SOURCES
            source/model/graphicitem/graphicitem.h
            source/model/graphicitem/graphicitem.cpp
            tests/graphicitemtest.cpp
        LIBRARIES Qt6::Core)

    create_test (
        NAME      TestTextCursor
        SOURCES
            source/helpers/text/chartools.h
            source/helpers/text/checkingtextcursor.cpp
            source/helpers/text/checkingtextcursor.h
            source/helpers/text/textcursor.cpp
            source/helpers/text/textcursor.h
            tests/cursortest.cpp
        LIBRARIES Qt6::Core CommonTools)

    create_test (
        NAME      TestTextRange
        SOURCES
            source/helpers/intmatrix.cpp
            source/helpers/intmatrix.h
            source/helpers/link/link.cpp
            source/helpers/link/link.h
            source/helpers/text/chartools.h
            source/helpers/text/checkingtextcursor.cpp
            source/helpers/text/checkingtextcursor.h
            source/helpers/text/textcursor.cpp
            source/helpers/text/textcursor.h
            source/helpers/text/textrange.cpp
            source/helpers/text/textrange.h
            tests/textrangetest.cpp
        LIBRARIES Qt6::Core CommonTools)

    create_test (
        NAME      TestTextLink
        SOURCES
            source/helpers/intmatrix.cpp
            source/helpers/intmatrix.h
            source/helpers/link/linksstring.cpp
            source/helpers/link/linksstring.h
            source/helpers/link/link.cpp
            source/helpers/link/link.h
            source/helpers/text/chartools.h
            source/helpers/text/checkingtextcursor.cpp
            source/helpers/text/checkingtextcursor.h
            source/helpers/text/textcursor.cpp
            source/helpers/text/textcursor.h
            source/helpers/text/textrange.cpp
            source/helpers/text/textrange.h
            tests/linktest.cpp
        LIBRARIES Qt6::Core CommonTools)

    create_test (
        NAME      TestLinkUtils
        SOURCES
            source/helpers/intmatrix.cpp
            source/helpers/intmatrix.h
            source/helpers/link/linkutils.cpp
            source/helpers/link/linkutils.h
            source/helpers/link/link.cpp
            source/helpers/link/link.h
            source/helpers/text/chartools.h
            source/helpers/text/checkingtextcursor.cpp
            source/helpers/text/checkingtextcursor.h
            source/helpers/text/textcursor.cpp
            source/helpers/text/textcursor.h
            source/helpers/text/textrange.cpp
            source/helpers/text/textrange.h
            source/helpers/text/textutils.cpp
            source/helpers/text/textutils.h
            tests/linkutilstest.cpp
        LIBRARIES Qt6::Core CommonTools)

    create_test (
        NAME      TestLinkDecorator
        SOURCES
            source/helpers/intmatrix.cpp
            source/helpers/intmatrix.h
            source/helpers/link/linksdecorator.cpp
            source/helpers/link/linksdecorator.h
            source/helpers/link/linksstring.cpp
            source/helpers/link/linksstring.h
            source/helpers/link/link.cpp
            source/helpers/link/link.h
            source/helpers/text/chartools.h
            source/helpers/text/checkingtextcursor.cpp
            source/helpers/text/checkingtextcursor.h
            source/helpers/text/textcursor.cpp
            source/helpers/text/textcursor.h
            source/helpers/text/textrange.cpp
            source/helpers/text/textrange.h
            tests/linkdecoratortest.cpp
        LIBRARIES Qt6::Core Qt6::Gui CommonTools)

endif ()
