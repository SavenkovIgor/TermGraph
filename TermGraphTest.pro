QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += ./source/Helpers/tagprocessor.h

SOURCES +=  ./tests/tst_tagprocessortest.cpp \
    ./source/Helpers/tagprocessor.cpp
