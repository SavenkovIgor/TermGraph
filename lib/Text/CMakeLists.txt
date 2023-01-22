# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (Text LANGUAGES CXX)

find_package (Qt6 REQUIRED COMPONENTS Core Gui)

add_library (Text STATIC)

target_link_libraries (Text PRIVATE Qt6::Core Qt6::Gui)
target_link_libraries (Text PRIVATE CommonTools)

set_target_properties (Text
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

target_include_directories (Text INTERFACE "./include")
target_include_directories (Text PRIVATE ${PROJECT_SOURCE_DIR})

# Sql queries
target_sources (Text
    INTERFACE
        include/Text/CharTools.h
        include/Text/CheckingTextCursor.h
        include/Text/TextCursor.h
        include/Text/TextRange.h
        include/Text/TextTools.h
        include/Text/Fonts.h
    PRIVATE
        source/checkingtextcursor.cpp
        source/textcursor.cpp
        source/textrange.cpp
        source/texttools.cpp
        source/fonts.cpp)

if (BUILD_TESTING)
    enable_testing ()

    find_package (Qt6 REQUIRED COMPONENTS Core)
    if (NOT GTest_FOUND)
        find_package (GTest REQUIRED)
    endif ()

    include (GoogleTest)

    add_executable (TextTest)
    target_sources (TextTest
                        PRIVATE
                            test/cursortest.cpp
                            test/textrangetest.cpp
                            test/texttoolstest.cpp)

    set_target_properties (TextTest
        PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

    target_link_libraries (TextTest GTest::gtest GTest::gtest_main)
    target_link_libraries (TextTest Qt6::Core Qt6::Gui)
    target_link_libraries (TextTest CommonTools Text)

    gtest_add_tests (TARGET TextTest)
endif ()