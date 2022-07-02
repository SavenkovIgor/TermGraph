# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required (VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

project (IconsResource LANGUAGES CXX)

find_package (Qt6 COMPONENTS Core Svg REQUIRED)

add_library (IconsResource STATIC)

set_target_properties (IconsResource PROPERTIES AUTORCC ON)

target_sources (IconsResource PRIVATE Icons.qrc)

target_link_libraries (IconsResource INTERFACE Qt6::Core Qt6::Svg)
