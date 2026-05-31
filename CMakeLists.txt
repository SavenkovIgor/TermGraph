# Copyright © 2016-2025. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


cmake_minimum_required (VERSION 4.0)

project (TermGraph
         VERSION 2.0.0
         DESCRIPTION "Terms diagram builder. Polish your knowledge with accurate definitions."
         HOMEPAGE_URL "https://termgraph.app/"
         LANGUAGES CXX)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_SCAN_FOR_MODULES False)

include (cmake/Warnings.cmake)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set (WASM "True")
endif ()

# Build options
option (BUILD_TESTING "Build tests with project" ON)
option (STATIC_STORAGE "Build with static storage" ON)

set(QT_QML_GENERATE_QMLLS_INI ON)

find_package (Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Network Svg)

qt_standard_project_setup(REQUIRES 6.8)

# Applications subdirs
add_subdirectory ("source/Atoms")
add_subdirectory ("source/Molecules")
add_subdirectory ("source/Tools")
add_subdirectory ("source/Theme")
add_subdirectory ("source/Pages")

qt_add_executable (TermGraph)

set_target_properties (TermGraph
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_SCAN_FOR_MODULES TRUE
)

set_target_properties (TermGraph PROPERTIES AUTORCC ON)

target_sources (TermGraph
    PRIVATE
        "data/Data.qrc"
        "source/Api.cpp"
        "source/Api.hpp"
        "source/CommonTools/Errors.hpp"
        "source/CommonTools/GroupUuid.cpp"
        "source/CommonTools/GroupUuid.hpp"
        "source/CommonTools/HandyTypes.hpp"
        "source/CommonTools/Platform.hpp"
        "source/CommonTools/SafeUuid.hpp"
        "source/CommonTools/TermUuid.cpp"
        "source/CommonTools/TermUuid.hpp"
        "source/DataProvider.cpp"
        "source/DataProvider.hpp"
        "source/enums/qml_edge_selection.hpp"
        "source/enums/qml_edge_type.hpp"
        "source/graph/NodeType.hpp"
        "source/helpers/exclusivecall.hpp"
        "source/helpers/firststartpreparer.cpp"
        "source/helpers/firststartpreparer.hpp"
        "source/helpers/link/QtLinkTools.cpp"
        "source/helpers/link/QtLinkTools.hpp"
        "source/helpers/simplelistenserver.cpp"
        "source/helpers/simplelistenserver.hpp"
        "source/helpers/wiki/WikiUrlChecker.cpp"
        "source/helpers/wiki/WikiUrlChecker.hpp"
        "source/main.cpp"
        "source/MainScene.cpp"
        "source/MainScene.hpp"
        "source/managers/groupsmanager.cpp"
        "source/managers/groupsmanager.hpp"
        "source/managers/linkshardeningmanager.cpp"
        "source/managers/linkshardeningmanager.hpp"
        "source/managers/networkmanager.cpp"
        "source/managers/networkmanager.hpp"
        "source/managers/notificationmanager.cpp"
        "source/managers/notificationmanager.hpp"
        "source/managers/notifyinterface.hpp"
        "source/model/edge/edgesmodel.cpp"
        "source/model/edge/edgesmodel.hpp"
        "source/model/enums.cpp"
        "source/model/enums.hpp"
        "source/model/group/groupsummarywrapper.hpp"
        "source/model/group/qttermgroup.cpp"
        "source/model/group/qttermgroup.hpp"
        "source/model/term/termdatawrapper.hpp"
        "source/model/term/termsmodel.cpp"
        "source/model/term/termsmodel.hpp"
        "source/TermDataInterface/DataStorageInterface.hpp"
        "source/TermDataInterface/groupsummary.cpp"
        "source/TermDataInterface/GroupSummary.hpp"
        "source/TermDataInterface/termdata.cpp"
        "source/TermDataInterface/TermData.hpp"
    PUBLIC
    FILE_SET CXX_MODULES
    FILES
        "source/CommonTools/HandyTypesModule.cpp"
        "source/CommonTools/JsonKeys.cpp"
        "source/CommonTools/JsonTools.cpp"
        "source/CommonTools/NetworkTools.cpp"
        "source/CommonTools/Validator.cpp"
        "source/DataSourceFactory.cpp"
        "source/enums/coord_type.cpp"
        "source/enums/edge_selection.cpp"
        "source/enums/edge_type.cpp"
        "source/graph/Edge.cpp"
        "source/graph/Forest.cpp"
        "source/graph/Graph.cpp"
        "source/graph/GraphData.cpp"
        "source/graph/GraphModule.cpp"
        "source/graph/Node.cpp"
        "source/helpers/AppSettings.cpp"
        "source/helpers/appstyle.cpp"
        "source/helpers/intmatrix.cpp"
        "source/helpers/link/Link.cpp"
        "source/helpers/link/LinkModule.cpp"
        "source/helpers/link/LinksDecorator.cpp"
        "source/helpers/link/LinksString.cpp"
        "source/helpers/link/LinkTools.cpp"
        "source/helpers/linktermdistancecache.cpp"
        "source/helpers/stemming/Stemmer.cpp"
        "source/model/edge/edgedata.cpp"
        "source/model/edge/graphedge.cpp"
        "source/model/edge/paintededge.cpp"
        "source/model/graphicitem/graphicitem.cpp"
        "source/model/graphicitem/rectgraphicitem.cpp"
        "source/model/group/nodeverticalstack.cpp"
        "source/model/group/nodeverticalstacktools.cpp"
        "source/model/group/paintedforest.cpp"
        "source/model/group/termgroup.cpp"
        "source/model/group/termpositioner.cpp"
        "source/model/term/graphterm.cpp"
        "source/model/term/paintedterm.cpp"
        "source/model/term/termdatacache.cpp"
        "source/staticDataStorage/StaticDataStorage.cpp"
        "source/TermDataInterface/GroupValidator.cpp"
        "source/TermDataInterface/staticgroupdata.cpp"
        "source/TermDataInterface/TermValidator.cpp"
        "source/Text/CharTools.cpp"
        "source/Text/CheckingTextCursor.cpp"
        "source/Text/Fonts.cpp"
        "source/Text/TextCursor.cpp"
        "source/Text/TextModule.cpp"
        "source/Text/TextRange.cpp"
        "source/Text/TextTools.cpp"
)

# Add platform-specific sources
if (WASM)
    target_include_directories (TermGraph PRIVATE "${PROJECT_SOURCE_DIR}/source/CommonTools/wasm/")
    target_sources (TermGraph PRIVATE "source/CommonTools/wasm/PlatformTypes.hpp")
else ()
    target_include_directories (TermGraph PRIVATE "${PROJECT_SOURCE_DIR}/source/CommonTools/desktop/")
    target_sources (TermGraph PRIVATE "source/CommonTools/desktop/PlatformTypes.hpp")
endif ()

if (NOT WASM)
    target_sources (TermGraph PRIVATE "resources/HelpImages/HelpImages.qrc")
endif ()

target_include_directories (
    TermGraph PRIVATE
        "${PROJECT_SOURCE_DIR}"
        "${PROJECT_SOURCE_DIR}/source/"
        "${PROJECT_SOURCE_DIR}/source/enums/"
        "${PROJECT_SOURCE_DIR}/source/helpers/link/"
        "${PROJECT_SOURCE_DIR}/source/helpers/stemming/"
        "${PROJECT_SOURCE_DIR}/source/managers/"
        "${PROJECT_SOURCE_DIR}/source/model/"
        "${PROJECT_SOURCE_DIR}/source/model/group/"
        "${PROJECT_SOURCE_DIR}/source/model/term/"
)

target_include_directories (TermGraph SYSTEM PRIVATE
    "${PROJECT_SOURCE_DIR}/third_party/stemming/src")

if (STATIC_STORAGE)
    target_compile_definitions (TermGraph PRIVATE STATIC_STORAGE)
endif ()

target_compile_definitions (
    TermGraph PRIVATE
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries (TermGraph
    PRIVATE
        project_warnings
        Qt6::Core
        Qt6::Gui
        Qt6::Svg
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Network)

if (NOT WASM)
    target_sources (TermGraph
        PRIVATE
            "source/TermDataConnection/DataStorageConnection.cpp"
            "source/TermDataConnection/DataStorageConnection.hpp"
            "source/TermDataConnection/NetWrapper.cpp"
            "source/TermDataConnection/NetWrapper.hpp"
    )

    find_package (Qt6 REQUIRED COMPONENTS Sql)
    target_link_libraries (TermGraph PRIVATE Qt6::Sql)

    target_sources (TermGraph PRIVATE "source/TermDataStorage/queries/SqlQueries.qrc")

    # List of sources containing modules
    target_sources (TermGraph
        PUBLIC
        FILE_SET CXX_MODULES
        FILES
            "source/TermDataStorage/Database.cpp"
            "source/TermDataStorage/DbInfo.cpp"
            "source/TermDataStorage/DbTools.cpp"
            "source/TermDataStorage/LocalDataStorage.cpp"
            "source/TermDataStorage/SqlQueryBuilder.cpp"
            "source/TermDataStorage/tables/AppConfigTable.cpp"
            "source/TermDataStorage/tables/TermGroupTable.cpp"
            "source/TermDataStorage/tables/TermTable.cpp"
    )
endif ()

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
    QML_FILES "source/Main.qml")

if (WASM)
    # The install folder is inside the build directory
    set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/dist")

    # Install static resources
    install (FILES
        "${CMAKE_SOURCE_DIR}/resources/applicationIcons/no_text_icon256x256.png"
        "${CMAKE_SOURCE_DIR}/resources/index.html"
        DESTINATION ".")

    # Install WASM build artifacts (these are generated during build)
    install (FILES
        "${CMAKE_BINARY_DIR}/qtloader.js"
        "${CMAKE_BINARY_DIR}/TermGraph.js"
        "${CMAKE_BINARY_DIR}/TermGraph.wasm"
        DESTINATION ".")
endif ()


if (BUILD_TESTING)
    enable_testing ()

    find_package (Qt6 REQUIRED COMPONENTS Core Gui Qml Sql)
    if (NOT GTest_FOUND)
        find_package (GTest REQUIRED)
    endif ()

    include (GoogleTest)

    add_executable (TermGraphTest)

    set_target_properties (TermGraphTest
        PROPERTIES
            CXX_STANDARD 23
            CXX_STANDARD_REQUIRED ON
            CXX_SCAN_FOR_MODULES TRUE
    )

    # Application sources
    target_sources (TermGraphTest
        PRIVATE
            "data/Data.qrc"
            "source/CommonTools/Errors.hpp"
            "source/CommonTools/GroupUuid.cpp"
            "source/CommonTools/GroupUuid.hpp"
            "source/CommonTools/HandyTypes.hpp"
            "source/CommonTools/Platform.hpp"
            "source/CommonTools/SafeUuid.hpp"
            "source/CommonTools/TermUuid.cpp"
            "source/CommonTools/TermUuid.hpp"
            "source/enums/qml_edge_selection.hpp"
            "source/enums/qml_edge_type.hpp"
            "source/graph/NodeType.hpp"
            "source/helpers/link/QtLinkTools.cpp"
            "source/helpers/link/QtLinkTools.hpp"
            "source/helpers/wiki/WikiUrlChecker.cpp"
            "source/helpers/wiki/WikiUrlChecker.hpp"
            "source/model/enums.cpp"
            "source/model/enums.hpp"
            "source/TermDataInterface/DataStorageInterface.hpp"
            "source/TermDataInterface/groupsummary.cpp"
            "source/TermDataInterface/GroupSummary.hpp"
            "source/TermDataInterface/termdata.cpp"
            "source/TermDataInterface/TermData.hpp"
            "source/TermDataStorage/queries/SqlQueries.qrc"
    )

    # List of sources containing modules
    target_sources (TermGraphTest
        PUBLIC
        FILE_SET CXX_MODULES
        FILES
            "source/CommonTools/HandyTypesModule.cpp"
            "source/CommonTools/JsonKeys.cpp"
            "source/CommonTools/JsonTools.cpp"
            "source/CommonTools/NetworkTools.cpp"
            "source/CommonTools/Validator.cpp"
            "source/enums/coord_type.cpp"
            "source/enums/edge_selection.cpp"
            "source/enums/edge_type.cpp"
            "source/graph/Edge.cpp"
            "source/graph/Forest.cpp"
            "source/graph/Graph.cpp"
            "source/graph/GraphData.cpp"
            "source/graph/Node.cpp"
            "source/helpers/intmatrix.cpp"
            "source/helpers/link/Link.cpp"
            "source/helpers/link/LinksDecorator.cpp"
            "source/helpers/link/LinksString.cpp"
            "source/helpers/link/LinkTools.cpp"
            "source/helpers/stemming/Stemmer.cpp"
            "source/model/graphicitem/graphicitem.cpp"
            "source/staticDataStorage/StaticDataStorage.cpp"
            "source/TermDataInterface/GroupValidator.cpp"
            "source/TermDataInterface/staticgroupdata.cpp"
            "source/TermDataInterface/TermValidator.cpp"
            "source/TermDataStorage/Database.cpp"
            "source/TermDataStorage/DbInfo.cpp"
            "source/TermDataStorage/DbTools.cpp"
            "source/TermDataStorage/LocalDataStorage.cpp"
            "source/TermDataStorage/SqlQueryBuilder.cpp"
            "source/TermDataStorage/tables/AppConfigTable.cpp"
            "source/TermDataStorage/tables/TermGroupTable.cpp"
            "source/TermDataStorage/tables/TermTable.cpp"
            "source/Text/CharTools.cpp"
            "source/Text/CheckingTextCursor.cpp"
            "source/Text/Fonts.cpp"
            "source/Text/TextCursor.cpp"
            "source/Text/TextModule.cpp"
            "source/Text/TextRange.cpp"
            "source/Text/TextTools.cpp"
        )

    # Test sources
    target_sources (TermGraphTest
        PRIVATE
            "test/cursortest.cpp"
            # "test/DbWorksTest.cpp" NOTE: Disabled during Qt 6.7 migration
            "test/edgetest.cpp"
            "test/foresttest.cpp"
            "test/graphicitemtest.cpp"
            "test/graphtest.cpp"
            "test/groupsummarytest.cpp"
            "test/linkdecoratortest.cpp"
            "test/linktest.cpp"
            "test/linktoolstest.cpp"
            "test/nodetest.cpp"
            "test/staticgroupdatatest.cpp"
            "test/StaticStorageTest.cpp"
            "test/termdatatest.cpp"
            "test/textrangetest.cpp"
            "test/texttoolstest.cpp"
            "test/uuidstest.cpp"
            "test/wikiurlcheckertest.cpp"
            "test/stemmingtest.cpp"
            "test/validatortest.cpp"
        )

    # Add platform-specific sources
    if (WASM)
        target_include_directories (TermGraphTest PRIVATE "${PROJECT_SOURCE_DIR}/source/CommonTools/wasm/")
        target_sources (TermGraphTest PRIVATE "source/CommonTools/wasm/PlatformTypes.hpp")
    else ()
        target_include_directories (TermGraphTest PRIVATE "${PROJECT_SOURCE_DIR}/source/CommonTools/desktop/")
        target_sources (TermGraphTest PRIVATE "source/CommonTools/desktop/PlatformTypes.hpp")
    endif ()

    target_include_directories (TermGraphTest PRIVATE ${PROJECT_SOURCE_DIR})
    target_include_directories (TermGraphTest SYSTEM PRIVATE
        "${PROJECT_SOURCE_DIR}/third_party/stemming/src")
    target_link_libraries (TermGraphTest
        PRIVATE
            # Warnings
            project_warnings

            # Qt
            Qt6::Core
            Qt6::Gui
            Qt6::Qml
            Qt6::Sql

            # GTest section
            GTest::gtest
            GTest::gtest_main
    )

    set_target_properties (TermGraphTest PROPERTIES AUTORCC ON)

    gtest_add_tests (TARGET TermGraphTest)
endif ()
