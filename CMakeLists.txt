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

# Enable ranges support for emscripten
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-fexperimental-library)
    add_compile_options(-fmodules)
endif()

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
add_subdirectory ("qml/Tools")
add_subdirectory ("qml/Theme")
add_subdirectory ("qml/Atoms")
add_subdirectory ("qml/Molecules")
add_subdirectory ("qml/Pages")

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
        "source/Api.h"
        "source/CommonTools/Errors.h"
        "source/CommonTools/GroupUuid.cpp"
        "source/CommonTools/GroupUuid.h"
        "source/CommonTools/HandyTypes.h"
        "source/CommonTools/Platform.h"
        "source/CommonTools/SafeUuid.h"
        "source/CommonTools/TermUuid.cpp"
        "source/CommonTools/TermUuid.h"
        "source/DataProvider.cpp"
        "source/DataProvider.h"
        "source/DataSourceFactory.cpp"
        "source/DataSourceFactory.h"
        "source/graph/Edge.hpp"
        "source/graph/Forest.hpp"
        "source/graph/Graph.hpp"
        "source/graph/GraphData.hpp"
        "source/graph/Node.hpp"
        "source/helpers/AppSettings.cpp"
        "source/helpers/AppSettings.h"
        "source/helpers/exclusivecall.h"
        "source/helpers/firststartpreparer.cpp"
        "source/helpers/firststartpreparer.h"
        "source/helpers/intmatrix.cpp"
        "source/helpers/intmatrix.h"
        "source/helpers/link/Link.cpp"
        "source/helpers/link/Link.h"
        "source/helpers/link/LinksDecorator.cpp"
        "source/helpers/link/LinksDecorator.h"
        "source/helpers/link/LinksString.cpp"
        "source/helpers/link/LinksString.h"
        "source/helpers/link/QtLinkTools.cpp"
        "source/helpers/link/QtLinkTools.h"
        "source/helpers/linktermdistancecache.cpp"
        "source/helpers/linktermdistancecache.h"
        "source/helpers/simplelistenserver.cpp"
        "source/helpers/simplelistenserver.h"
        "source/helpers/stemming/Stemmer.cpp"
        "source/helpers/stemming/Stemmer.h"
        "source/helpers/wiki/WikiUrlChecker.cpp"
        "source/helpers/wiki/WikiUrlChecker.h"
        "source/MainScene.cpp"
        "source/MainScene.h"
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
        "source/model/group/qttermgroup.cpp"
        "source/model/group/qttermgroup.h"
        "source/model/term/graphterm.cpp"
        "source/model/term/graphterm.h"
        "source/model/term/paintedterm.cpp"
        "source/model/term/paintedterm.h"
        "source/model/term/termdatacache.cpp"
        "source/model/term/termdatacache.h"
        "source/model/term/termdatawrapper.h"
        "source/model/term/termsmodel.cpp"
        "source/model/term/termsmodel.h"
        "source/staticDataStorage/StaticDataStorage.cpp"
        "source/staticDataStorage/StaticDataStorage.h"
        "source/TermDataInterface/DataStorageInterface.h"
        "source/TermDataInterface/groupsummary.cpp"
        "source/TermDataInterface/GroupSummary.h"
        "source/TermDataInterface/staticgroupdata.cpp"
        "source/TermDataInterface/StaticGroupData.h"
        "source/TermDataInterface/termdata.cpp"
        "source/TermDataInterface/TermData.h"
        "source/Text/CharTools.h"
        "source/Text/checkingtextcursor.cpp"
        "source/Text/CheckingTextCursor.h"
        "source/Text/textcursor.cpp"
        "source/Text/TextCursor.h"
        "source/Text/textrange.cpp"
        "source/Text/TextRange.h"
)

# List of sources containing modules
target_sources (TermGraph
    PUBLIC
    FILE_SET CXX_MODULES
    FILES
        "source/CommonTools/HandyTypesModule.cpp"
        "source/CommonTools/JsonTools.cpp"
        "source/CommonTools/NetworkTools.cpp"
        "source/CommonTools/Validator.cpp"
        "source/helpers/appstyle.cpp"
        "source/helpers/link/LinkTools.cpp"
        "source/main.cpp"
        "source/model/group/nodeverticalstack.cpp"
        "source/model/group/nodeverticalstacktools.cpp"
        "source/model/group/paintedforest.cpp"
        "source/model/group/termgroup.cpp"
        "source/model/group/termpositioner.cpp"
        "source/TermDataInterface/GroupValidator.cpp"
        "source/TermDataInterface/TermValidator.cpp"
        "source/Text/Fonts.cpp"
        "source/Text/TextTools.cpp"
)

# Add platform-specific sources
if (WASM)
    target_include_directories (TermGraph PRIVATE "${PROJECT_SOURCE_DIR}/source/CommonTools/wasm/")
    target_sources (TermGraph PRIVATE "source/CommonTools/wasm/PlatformTypes.h")
else ()
    target_include_directories (TermGraph PRIVATE "${PROJECT_SOURCE_DIR}/source/CommonTools/desktop/")
    target_sources (TermGraph PRIVATE "source/CommonTools/desktop/PlatformTypes.h")
endif ()

if (NOT WASM)
    target_sources (TermGraph PRIVATE "resources/HelpImages/HelpImages.qrc")
endif ()

target_include_directories (
    TermGraph PRIVATE
        "${PROJECT_SOURCE_DIR}"
        "${PROJECT_SOURCE_DIR}/source/"
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
            "source/TermDataConnection/DataStorageConnection.h"
            "source/TermDataConnection/NetWrapper.cpp"
            "source/TermDataConnection/NetWrapper.h"
    )

    find_package (Qt6 REQUIRED COMPONENTS Sql)
    target_link_libraries (TermGraph PRIVATE Qt6::Sql)

    target_sources (TermGraph
        PRIVATE
            "source/TermDataStorage/Database.cpp"
            "source/TermDataStorage/Database.h"
            "source/TermDataStorage/DbInfo.h"
            "source/TermDataStorage/DbTools.cpp"
            "source/TermDataStorage/DbTools.h"
            "source/TermDataStorage/LocalDataStorage.cpp"
            "source/TermDataStorage/LocalDataStorage.h"
            "source/TermDataStorage/queries/SqlQueries.qrc"
            "source/TermDataStorage/SqlQueryBuilder.cpp"
            "source/TermDataStorage/SqlQueryBuilder.h"
            "source/TermDataStorage/tables/AppConfigTable.cpp"
            "source/TermDataStorage/tables/AppConfigTable.h"
            "source/TermDataStorage/tables/TermGroupTable.cpp"
            "source/TermDataStorage/tables/TermGroupTable.h"
            "source/TermDataStorage/tables/TermTable.cpp"
            "source/TermDataStorage/tables/TermTable.h"
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
    QML_FILES "qml/Main.qml")

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
            "source/CommonTools/Errors.h"
            "source/CommonTools/GroupUuid.cpp"
            "source/CommonTools/GroupUuid.h"
            "source/CommonTools/HandyTypes.h"
            "source/CommonTools/Platform.h"
            "source/CommonTools/SafeUuid.h"
            "source/CommonTools/TermUuid.cpp"
            "source/CommonTools/TermUuid.h"
            "source/graph/Edge.hpp"
            "source/graph/Forest.hpp"
            "source/graph/Graph.hpp"
            "source/graph/GraphData.hpp"
            "source/graph/Node.hpp"
            "source/helpers/intmatrix.cpp"
            "source/helpers/intmatrix.h"
            "source/helpers/link/Link.cpp"
            "source/helpers/link/Link.h"
            "source/helpers/link/LinksDecorator.cpp"
            "source/helpers/link/LinksDecorator.h"
            "source/helpers/link/LinksString.cpp"
            "source/helpers/link/LinksString.h"
            "source/helpers/link/QtLinkTools.cpp"
            "source/helpers/link/QtLinkTools.h"
            "source/helpers/stemming/Stemmer.cpp"
            "source/helpers/stemming/Stemmer.h"
            "source/helpers/wiki/WikiUrlChecker.cpp"
            "source/helpers/wiki/WikiUrlChecker.h"
            "source/model/enums.h"
            "source/model/graphicitem/graphicitem.cpp"
            "source/model/graphicitem/graphicitem.h"
            "source/staticDataStorage/StaticDataStorage.cpp"
            "source/staticDataStorage/StaticDataStorage.h"
            "source/TermDataInterface/DataStorageInterface.h"
            "source/TermDataInterface/groupsummary.cpp"
            "source/TermDataInterface/GroupSummary.h"
            "source/TermDataInterface/staticgroupdata.cpp"
            "source/TermDataInterface/StaticGroupData.h"
            "source/TermDataInterface/termdata.cpp"
            "source/TermDataInterface/TermData.h"
            "source/TermDataStorage/Database.cpp"
            "source/TermDataStorage/Database.h"
            "source/TermDataStorage/DbInfo.h"
            "source/TermDataStorage/DbTools.cpp"
            "source/TermDataStorage/DbTools.h"
            "source/TermDataStorage/LocalDataStorage.cpp"
            "source/TermDataStorage/LocalDataStorage.h"
            "source/TermDataStorage/queries/SqlQueries.qrc"
            "source/TermDataStorage/SqlQueryBuilder.cpp"
            "source/TermDataStorage/SqlQueryBuilder.h"
            "source/TermDataStorage/tables/AppConfigTable.cpp"
            "source/TermDataStorage/tables/AppConfigTable.h"
            "source/TermDataStorage/tables/TermGroupTable.cpp"
            "source/TermDataStorage/tables/TermGroupTable.h"
            "source/TermDataStorage/tables/TermTable.cpp"
            "source/TermDataStorage/tables/TermTable.h"
            "source/Text/CharTools.h"
            "source/Text/checkingtextcursor.cpp"
            "source/Text/CheckingTextCursor.h"
            "source/Text/textcursor.cpp"
            "source/Text/TextCursor.h"
            "source/Text/textrange.cpp"
            "source/Text/TextRange.h"
    )

    # List of sources containing modules
    target_sources (TermGraphTest
        PUBLIC
        FILE_SET CXX_MODULES
        FILES
            "source/CommonTools/HandyTypesModule.cpp"
            "source/CommonTools/JsonTools.cpp"
            "source/CommonTools/NetworkTools.cpp"
            "source/CommonTools/Validator.cpp"
            "source/helpers/link/LinkTools.cpp"
            "source/TermDataInterface/GroupValidator.cpp"
            "source/TermDataInterface/TermValidator.cpp"
            "source/Text/Fonts.cpp"
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
        target_sources (TermGraphTest PRIVATE "source/CommonTools/wasm/PlatformTypes.h")
    else ()
        target_include_directories (TermGraphTest PRIVATE "${PROJECT_SOURCE_DIR}/source/CommonTools/desktop/")
        target_sources (TermGraphTest PRIVATE "source/CommonTools/desktop/PlatformTypes.h")
    endif ()

    target_include_directories (TermGraphTest PRIVATE ${PROJECT_SOURCE_DIR})
    target_include_directories (TermGraphTest SYSTEM PRIVATE
        "${PROJECT_SOURCE_DIR}/third_party/stemming/src")
    target_link_libraries (TermGraphTest GTest::gtest GTest::gtest_main)
    target_link_libraries (TermGraphTest Qt6::Core Qt6::Gui Qt6::Qml Qt6::Sql)

    set_target_properties (TermGraphTest PROPERTIES AUTORCC ON)

    gtest_add_tests (TARGET TermGraphTest)
endif ()
