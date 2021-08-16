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

enable_testing ()
find_package (GTest REQUIRED)

function (test_case_config TEST_NAME)
    target_include_directories (${TEST_NAME}
        PRIVATE
            ${PROJECT_SOURCE_DIR}
            ${CMAKE_SOURCE_DIR}
            ${GTest_INCLUDE_DIRS}
            ${TermGraph_SOURCE_DIR})
    target_link_libraries (${TEST_NAME} Qt6::Core GTest::gtest GTest::gtest_main)
    set_target_properties (${TEST_NAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)
endfunction ()


# TODO: Maybe rework with add_test

# TEXT_UTILS_TEST
add_executable (TEXT_UTILS_TEST
    ../source/helpers/text/textutils.cpp
    ../source/helpers/text/textutils.h
    textutilstest.cpp)

test_case_config (TEXT_UTILS_TEST)


# FS_WORKS_TEST
add_executable (FS_WORKS_TEST
    ../source/helpers/fsworks.cpp
    ../source/helpers/fsworks.h
    fsworkstest.cpp)

test_case_config (FS_WORKS_TEST)


# UUIDS_TEST
add_executable (UUIDS_TEST
    ../source/helpers/uuid/uuidtools.cpp
    ../source/helpers/uuid/uuidtools.h
    uuidstest.cpp)

test_case_config (UUIDS_TEST)
target_link_libraries (UUIDS_TEST CommonTools)

# NODE_TEST
add_executable (NODE_TEST
    ../source/model/base/node.hpp
    nodetest.cpp)

test_case_config (NODE_TEST)

# EDGE_TEST
add_executable (EDGE_TEST
    ../source/model/base/node.hpp
    ../source/model/base/edge.hpp
    edgetest.cpp)

test_case_config (EDGE_TEST)

# GRAPH_TEST
add_executable (GRAPH_TEST
    ../source/model/base/node.hpp
    ../source/model/base/edge.hpp
    ../source/model/base/graph.hpp
    ../source/model/base/graphdata.hpp
    graphtest.cpp)

test_case_config (GRAPH_TEST)

add_executable (FOREST_TEST
    ../source/model/base/node.hpp
    ../source/model/base/edge.hpp
    ../source/model/base/graph.hpp
    ../source/model/base/graphdata.hpp
    ../source/model/base/forest.hpp
    ../source/model/enums.h
    foresttest.cpp)

test_case_config (FOREST_TEST)

# GRAPHIC_ITEM_TEST
add_executable (GRAPHIC_ITEM_TEST
    ../source/model/graphicitem/graphicitem.h
    ../source/model/graphicitem/graphicitem.cpp
    graphicitemtest.cpp)

test_case_config (GRAPHIC_ITEM_TEST)

# TEXT_CURSOR_TEST
add_executable (TEXT_CURSOR_TEST
    ../source/helpers/text/chartools.h
    ../source/helpers/text/checkingtextcursor.cpp
    ../source/helpers/text/checkingtextcursor.h
    ../source/helpers/text/textcursor.cpp
    ../source/helpers/text/textcursor.h
    cursortest.cpp)

test_case_config (TEXT_CURSOR_TEST)
target_link_libraries (TEXT_CURSOR_TEST CommonTools)

# TEXT_RANGE_TEST
add_executable (TEXT_RANGE_TEST
    ../source/helpers/intmatrix.cpp
    ../source/helpers/intmatrix.h
    ../source/helpers/link/link.cpp
    ../source/helpers/link/link.h
    ../source/helpers/text/chartools.h
    ../source/helpers/text/checkingtextcursor.cpp
    ../source/helpers/text/checkingtextcursor.h
    ../source/helpers/text/textcursor.cpp
    ../source/helpers/text/textcursor.h
    ../source/helpers/text/textrange.cpp
    ../source/helpers/text/textrange.h
    ../source/helpers/uuid/uuidtools.cpp
    ../source/helpers/uuid/uuidtools.h
    textrangetest.cpp)

test_case_config (TEXT_RANGE_TEST)
target_link_libraries (TEXT_RANGE_TEST CommonTools)


# TEXT_LINK_TEST
add_executable (TEXT_LINK_TEST
    ../source/helpers/intmatrix.cpp
    ../source/helpers/intmatrix.h
    ../source/helpers/link/linksstring.cpp
    ../source/helpers/link/linksstring.h
    ../source/helpers/link/link.cpp
    ../source/helpers/link/link.h
    ../source/helpers/text/chartools.h
    ../source/helpers/text/checkingtextcursor.cpp
    ../source/helpers/text/checkingtextcursor.h
    ../source/helpers/text/textcursor.cpp
    ../source/helpers/text/textcursor.h
    ../source/helpers/text/textrange.cpp
    ../source/helpers/text/textrange.h
    ../source/helpers/uuid/uuidtools.cpp
    ../source/helpers/uuid/uuidtools.h
    linktest.cpp)

test_case_config (TEXT_LINK_TEST)
target_link_libraries (TEXT_LINK_TEST CommonTools)

# LINK_UTILS_TEST
add_executable (LINK_UTILS_TEST
    ../source/helpers/intmatrix.cpp
    ../source/helpers/intmatrix.h
    ../source/helpers/link/linkutils.cpp
    ../source/helpers/link/linkutils.h
    ../source/helpers/link/link.cpp
    ../source/helpers/link/link.h
    ../source/helpers/text/chartools.h
    ../source/helpers/text/checkingtextcursor.cpp
    ../source/helpers/text/checkingtextcursor.h
    ../source/helpers/text/textcursor.cpp
    ../source/helpers/text/textcursor.h
    ../source/helpers/text/textrange.cpp
    ../source/helpers/text/textrange.h
    ../source/helpers/text/textutils.cpp
    ../source/helpers/text/textutils.h
    ../source/helpers/uuid/uuidtools.cpp
    ../source/helpers/uuid/uuidtools.h
    linkutilstest.cpp)

test_case_config (LINK_UTILS_TEST)
target_link_libraries (LINK_UTILS_TEST CommonTools)

# LINK_DECORATOR_TEST
add_executable (LINK_DECORATOR_TEST
    ../source/helpers/intmatrix.cpp
    ../source/helpers/intmatrix.h
    ../source/helpers/link/linksdecorator.cpp
    ../source/helpers/link/linksdecorator.h
    ../source/helpers/link/linksstring.cpp
    ../source/helpers/link/linksstring.h
    ../source/helpers/link/link.cpp
    ../source/helpers/link/link.h
    ../source/helpers/text/chartools.h
    ../source/helpers/text/checkingtextcursor.cpp
    ../source/helpers/text/checkingtextcursor.h
    ../source/helpers/text/textcursor.cpp
    ../source/helpers/text/textcursor.h
    ../source/helpers/text/textrange.cpp
    ../source/helpers/text/textrange.h
    ../source/helpers/uuid/uuidtools.cpp
    ../source/helpers/uuid/uuidtools.h
    linkdecoratortest.cpp)

test_case_config (LINK_DECORATOR_TEST)
target_link_libraries (LINK_DECORATOR_TEST Qt6::Gui CommonTools)
