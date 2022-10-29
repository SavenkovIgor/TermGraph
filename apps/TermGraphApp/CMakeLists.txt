# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (TermGraph
         VERSION 2.0.0
         DESCRIPTION "Terms diagram builder. Polish your knowledge with accurate definitions."
         HOMEPAGE_URL "https://termgraph.app/"
         LANGUAGES CXX)

find_package (Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Network Svg)

qt_standard_project_setup()

qt_add_executable (TermGraph
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
    "source/helpers/asyncgroupbuilder.cpp"
    "source/helpers/asyncgroupbuilder.h"
    "source/helpers/exclusivecall.h"
    "source/helpers/firststartpreparer.cpp"
    "source/helpers/firststartpreparer.h"
    "source/helpers/fonts.cpp"
    "source/helpers/fonts.h"
    "source/helpers/globaltagcache.cpp"
    "source/helpers/globaltagcache.h"
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
    "source/helpers/text/chartools.h"
    "source/helpers/text/checkingtextcursor.cpp"
    "source/helpers/text/checkingtextcursor.h"
    "source/helpers/text/textcursor.cpp"
    "source/helpers/text/textcursor.h"
    "source/helpers/text/textrange.cpp"
    "source/helpers/text/textrange.h"
    "source/helpers/text/texttools.cpp"
    "source/helpers/text/texttools.h"
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
    "source/model/base/edge.hpp"
    "source/model/base/forest.hpp"
    "source/model/base/graph.hpp"
    "source/model/base/graphdata.hpp"
    "source/model/base/node.hpp"
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
    "source/model/group/groupdatatwrapper.h"
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

set_target_properties (TermGraph
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

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
target_link_libraries (TermGraph PRIVATE TermDataInterface)
target_link_libraries (TermGraph PRIVATE TermDataConnection)

if (NOT WASM)
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
    VERSION 1.0
    IMPORTS
        QtQml QtQuick QtQuick.Controls QtQuick.Controls.Material QtQuick.Shapes QtQuick.Layouts QtQuick.Window QtQuick.Templates
        Theme Tools Atoms Molecules Pages
    QML_FILES "qml/main.qml"
    RESOURCE_PREFIX "/qt/qml")


if (BUILD_TESTING)
    include (TestTools)
    create_test (
        NAME      TestTextTools
        SOURCES
            source/helpers/fonts.cpp
            source/helpers/fonts.h
            source/helpers/text/texttools.cpp
            source/helpers/text/texttools.h
            tests/texttoolstest.cpp
        LIBRARIES Qt6::Core Qt6::Gui)

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
        LIBRARIES Qt6::Core Qt6::Qml)

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
            source/helpers/link/link.cpp
            source/helpers/link/link.h
            source/helpers/link/linksdecorator.cpp
            source/helpers/link/linksdecorator.h
            source/helpers/link/linksstring.cpp
            source/helpers/link/linksstring.h
            source/helpers/text/chartools.h
            source/helpers/text/checkingtextcursor.cpp
            source/helpers/text/checkingtextcursor.h
            source/helpers/text/textcursor.cpp
            source/helpers/text/textcursor.h
            source/helpers/text/textrange.cpp
            source/helpers/text/textrange.h
            tests/linktest.cpp
        LIBRARIES Qt6::Core Qt6::Gui CommonTools)

    create_test (
        NAME      TestLinkTools
        SOURCES
            source/helpers/fonts.cpp
            source/helpers/fonts.h
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
            source/helpers/text/chartools.h
            source/helpers/text/checkingtextcursor.cpp
            source/helpers/text/checkingtextcursor.h
            source/helpers/text/textcursor.cpp
            source/helpers/text/textcursor.h
            source/helpers/text/textrange.cpp
            source/helpers/text/textrange.h
            source/helpers/text/texttools.cpp
            source/helpers/text/texttools.h
            tests/linktoolstest.cpp
        LIBRARIES Qt6::Core Qt6::Gui Qt6::Qml CommonTools)

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
