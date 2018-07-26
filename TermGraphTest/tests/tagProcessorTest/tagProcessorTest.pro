QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

MAIN_PROJECT_PATH = ../../../TermGraph/source

HEADERS += $${MAIN_PROJECT_PATH}/Helpers/tagprocessor.h

SOURCES +=  ./tst_tagprocessortest.cpp \
    $${MAIN_PROJECT_PATH}/Helpers/tagprocessor.cpp
