# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (CommonTools VERSION 1.0.0 LANGUAGES CXX)

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

target_include_directories (CommonTools INTERFACE "./include")

target_link_libraries (CommonTools INTERFACE Qt6::Core)
target_link_libraries (CommonTools INTERFACE outcome::outcome)

if (BUILD_TESTING)
    list (APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
    include (TestTools)

    enable_testing ()
    find_package (Qt6 REQUIRED COMPONENTS Core)

    create_test (
        NAME      TestUuids
        SOURCES
            include/CommonTools/SafeUuid.h
            include/CommonTools/GroupUuid.h
            include/CommonTools/TermUuid.h
            tests/uuidstest.cpp
        LIBRARIES CommonTools TermDataStorage Qt6::Core)

    create_test (
        NAME      TestValidator
        SOURCES
            include/CommonTools/Validator.h
            tests/validatortest.cpp
        LIBRARIES CommonTools TermDataStorage Qt6::Core)

endif ()
