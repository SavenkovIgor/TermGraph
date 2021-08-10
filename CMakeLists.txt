#  TermGraph, build graph of knowledge.
#  Copyright © 2016-2020. Savenkov Igor. All rights reserved
#  Contacts: dev.savenkovigor@protonmail.com
#
#  This file is part of TermGraph.
#
#  TermGraph is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  TermGraph is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.

# TODO:
# 1) Split to apps: TermGraphApplication, TermDataProvider, TermServer, TermFront
# 2) Move tests to appropriate libraries

cmake_minimum_required (VERSION 3.16.0)

project (TermGraphPackage VERSION 1.0.0)

option (BUILD_TESTING "Build tests with project" ON)

if (ANDROID)
    set (BUILD_TESTING OFF)
endif ()

add_subdirectory (apps/TermGraphApp)

add_subdirectory (libs/commontools)
add_subdirectory (libs/datainterface)
add_subdirectory (libs/termstorage)
