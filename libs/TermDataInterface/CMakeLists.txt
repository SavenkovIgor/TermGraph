# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (TermDataInterface VERSION 1.0.0 LANGUAGES CXX)

find_package (Qt6 REQUIRED COMPONENTS Core)

add_library (TermDataInterface
    INTERFACE
        include/TermDataInterface/DataStorageInterface.h
        include/TermDataInterface/GroupData.h
        include/TermDataInterface/GroupValidator.h
        include/TermDataInterface/TermData.h
        include/TermDataInterface/TermValidator.h)

target_include_directories (TermDataInterface INTERFACE "./include")

target_link_libraries (TermDataInterface INTERFACE CommonTools)
target_link_libraries (TermDataInterface INTERFACE Qt6::Core)

if (BUILD_TESTING)
    include (TestTools)

    create_test (
        NAME    TestTermData
        SOURCES
            include/TermDataInterface/TermData.h
            include/TermDataInterface/TermValidator.h
            tests/termdatatest.cpp
        LIBRARIES TermDataInterface CommonTools Qt6::Core)

    create_test (
        NAME    TestGroupData
        SOURCES
            include/TermDataInterface/GroupData.h
            include/TermDataInterface/GroupValidator.h
            tests/groupdatatest.cpp
        LIBRARIES TermDataInterface CommonTools Qt6::Core)

endif ()
