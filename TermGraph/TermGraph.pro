QT       += core gui sql quickwidgets quickcontrols2 network #charts

TARGET = TermGraph
TEMPLATE = app

CONFIG += c++17

HEADERS += \
    source/Managers/syncmanager.h \
    source/Model/Termin/nodeinfocontainer.h \
    source/databaseWorks/database.h\
    source/databaseWorks/edgtbl.h\
    source/databaseWorks/tblbase.h\
    source/databaseWorks/ndtbl.h\
    source/databaseWorks/termgrouptbl.h\
    source/databaseWorks/sqlqueryconstructor.h\
    source/databaseWorks/cloudservices.h\
    source/databaseWorks/dropboxconnection.h\
    source/mainscene.h\
    source/mainwindow.h\
    source/Model/TerminEdge/edge.h\
    source/Model/TerminGroup/termgroup.h\
    source/Model/wordfreqanalyze.h \
    source/Model/reminder.h\
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
    source/paintqueuemanager.h \
    source/Model/GraphicItem/labelgraphicitem.h \
    source/Model/enums.h \
    source/databaseWorks/dbtablenames.h \
    source/databaseWorks/appconfigtable.h \
    source/Helpers/textprocessor.h

SOURCES += \
    source/Managers/syncmanager.cpp \
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
    source/Model/reminder.cpp\
    source/Model/TerminEdge/edge.cpp\
    source/Model/TerminGroup/termgroup.cpp\
    source/Model/wordfreqanalyze.cpp \
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
    source/paintqueuemanager.cpp \
    source/Model/GraphicItem/labelgraphicitem.cpp \
    source/databaseWorks/appconfigtable.cpp \
    source/Helpers/textprocessor.cpp

FORMS += \
    source/ui/reminder.ui\
    source/ui/wordfreqanalyze.ui

RESOURCES += \
    source/icons.qrc\
    source/qml.qrc

#INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
