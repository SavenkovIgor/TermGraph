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
            ${TermGraph_SOURCE_DIR})
    target_link_libraries (${TEST_NAME} Qt6::Core GTest::gtest GTest::gtest_main)
    set_target_properties (${TEST_NAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)
endfunction ()


# TODO: Maybe rework with add_test

# TestTextUtils
add_executable (TestTextUtils
    ../source/helpers/text/textutils.cpp
    ../source/helpers/text/textutils.h
    textutilstest.cpp)

test_case_config (TestTextUtils)


# TestFsWorks
add_executable (TestFsWorks
    ../source/helpers/fsworks.cpp
    ../source/helpers/fsworks.h
    fsworkstest.cpp)

test_case_config (TestFsWorks)

# TestNode
add_executable (TestNode
    ../source/model/base/node.hpp
    nodetest.cpp)

test_case_config (TestNode)

# TestEdge
add_executable (TestEdge
    ../source/model/base/node.hpp
    ../source/model/base/edge.hpp
    edgetest.cpp)

test_case_config (TestEdge)

# TestGraph
add_executable (TestGraph
    ../source/model/base/node.hpp
    ../source/model/base/edge.hpp
    ../source/model/base/graph.hpp
    ../source/model/base/graphdata.hpp
    graphtest.cpp)

test_case_config (TestGraph)

# TestForest
add_executable (TestForest
    ../source/model/base/node.hpp
    ../source/model/base/edge.hpp
    ../source/model/base/graph.hpp
    ../source/model/base/graphdata.hpp
    ../source/model/base/forest.hpp
    ../source/model/enums.h
    foresttest.cpp)

test_case_config (TestForest)

# TestGraphicItem
add_executable (TestGraphicItem
    ../source/model/graphicitem/graphicitem.h
    ../source/model/graphicitem/graphicitem.cpp
    graphicitemtest.cpp)

test_case_config (TestGraphicItem)

# TestTextCursor
add_executable (TestTextCursor
    ../source/helpers/text/chartools.h
    ../source/helpers/text/checkingtextcursor.cpp
    ../source/helpers/text/checkingtextcursor.h
    ../source/helpers/text/textcursor.cpp
    ../source/helpers/text/textcursor.h
    cursortest.cpp)

test_case_config (TestTextCursor)
target_link_libraries (TestTextCursor CommonTools)

# TestTextRange
add_executable (TestTextRange
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
    textrangetest.cpp)

test_case_config (TestTextRange)
target_link_libraries (TestTextRange CommonTools)


# TestTextLink
add_executable (TestTextLink
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
    linktest.cpp)

test_case_config (TestTextLink)
target_link_libraries (TestTextLink CommonTools)

# TestLinkUtils
add_executable (TestLinkUtils
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
    linkutilstest.cpp)

test_case_config (TestLinkUtils)
target_link_libraries (TestLinkUtils CommonTools)

# TestLinkDecorator
add_executable (TestLinkDecorator
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
    linkdecoratortest.cpp)

test_case_config (TestLinkDecorator)
target_link_libraries (TestLinkDecorator Qt6::Gui CommonTools)
