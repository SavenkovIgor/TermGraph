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
    target_include_directories (${TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR} ${GTest_INCLUDE_DIRS})
    target_link_libraries (${TEST_NAME} Qt6::Core GTest::gtest GTest::gtest_main)
    set_target_properties (${TEST_NAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)
endfunction ()


# TEXT_UTILS_TEST
add_executable (TEXT_UTILS_TEST
    ../source/Helpers/text/textutils.cpp
    ../source/Helpers/text/textutils.h
    textutilstest.cpp)

test_case_config (TEXT_UTILS_TEST)


# FS_WORKS_TEST
add_executable (FS_WORKS_TEST
    ../source/Helpers/fsworks.cpp
    ../source/Helpers/fsworks.h
    fsworkstest.cpp)

test_case_config (FS_WORKS_TEST)


# UUIDS_TEST
add_executable (UUIDS_TEST
    ../source/Helpers/uuid/uuidtools.cpp
    ../source/Helpers/uuid/uuidtools.h
    uuidstest.cpp)

test_case_config (UUIDS_TEST)

# NODE_TEST
add_executable(NODE_TEST
    ../source/Model/Base/node.hpp
    nodetest.cpp)

test_case_config (NODE_TEST)

# EDGE_TEST
add_executable(EDGE_TEST
    ../source/Model/Base/node.hpp
    ../source/Model/Base/edge.hpp
    edgetest.cpp)

test_case_config (EDGE_TEST)

# GRAPH_TEST
add_executable(GRAPH_TEST
    ../source/Model/Base/node.hpp
    ../source/Model/Base/edge.hpp
    ../source/Model/Base/graph.hpp
    graphtest.cpp)

test_case_config (GRAPH_TEST)


# TEXT_CURSOR_TEST
add_executable (TEXT_CURSOR_TEST
    ../source/Helpers/handytypes.h
    ../source/Helpers/text/chartools.h
    ../source/Helpers/text/checkingtextcursor.cpp
    ../source/Helpers/text/checkingtextcursor.h
    ../source/Helpers/text/textcursor.cpp
    ../source/Helpers/text/textcursor.h
    cursortest.cpp)

test_case_config (TEXT_CURSOR_TEST)


# TEXT_RANGE_TEST
add_executable (TEXT_RANGE_TEST
    ../source/Helpers/handytypes.h
    ../source/Helpers/intmatrix.cpp
    ../source/Helpers/intmatrix.h
    ../source/Helpers/link/link.cpp
    ../source/Helpers/link/link.h
    ../source/Helpers/text/chartools.h
    ../source/Helpers/text/checkingtextcursor.cpp
    ../source/Helpers/text/checkingtextcursor.h
    ../source/Helpers/text/textcursor.cpp
    ../source/Helpers/text/textcursor.h
    ../source/Helpers/text/textrange.cpp
    ../source/Helpers/text/textrange.h
    ../source/Helpers/uuid/uuidtools.cpp
    ../source/Helpers/uuid/uuidtools.h
    textrangetest.cpp)

test_case_config (TEXT_RANGE_TEST)


# TEXT_LINK_TEST
add_executable (TEXT_LINK_TEST
    ../source/Helpers/handytypes.h
    ../source/Helpers/intmatrix.cpp
    ../source/Helpers/intmatrix.h
    ../source/Helpers/link/linksstring.cpp
    ../source/Helpers/link/linksstring.h
    ../source/Helpers/link/link.cpp
    ../source/Helpers/link/link.h
    ../source/Helpers/text/chartools.h
    ../source/Helpers/text/checkingtextcursor.cpp
    ../source/Helpers/text/checkingtextcursor.h
    ../source/Helpers/text/textcursor.cpp
    ../source/Helpers/text/textcursor.h
    ../source/Helpers/text/textrange.cpp
    ../source/Helpers/text/textrange.h
    ../source/Helpers/uuid/uuidtools.cpp
    ../source/Helpers/uuid/uuidtools.h
    linktest.cpp)

test_case_config (TEXT_LINK_TEST)

# LINK_UTILS_TEST
add_executable (LINK_UTILS_TEST
    ../source/Helpers/handytypes.h
    ../source/Helpers/intmatrix.cpp
    ../source/Helpers/intmatrix.h
    ../source/Helpers/link/linkutils.cpp
    ../source/Helpers/link/linkutils.h
    ../source/Helpers/link/link.cpp
    ../source/Helpers/link/link.h
    ../source/Helpers/text/chartools.h
    ../source/Helpers/text/checkingtextcursor.cpp
    ../source/Helpers/text/checkingtextcursor.h
    ../source/Helpers/text/textcursor.cpp
    ../source/Helpers/text/textcursor.h
    ../source/Helpers/text/textrange.cpp
    ../source/Helpers/text/textrange.h
    ../source/Helpers/text/textutils.cpp
    ../source/Helpers/text/textutils.h
    ../source/Helpers/uuid/uuidtools.cpp
    ../source/Helpers/uuid/uuidtools.h
    linkutilstest.cpp)

test_case_config (LINK_UTILS_TEST)


# LINK_DECORATOR_TEST
add_executable (LINK_DECORATOR_TEST
    ../source/Helpers/handytypes.h
    ../source/Helpers/intmatrix.cpp
    ../source/Helpers/intmatrix.h
    ../source/Helpers/link/linksdecorator.cpp
    ../source/Helpers/link/linksdecorator.h
    ../source/Helpers/link/linksstring.cpp
    ../source/Helpers/link/linksstring.h
    ../source/Helpers/link/link.cpp
    ../source/Helpers/link/link.h
    ../source/Helpers/text/chartools.h
    ../source/Helpers/text/checkingtextcursor.cpp
    ../source/Helpers/text/checkingtextcursor.h
    ../source/Helpers/text/textcursor.cpp
    ../source/Helpers/text/textcursor.h
    ../source/Helpers/text/textrange.cpp
    ../source/Helpers/text/textrange.h
    ../source/Helpers/uuid/uuidtools.cpp
    ../source/Helpers/uuid/uuidtools.h
    linkdecoratortest.cpp)

test_case_config (LINK_DECORATOR_TEST)
target_link_libraries (LINK_DECORATOR_TEST Qt6::Gui)

# DB_WORKS_TEST
add_executable (DB_WORKS_TEST
    ../source/Database/database.cpp
    ../source/Database/database.h
    ../source/Database/dbinfo.h
    ../source/Database/dbtools.cpp
    ../source/Database/dbtools.h
    ../source/Database/sqlqueries.qrc
    ../source/Database/sqlquerybuilder.cpp
    ../source/Database/sqlquerybuilder.h
    ../source/Database/tables/appconfigtable.cpp
    ../source/Database/tables/appconfigtable.h
    ../source/Database/tables/termgrouptable.cpp
    ../source/Database/tables/termgrouptable.h
    ../source/Database/tables/termtable.cpp
    ../source/Database/tables/termtable.h
    ../source/Helpers/appconfig.cpp
    ../source/Helpers/appconfig.h
    ../source/Helpers/fsworks.cpp
    ../source/Helpers/fsworks.h
    ../source/Helpers/handytypes.h
    ../source/Managers/datastorageinterface.h
    ../source/Managers/localdatabasestorage.cpp
    ../source/Managers/localdatabasestorage.h
    ../source/Managers/notificationmanager.cpp
    ../source/Managers/notificationmanager.h
    ../source/Model/Termin/nodeinfocontainer.h
    ../source/Model/TerminGroup/groupinfocontainer.h
    dbworkstest.cpp)

test_case_config (DB_WORKS_TEST)
target_link_libraries (DB_WORKS_TEST Qt6::Sql)
set_target_properties (DB_WORKS_TEST PROPERTIES AUTOMOC ON AUTORCC ON)
