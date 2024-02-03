# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


cmake_minimum_required (VERSION 3.22.0)

project (TermGraph
         VERSION 2.0.0
         DESCRIPTION "Terms diagram builder. Polish your knowledge with accurate definitions."
         HOMEPAGE_URL "https://termgraph.app/"
         LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set (WASM "True")
endif ()

# Build options
option (BUILD_TESTING "Build tests with project" ON)
option (STATIC_STORAGE "Build with static storage" ON)

find_package (Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Network Svg)

qt_standard_project_setup(REQUIRES 6.6)

# Applications subdirs
add_subdirectory ("qml/Tools")
add_subdirectory ("qml/Theme")
add_subdirectory ("qml/Atoms")
add_subdirectory ("qml/Molecules")
add_subdirectory ("qml/Pages")

add_subdirectory ("lib/CommonTools")
add_subdirectory ("lib/Graph")
add_subdirectory ("lib/QrcHelpImages")
add_subdirectory ("lib/StaticDataStorage")
add_subdirectory ("lib/TermDataInterface")
add_subdirectory ("lib/Text")

if (NOT WASM)
    add_subdirectory ("lib/TermDataConnection")
    add_subdirectory ("lib/TermDataStorage")
endif ()

qt_add_executable (TermGraph)
target_sources (TermGraph
    PRIVATE
    "android/AndroidManifest.xml"
    "source/api.cpp"
    "source/api.h"
    "source/dataprovider.cpp"
    "source/dataprovider.h"
    "source/datasourcefactory.cpp"
    "source/datasourcefactory.h"
    "source/helpers/appsettings.cpp"
    "source/helpers/appsettings.h"
    "source/helpers/appstyle.h"
    "source/helpers/exclusivecall.h"
    "source/helpers/firststartpreparer.cpp"
    "source/helpers/firststartpreparer.h"
    "source/helpers/linktermdistancecache.cpp"
    "source/helpers/linktermdistancecache.h"
    "source/helpers/intmatrix.cpp"
    "source/helpers/intmatrix.h"
    "source/helpers/link/link.cpp"
    "source/helpers/link/link.h"
    "source/helpers/link/linksdecorator.cpp"
    "source/helpers/link/linksdecorator.h"
    "source/helpers/link/linksstring.cpp"
    "source/helpers/link/linksstring.h"
    "source/helpers/link/linktools.cpp"
    "source/helpers/link/linktools.h"
    "source/helpers/simplelistenserver.cpp"
    "source/helpers/simplelistenserver.h"
    "source/main.cpp"
    "source/mainscene.cpp"
    "source/mainscene.h"
    "source/managers/groupsmanager.cpp"
    "source/managers/groupsmanager.h"
    "source/managers/linkshardeningmanager.cpp"
    "source/managers/linkshardeningmanager.h"
    "source/managers/networkmanager.cpp"
    "source/managers/networkmanager.h"
    "source/managers/notificationmanager.cpp"
    "source/managers/notificationmanager.h"
    "source/managers/notifyinterface.h"
    "source/managers/syncmanager.cpp"
    "source/managers/syncmanager.h"
    "source/model/edge/edgedata.h"
    "source/model/edge/edgesmodel.cpp"
    "source/model/edge/edgesmodel.h"
    "source/model/edge/graphedge.cpp"
    "source/model/edge/graphedge.h"
    "source/model/edge/paintededge.cpp"
    "source/model/edge/paintededge.h"
    "source/model/enums.h"
    "source/model/graphicitem/graphicitem.cpp"
    "source/model/graphicitem/graphicitem.h"
    "source/model/graphicitem/rectgraphicitem.cpp"
    "source/model/graphicitem/rectgraphicitem.h"
    "source/model/group/groupsummarywrapper.h"
    "source/model/group/nodeverticalstack.cpp"
    "source/model/group/nodeverticalstack.h"
    "source/model/group/paintedforest.cpp"
    "source/model/group/paintedforest.h"
    "source/model/group/termgroup.cpp"
    "source/model/group/termgroup.h"
    "source/model/term/graphterm.cpp"
    "source/model/term/graphterm.h"
    "source/model/term/paintedterm.cpp"
    "source/model/term/paintedterm.h"
    "source/model/term/termdatacache.cpp"
    "source/model/term/termdatacache.h"
    "source/model/term/termdatawrapper.h"
    "source/model/term/termsmodel.cpp"
    "source/model/term/termsmodel.h"
)

# Define target properties for Android with Qt 6 as:
if (ANDROID)
    set_target_properties (TermGraph
        PROPERTIES QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android)
endif ()

target_include_directories (
    TermGraph PRIVATE
        "${PROJECT_SOURCE_DIR}/source/"
        "${PROJECT_SOURCE_DIR}/source/helpers/link/"
        "${PROJECT_SOURCE_DIR}/source/managers/"
        "${PROJECT_SOURCE_DIR}/source/model/"
        "${PROJECT_SOURCE_DIR}/source/model/group/"
        "${PROJECT_SOURCE_DIR}/source/model/term/")

target_compile_definitions (
    TermGraph PRIVATE
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries (TermGraph
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Svg
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Network)

# C++ libs
target_link_libraries (TermGraph PRIVATE CommonTools)
target_link_libraries (TermGraph PRIVATE Graph)
target_link_libraries (TermGraph PRIVATE Text)
target_link_libraries (TermGraph PRIVATE TermDataInterface)
target_link_libraries (TermGraph PRIVATE StaticDataStorage)

if (NOT WASM)
    target_link_libraries (TermGraph PRIVATE TermDataConnection)
    target_link_libraries (TermGraph PRIVATE TermDataStorage)
endif ()

# Qrc libs
target_link_libraries (TermGraph PRIVATE HelpImagesResource)

# Qml libs
target_link_libraries (TermGraph
    PRIVATE
        ToolsModuleplugin
        ThemeModuleplugin
        AtomsModuleplugin
        MoleculesModuleplugin
        PagesModuleplugin)

qt_add_qml_module (TermGraph
    URI Api
    IMPORTS
        QtQml QtQuick QtQuick.Controls QtQuick.Controls.Material QtQuick.Shapes QtQuick.Layouts QtQuick.Window QtQuick.Templates
        Theme Tools Atoms Molecules Pages
    QML_FILES "qml/Main.qml")

if (WASM)
    set (DEPLOY_DIR "${CMAKE_BINARY_DIR}/deploy/")
    set (DEV_DEPLOY True)

    # Creating deploy folder
    add_custom_command(TARGET TermGraph
                       POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                       make_directory "${DEPLOY_DIR}")

    # Copy of static resources
    set (WASM_RESOURCES)
    list (APPEND WASM_RESOURCES "${CMAKE_SOURCE_DIR}/resources/applicationIcons/no_text_icon256x256.png")
    list (APPEND WASM_RESOURCES "${CMAKE_SOURCE_DIR}/resources/index.html")

    # Copy of build artefacts
    list (APPEND WASM_RESOURCES "${CMAKE_BINARY_DIR}/qtloader.js")
    list (APPEND WASM_RESOURCES "${CMAKE_BINARY_DIR}/TermGraph.js")
    list (APPEND WASM_RESOURCES "${CMAKE_BINARY_DIR}/TermGraph.wasm")

    foreach (WASM_RESOURCE ${WASM_RESOURCES})
        add_custom_command (TARGET TermGraph
                            POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                            copy "${WASM_RESOURCE}" "${DEPLOY_DIR}")
    endforeach ()

    if (DEV_DEPLOY)
        add_custom_command (TARGET TermGraph
                            POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                            copy "${CMAKE_SOURCE_DIR}/resources/index.html" "${CMAKE_BINARY_DIR}/TermGraph.html")
        add_custom_command (TARGET TermGraph
                            POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                            copy "${CMAKE_SOURCE_DIR}/resources/applicationIcons/no_text_icon256x256.png" "${CMAKE_BINARY_DIR}/no_text_icon256x256.png")
    endif ()
endif ()


if (BUILD_TESTING)
    enable_testing ()

    find_package (Qt6 REQUIRED COMPONENTS Core Gui Qml)
    if (NOT GTest_FOUND)
        find_package (GTest REQUIRED)
    endif ()

    include (GoogleTest)

    add_executable (TermGraphTest)
    target_sources (TermGraphTest
        PRIVATE
            source/helpers/intmatrix.cpp
            source/helpers/intmatrix.h
            source/helpers/link/link.cpp
            source/helpers/link/link.h
            source/helpers/link/linksdecorator.cpp
            source/helpers/link/linksdecorator.h
            source/helpers/link/linksstring.cpp
            source/helpers/link/linksstring.h
            source/helpers/link/linktools.cpp
            source/helpers/link/linktools.h
            source/model/enums.h
            source/model/graphicitem/graphicitem.cpp
            source/model/graphicitem/graphicitem.h
            test/graphicitemtest.cpp
            test/linkdecoratortest.cpp
            test/linktest.cpp
            test/linktoolstest.cpp
        )

    target_include_directories (TermGraphTest PRIVATE ${PROJECT_SOURCE_DIR})
    target_link_libraries (TermGraphTest GTest::gtest GTest::gtest_main)
    target_link_libraries (TermGraphTest Qt6::Core Qt6::Gui Qt6::Qml)
    target_link_libraries (TermGraphTest CommonTools Graph Text)

    gtest_add_tests (TARGET TermGraphTest)
endif ()
