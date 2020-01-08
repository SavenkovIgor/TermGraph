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

QT       += core gui sql quickwidgets quickcontrols2 network svg #charts

TARGET = TermGraph
TEMPLATE = app

CONFIG += c++17

HEADERS += \
    source/Helpers/globaltagcache.h \
    source/Helpers/platform.h \
    source/Managers/jsongroupinfocontainerparser.h \
    source/Managers/jsonnodeinfocontainerparser.h \
    source/Managers/notificationmanager.h \
    source/Managers/syncmanager.h \
    source/Model/Termin/nodegadgetwrapper.h \
    source/Model/Termin/nodeinfocontainer.h \
    source/Model/TerminGroup/groupgadgetwrapper.h \
    source/Model/TerminGroup/groupinfocontainer.h \
    source/Model/TerminGroup/groupnamecache.h \
    source/databaseWorks/columns/nodecolumn.h \
    source/databaseWorks/columns/tcolumn.h \
    source/databaseWorks/columns/termgroupcolumn.h \
    source/databaseWorks/database.h\
    source/databaseWorks/tools/dbtools.h \
    source/databaseWorks/edgtbl.h\
    source/databaseWorks/tblbase.h\
    source/databaseWorks/ndtbl.h\
    source/databaseWorks/termgrouptbl.h\
    source/databaseWorks/sqlqueryconstructor.h\
    source/databaseWorks/cloudservices.h\
    source/databaseWorks/tools/querytools.h\
    source/databaseWorks/dropboxconnection.h\
    source/databaseWorks/tools/insertcontainer.h \
    source/databaseWorks/tools/setexpression.h \
    source/databaseWorks/tools/wherecondition.h \
    source/mainscene.h\
    source/mainwindow.h\
    source/Model/TerminEdge/edge.h\
    source/Model/TerminGroup/termgroup.h\
    source/Managers/networkmanager.h \
    source/Managers/groupsmanager.h \
    source/Managers/nodesmanager.h \
    source/Helpers/tagprocessor.h\
    source/Helpers/multipleshottimer.h \
    source/Helpers/simplelistenserver.h \
    source/Helpers/fonts.h \
    source/Helpers/appstyle.h \
    source/Helpers/helpstuff.h \
    source/Helpers/appconfig.h \
    source/Helpers/fsworks.h \
    source/Helpers/handytypes.h \
    source/Model/TerminEdge/graphedge.h \
    source/Model/Termin/graphterm.h \
    source/Model/Termin/infoterm.h \
    source/Model/Termin/paintedterm.h \
    source/Model/TerminGroup/termgroupinfo.h \
    source/Model/TerminGroup/nodeverticalstack.h \
    source/Model/TerminGroup/termtree.h \
    source/Model/GraphicItem/graphicitem.h \
    source/Model/GraphicItem/rectgraphicitem.h \
    source/Managers/paintqueuemanager.h \
    source/Model/GraphicItem/labelgraphicitem.h \
    source/Model/enums.h \
    source/databaseWorks/dbtablenames.h \
    source/databaseWorks/appconfigtable.h \
    source/Helpers/textprocessor.h

SOURCES += \
    source/Helpers/globaltagcache.cpp \
    source/Managers/jsongroupinfocontainerparser.cpp \
    source/Managers/jsonnodeinfocontainerparser.cpp \
    source/Managers/notificationmanager.cpp \
    source/Managers/syncmanager.cpp \
    source/Model/Termin/nodegadgetwrapper.cpp \
    source/Model/TerminGroup/groupgadgetwrapper.cpp \
    source/Model/TerminGroup/groupnamecache.cpp \
    source/databaseWorks/tools/dbtools.cpp \
    source/databaseWorks/tools/insertcontainer.cpp \
    source/databaseWorks/tools/setexpression.cpp \
    source/databaseWorks/tools/wherecondition.cpp \
    source/main.cpp\
    source/databaseWorks/database.cpp\
    source/databaseWorks/edgtbl.cpp\
    source/databaseWorks/tblbase.cpp\
    source/databaseWorks/ndtbl.cpp\
    source/databaseWorks/termgrouptbl.cpp\
    source/databaseWorks/sqlqueryconstructor.cpp\
    source/databaseWorks/cloudservices.cpp\
    source/databaseWorks/dropboxconnection.cpp\
    source/mainscene.cpp\
    source/mainwindow.cpp\
    source/Model/TerminEdge/edge.cpp\
    source/Model/TerminGroup/termgroup.cpp\
    source/Managers/networkmanager.cpp \
    source/Managers/groupsmanager.cpp \
    source/Managers/nodesmanager.cpp \
    source/Helpers/tagprocessor.cpp\
    source/Helpers/multipleshottimer.cpp \
    source/Helpers/simplelistenserver.cpp \
    source/Helpers/fonts.cpp \
    source/Helpers/helpstuff.cpp \
    source/Helpers/appconfig.cpp \
    source/Helpers/fsworks.cpp \
    source/Model/TerminEdge/graphedge.cpp \
    source/Model/Termin/graphterm.cpp \
    source/Model/Termin/infoterm.cpp \
    source/Model/Termin/paintedterm.cpp \
    source/Model/TerminGroup/termgroupinfo.cpp \
    source/Model/TerminGroup/nodeverticalstack.cpp \
    source/Model/TerminGroup/termtree.cpp \
    source/Model/GraphicItem/graphicitem.cpp \
    source/Model/GraphicItem/rectgraphicitem.cpp \
    source/Managers/paintqueuemanager.cpp \
    source/Model/GraphicItem/labelgraphicitem.cpp \
    source/databaseWorks/appconfigtable.cpp \
    source/Helpers/textprocessor.cpp

RESOURCES += \
    icons/icons.qrc \
    qml/qml.qrc

#INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
