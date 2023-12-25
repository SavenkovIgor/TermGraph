# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later

find_package (Qt6 REQUIRED COMPONENTS Core)

add_library (CommonToolsModule STATIC)

target_sources (CommonToolsModule
    PUBLIC
    FILE_SET CXX_MODULES
    FILES
        GroupUuid.cppm
        HandyTypes.cppm
        Errors.cppm
        JsonTools.cppm
        NetworkTools.cppm
        Platform.cppm
        SafeUuid.cppm
        TermUuid.cppm
        Validator.cppm
)

if (STATIC_STORAGE)
    target_compile_definitions (CommonToolsModule PUBLIC STATIC_STORAGE)
endif ()

target_link_libraries (CommonToolsModule PUBLIC Qt6::Core)

if (BUILD_TESTING)
    enable_testing ()

    find_package (Qt6 REQUIRED COMPONENTS Core)
    if (NOT GTest_FOUND)
        find_package (GTest REQUIRED)
    endif ()

    include (GoogleTest)

    add_executable (CommonToolsModuleTest)
    target_sources (CommonToolsModuleTest
        PRIVATE
            test/handy_types_test.cpp
            test/uuidstest.cpp
            test/validatortest.cpp)

    target_link_libraries (CommonToolsModuleTest GTest::gtest GTest::gtest_main)
    target_link_libraries (CommonToolsModuleTest CommonToolsModule Qt6::Core)

    gtest_add_tests (TARGET CommonToolsModuleTest)
endif ()
