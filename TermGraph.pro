QT       += core gui sql charts quickwidgets quickcontrols2

TARGET = TermGraph
TEMPLATE = app

HEADERS += \
        source/databaseWorks/dbabstract.h\
	source/databaseWorks/edgtbl.h\
        source/databaseWorks/tblbase.h\
        source/databaseWorks/ndtbl.h\
        source/databaseWorks/termgrouptbl.h\
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
    source/tgroupname.h

SOURCES += \
        source/main.cpp\
        source/databaseWorks/dbabstract.cpp\
        source/databaseWorks/edgtbl.cpp\        
        source/databaseWorks/tblbase.cpp\
        source/databaseWorks/termgrouptbl.cpp\
        source/databaseWorks/ndtbl.cpp\
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
    source/tgroupname.cpp

FORMS += \
        source/mainwindow.ui\
        source/reminder.ui\
        source/wordfreqanalyze.ui


RESOURCES += \
    source/icons.qrc

#INSTALLS += target

#DISTFILES += \
#    source/TestQmlForm.qml \
#    source/GoupsSideBar.qml \
#    source/MainScheme.qml \
#    source/MainWindow.qml \
#    source/NewNodeEdit.qml \
#    source/MyIconButton.qml
