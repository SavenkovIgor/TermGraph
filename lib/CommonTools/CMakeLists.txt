# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


find_package (Qt6 REQUIRED COMPONENTS Core)

add_library (CommonTools INTERFACE)

target_sources (CommonTools
    INTERFACE
        include/CommonTools/GroupUuid.h
        include/CommonTools/HandyTypes.h
        include/CommonTools/Errors.h
        include/CommonTools/JsonTools.h
        include/CommonTools/NetworkTools.h
        include/CommonTools/Platform.h
        include/CommonTools/SafeUuid.h
        include/CommonTools/TermUuid.h
        include/CommonTools/Validator.h)

if (STATIC_STORAGE)
    target_compile_definitions (CommonTools INTERFACE STATIC_STORAGE)
endif ()

target_include_directories (CommonTools INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries (CommonTools INTERFACE Qt6::Core)

if (BUILD_TESTING)
    enable_testing ()

    find_package (Qt6 REQUIRED COMPONENTS Core)
    if (NOT GTest_FOUND)
        find_package (GTest REQUIRED)
    endif ()

    include (GoogleTest)

    add_executable (CommonToolsTest)
    target_sources (CommonToolsTest
        PRIVATE
            test/uuidstest.cpp
            test/validatortest.cpp)

    target_link_libraries (CommonToolsTest GTest::gtest GTest::gtest_main)
    target_link_libraries (CommonToolsTest CommonTools Qt6::Core)

    gtest_add_tests (TARGET CommonToolsTest)
endif ()
