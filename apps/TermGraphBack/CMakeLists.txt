# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


cmake_minimum_required (VERSION 3.22.0)

project (TermGraphBack
         VERSION 0.1.0
         DESCRIPTION "TermGraph REST Api server"
         HOMEPAGE_URL "https://termgraph.app/"
         LANGUAGES CXX)

if (BUILD_BACKEND)
    find_package (Qt6 REQUIRED COMPONENTS Core)
    find_package (restinio REQUIRED)

    add_executable (TermGraphBack "source/main.cpp")

    set_target_properties (TermGraphBack
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)

    target_link_libraries (TermGraphBack PRIVATE Qt6::Core)
    target_link_libraries (TermGraphBack PRIVATE restinio::restinio)
    target_link_libraries (TermGraphBack PRIVATE CommonTools TermDataInterface TermDataStorage)
endif ()
