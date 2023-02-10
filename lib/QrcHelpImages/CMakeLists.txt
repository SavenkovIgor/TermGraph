# Copyright © 2016-2023. Savenkov Igor
# SPDX-License-Identifier: GPL-3.0-or-later


project (HelpImagesResource LANGUAGES CXX)

find_package (Qt6 REQUIRED COMPONENTS Core)

add_library (HelpImagesResource STATIC)

set_target_properties (HelpImagesResource PROPERTIES AUTORCC ON)

target_sources (HelpImagesResource PRIVATE HelpImages.qrc)
