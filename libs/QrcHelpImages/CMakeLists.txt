# Copyright © 2016-2022. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required (VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

project (HelpImagesResource LANGUAGES CXX)

find_package (Qt6 COMPONENTS Core REQUIRED)

add_library (HelpImagesResource STATIC)

set_target_properties (HelpImagesResource PROPERTIES AUTORCC ON)

target_sources (HelpImagesResource PRIVATE HelpImages.qrc)
