#  TermGraph, build graph of knowledge.
#  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

project (TermGraph VERSION 1.0.0 LANGUAGES CXX)

set (CMAKE_INCLUDE_CURRENT_DIR ON)

set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

# If you want, android build correctly,
# This line must be before targets definition
find_package (Qt5 COMPONENTS Core Quick QuickControls2 Sql Network Svg REQUIRED)

if (ANDROID)
    add_library (${PROJECT_NAME} SHARED)
else ()
    add_executable (${PROJECT_NAME})
endif ()

# Resources folder
target_sources (${PROJECT_NAME}
    PRIVATE
        resources/helpImages/helpImages.qrc
        resources/icons/icons.qrc
)

# Qml folder
target_sources (${PROJECT_NAME} PRIVATE qml/qml.qrc)

# Some non compilable files
target_sources (${PROJECT_NAME}
    PRIVATE
        android/AndroidManifest.xml
        android/res/drawable-hdpi/icon.png
        android/res/drawable-mdpi/icon.png
        android/res/drawable-ldpi/icon.png
        .devTools/goodcheck.yml
)

# Source folder
target_sources (${PROJECT_NAME}
    PRIVATE
        source/Helpers/appconfig.cpp
        source/Helpers/appconfig.h
        source/Helpers/appstyle.h
        source/Helpers/fonts.cpp
        source/Helpers/fonts.h
        source/Helpers/fsworks.cpp
        source/Helpers/fsworks.h
        source/Helpers/globaltagcache.cpp
        source/Helpers/globaltagcache.h
        source/Helpers/handytypes.h
        source/Helpers/helpstuff.cpp
        source/Helpers/helpstuff.h
        source/Helpers/multipleshottimer.cpp
        source/Helpers/multipleshottimer.h
        source/Helpers/platform.h
        source/Helpers/simplelistenserver.cpp
        source/Helpers/simplelistenserver.h
        source/Helpers/tagprocessor.cpp
        source/Helpers/tagprocessor.h
        source/Helpers/textprocessor.cpp
        source/Helpers/textprocessor.h
        source/Helpers/intmatrix.cpp
        source/Helpers/intmatrix.h

        source/Managers/groupsmanager.cpp
        source/Managers/groupsmanager.h
        source/Managers/jsongroupinfocontainerparser.cpp
        source/Managers/jsongroupinfocontainerparser.h
        source/Managers/jsonnodeinfocontainerparser.cpp
        source/Managers/jsonnodeinfocontainerparser.h
        source/Managers/networkmanager.cpp
        source/Managers/networkmanager.h
        source/Managers/nodesmanager.cpp
        source/Managers/nodesmanager.h
        source/Managers/notificationmanager.cpp
        source/Managers/notificationmanager.h
        source/Managers/syncmanager.cpp
        source/Managers/syncmanager.h

        source/Model/GraphicItem/graphicitem.cpp
        source/Model/GraphicItem/graphicitem.h
        source/Model/GraphicItem/rectgraphicitem.cpp
        source/Model/GraphicItem/rectgraphicitem.h
        source/Model/Termin/graphterm.cpp
        source/Model/Termin/graphterm.h
        source/Model/Termin/infoterm.cpp
        source/Model/Termin/infoterm.h
        source/Model/Termin/nodegadgetwrapper.cpp
        source/Model/Termin/nodegadgetwrapper.h
        source/Model/Termin/nodeinfocontainer.h
        source/Model/Termin/paintedterm.cpp
        source/Model/Termin/paintedterm.h
        source/Model/TerminEdge/edge.cpp
        source/Model/TerminEdge/edge.h
        source/Model/TerminEdge/graphedge.cpp
        source/Model/TerminEdge/graphedge.h
        source/Model/TerminGroup/groupgadgetwrapper.cpp
        source/Model/TerminGroup/groupgadgetwrapper.h
        source/Model/TerminGroup/groupinfocontainer.h
        source/Model/TerminGroup/groupnamecache.cpp
        source/Model/TerminGroup/groupnamecache.h
        source/Model/TerminGroup/nodeverticalstack.cpp
        source/Model/TerminGroup/nodeverticalstack.h
        source/Model/TerminGroup/termgroup.cpp
        source/Model/TerminGroup/termgroup.h
        source/Model/TerminGroup/termgroupinfo.cpp
        source/Model/TerminGroup/termgroupinfo.h
        source/Model/TerminGroup/termtree.cpp
        source/Model/TerminGroup/termtree.h
        source/Model/enums.h

        source/databaseWorks/columns/nodecolumn.h
        source/databaseWorks/columns/tcolumn.h
        source/databaseWorks/columns/termgroupcolumn.h
        source/databaseWorks/tools/dbtools.cpp
        source/databaseWorks/tools/dbtools.h
        source/databaseWorks/tools/insertcontainer.cpp
        source/databaseWorks/tools/insertcontainer.h
        source/databaseWorks/tools/querytools.h
        source/databaseWorks/tools/setexpression.cpp
        source/databaseWorks/tools/setexpression.h
        source/databaseWorks/tools/wherecondition.cpp
        source/databaseWorks/tools/wherecondition.h
        source/databaseWorks/appconfigtable.cpp
        source/databaseWorks/appconfigtable.h
        source/databaseWorks/database.cpp
        source/databaseWorks/database.h
        source/databaseWorks/dbtablenames.h
        source/databaseWorks/ndtbl.cpp
        source/databaseWorks/ndtbl.h
        source/databaseWorks/sqlqueryconstructor.cpp
        source/databaseWorks/sqlqueryconstructor.h
        source/databaseWorks/tblbase.cpp
        source/databaseWorks/tblbase.h
        source/databaseWorks/termgrouptbl.cpp
        source/databaseWorks/termgrouptbl.h

        source/main.cpp
        source/mainscene.cpp
        source/mainscene.h
        source/mainwindow.cpp
        source/mainwindow.h
)

target_compile_definitions (${PROJECT_NAME}
    PRIVATE
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries (${PROJECT_NAME}
    PRIVATE
        Qt5::Core
        Qt5::Quick
        Qt5::QuickControls2
        Qt5::Sql
        Qt5::Network
        Qt5::Svg
)

add_subdirectory (tests)
