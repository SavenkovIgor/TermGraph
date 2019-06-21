QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase c++17
CONFIG -= app_bundle

TEMPLATE = app

MAIN_PROJECT_PATH = ../../../TermGraph/source

HEADERS += $${MAIN_PROJECT_PATH}/Helpers/textprocessor.h

SOURCES +=  ./tst_textprocessortest.cpp \
    $${MAIN_PROJECT_PATH}/Helpers/textprocessor.cpp
