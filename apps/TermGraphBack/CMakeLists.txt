# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (TermGraphBack
         VERSION 0.1.0
         DESCRIPTION "TermGraph REST Api server"
         HOMEPAGE_URL "https://termgraph.app/"
         LANGUAGES CXX)

if (BUILD_BACKEND)
    find_package (Qt6 REQUIRED COMPONENTS Core)
    find_package (restinio REQUIRED)

    add_executable (TermGraphBack)

    set_target_properties (TermGraphBack
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

    ## Source folder
    file (GLOB_RECURSE ALL_SOURCES source/*.h source/*.hpp source/*.cpp)
    target_sources (TermGraphBack PRIVATE ${ALL_SOURCES})

    target_link_libraries (TermGraphBack PRIVATE CommonTools TermDataInterface TermDataStorage)
    target_link_libraries (TermGraphBack PRIVATE restinio::restinio)
    target_link_libraries (TermGraphBack PRIVATE Qt6::Core)
endif ()
