# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (CommonTools LANGUAGES CXX)

find_package (Qt6 REQUIRED COMPONENTS Core)

if (ANDROID OR WASM)
    set (outcome_DIR "${CMAKE_BINARY_DIR}/conan-dependencies")
endif ()

find_package (outcome REQUIRED)

add_library (CommonTools
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

# Disable of std expected for outcome library
target_compile_definitions (CommonTools INTERFACE nsel_CONFIG_SELECT_EXPECTED nsel_EXPECTED_NONSTD)

if (STATIC_STORAGE)
    target_compile_definitions (CommonTools INTERFACE STATIC_STORAGE)
endif ()

target_include_directories (CommonTools INTERFACE "./include")

target_link_libraries (CommonTools INTERFACE Qt6::Core)
target_link_libraries (CommonTools INTERFACE outcome::outcome)

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

    set_target_properties (CommonToolsTest
        PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

    target_link_libraries (CommonToolsTest GTest::gtest GTest::gtest_main)
    target_link_libraries (CommonToolsTest CommonTools Qt6::Core)

    gtest_add_tests (TARGET CommonToolsTest)
endif ()
