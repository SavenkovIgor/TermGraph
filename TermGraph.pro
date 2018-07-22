QT       += core gui sql quickwidgets quickcontrols2 network #charts

TARGET = TermGraph
TEMPLATE = app

HEADERS += \
        source/databaseWorks/dbabstract.h\
	source/databaseWorks/edgtbl.h\
        source/databaseWorks/tblbase.h\
        source/databaseWorks/ndtbl.h\
        source/databaseWorks/termgrouptbl.h\
        source/databaseWorks/sqlqueryconstructor.h\
        source/databaseWorks/cloudservices.h\
        source/databaseWorks/dropboxconnection.h\
        source/Helpers/tagprocessor.h\
        source/edge.h\
        source/glb.h\
        source/mainscene.h\
        source/mainwindow.h\
        source/mysceneview.h\
        source/reminder.h\
        source/termgroup.h\
        source/terminfo.h\
        source/termnode.h\
        source/wordfreqanalyze.h \
        source/tgroupname.h \
    source/termgraph.h \
    source/Managers/networkmanager.h \
    source/Managers/groupsmanager.h \
    source/Managers/nodesmanager.h \
    source/Helpers/multipleshottimer.h \
    source/Helpers/listenserver.h

SOURCES += \
        source/main.cpp\
        source/databaseWorks/dbabstract.cpp\
        source/databaseWorks/edgtbl.cpp\
        source/databaseWorks/tblbase.cpp\
        source/databaseWorks/ndtbl.cpp\
        source/databaseWorks/termgrouptbl.cpp\
        source/databaseWorks/sqlqueryconstructor.cpp\
        source/databaseWorks/cloudservices.cpp\
        source/databaseWorks/dropboxconnection.cpp\
        source/Helpers/tagprocessor.cpp\
        source/reminder.cpp\
        source/edge.cpp\
        source/glb.cpp\
        source/terminfo.cpp\
        source/mainscene.cpp\
        source/mainwindow.cpp\
        source/mysceneview.cpp\
        source/termgroup.cpp\
        source/termnode.cpp\
        source/wordfreqanalyze.cpp \
    source/termgraph.cpp \
    source/Managers/networkmanager.cpp \
    source/Managers/groupsmanager.cpp \
    source/Managers/nodesmanager.cpp \
    source/Helpers/multipleshottimer.cpp \
    source/Helpers/listenserver.cpp

FORMS += \
        source/ui/reminder.ui\
        source/ui/wordfreqanalyze.ui
#        source/mainwindow.ui\


RESOURCES += \
    source/icons.qrc\
    source/qml.qrc

#INSTALLS += target

#DISTFILES += \
#    source/TestQmlForm.qml \
#    source/GoupsSideBar.qml \
#    source/MainScheme.qml \
#    source/MainWindow.qml \
#    source/NewNodeEdit.qml \
#    source/MyIconButton.qml
