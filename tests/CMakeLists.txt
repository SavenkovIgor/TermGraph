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

find_package (Qt5Test REQUIRED)

include_directories (${CMAKE_SOURCE_DIR})

# LINK_UTILS_TEST
set (LINK_UTILS_TEST "LinkUtilsTest")

add_executable (${LINK_UTILS_TEST})
target_link_libraries (${LINK_UTILS_TEST} PRIVATE Qt5::Test)
add_test (NAME ${LINK_UTILS_TEST} COMMAND ${LINK_UTILS_TEST})
set_target_properties (${LINK_UTILS_TEST} PROPERTIES
    AUTOMOC ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON)

target_sources (
    ${LINK_UTILS_TEST}
    PRIVATE ../source/Helpers/handytypes.h
            ../source/Helpers/intmatrix.cpp
            ../source/Helpers/intmatrix.h
            ../source/Helpers/linkutils.cpp
            ../source/Helpers/linkutils.h
            ../source/Helpers/text/chartools.h
            ../source/Helpers/text/checkingtextcursor.cpp
            ../source/Helpers/text/checkingtextcursor.h
            ../source/Helpers/text/textcursor.cpp
            ../source/Helpers/text/textcursor.h
            ../source/Helpers/text/textlink.cpp
            ../source/Helpers/text/textlink.h
            ../source/Helpers/text/textrange.cpp
            ../source/Helpers/text/textrange.h
            ../source/Helpers/uuid/uuidtools.cpp
            ../source/Helpers/uuid/uuidtools.h
            ../source/Helpers/validators/linktextvalidator.cpp
            ../source/Helpers/validators/linktextvalidator.h
            ../source/Helpers/validators/uuidvalidator.cpp
            ../source/Helpers/validators/uuidvalidator.h
            linkutilstest.cpp)


# TEXT_PROCESSOR_TEST
set (TEXT_PROCESSOR_TEST "TextProcessorTest")

add_executable (${TEXT_PROCESSOR_TEST})
target_link_libraries (${TEXT_PROCESSOR_TEST} PRIVATE Qt5::Test)
add_test (NAME ${TEXT_PROCESSOR_TEST} COMMAND ${TEXT_PROCESSOR_TEST})
set_target_properties (${TEXT_PROCESSOR_TEST} PROPERTIES
    AUTOMOC ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON)

target_sources (
    ${TEXT_PROCESSOR_TEST}
    PRIVATE ../source/Helpers/textutils.cpp
            ../source/Helpers/textutils.h
            textprocessortest.cpp)


# FS_WORKS_TEST
set (FS_WORKS_TEST "FsWorksTest")

add_executable (${FS_WORKS_TEST})
target_link_libraries (${FS_WORKS_TEST} PRIVATE Qt5::Test)
add_test (NAME ${FS_WORKS_TEST} COMMAND ${FS_WORKS_TEST})
set_target_properties (${FS_WORKS_TEST} PROPERTIES
    AUTOMOC ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON)

target_sources (
    ${FS_WORKS_TEST}
    PRIVATE ../source/Helpers/fsworks.h
            ../source/Helpers/fsworks.cpp
            fsworkstest.cpp)


# DB_WORKS_TEST
set (DB_WORKS_TEST "DbWorksTest")

add_executable (${DB_WORKS_TEST})
target_link_libraries (${DB_WORKS_TEST} PRIVATE Qt5::Test Qt5::Sql)
add_test (NAME ${DB_WORKS_TEST} COMMAND ${DB_WORKS_TEST})
set_target_properties (${DB_WORKS_TEST} PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON)

# TODO Reduce files count
target_sources (
    ${DB_WORKS_TEST}
    PRIVATE ../source/Database/database.cpp
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


# UUIDS_TEST
set (UUIDS_TEST "UuidsTest")

add_executable (${UUIDS_TEST})
target_link_libraries (${UUIDS_TEST} PRIVATE Qt5::Test)
add_test (NAME ${UUIDS_TEST} COMMAND ${UUIDS_TEST})
set_target_properties (${UUIDS_TEST} PROPERTIES
    AUTOMOC ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON)

# TODO Reduce files count
target_sources (
    ${UUIDS_TEST}
    PRIVATE ../source/Helpers/uuid/uuidtools.cpp
            ../source/Helpers/uuid/uuidtools.h
            ../source/Helpers/validators/uuidvalidator.cpp
            ../source/Helpers/validators/uuidvalidator.h
            uuidstest.cpp)


# TEXT_CURSOR_TEST
set (TEXT_CURSOR_TEST "TextCursorTest")

add_executable (${TEXT_CURSOR_TEST})
target_link_libraries (${TEXT_CURSOR_TEST} PRIVATE Qt5::Test)
add_test (NAME ${TEXT_CURSOR_TEST} COMMAND ${TEXT_CURSOR_TEST})
set_target_properties (${TEXT_CURSOR_TEST} PROPERTIES
    AUTOMOC ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON)

target_sources (
    ${TEXT_CURSOR_TEST}
    PRIVATE ../source/Helpers/text/textcursor.cpp
            ../source/Helpers/text/textcursor.h
            ../source/Helpers/text/checkingtextcursor.cpp
            ../source/Helpers/text/checkingtextcursor.h
            ../source/Helpers/text/chartools.h
            ../source/Helpers/handytypes.h
            cursortest.cpp)


# TEXT_RANGE_TEST
set (TEXT_RANGE_TEST "TextRangeTest")

add_executable (${TEXT_RANGE_TEST})
target_link_libraries (${TEXT_RANGE_TEST} PRIVATE Qt5::Test)
add_test (NAME ${TEXT_RANGE_TEST} COMMAND ${TEXT_RANGE_TEST})
set_target_properties (${TEXT_RANGE_TEST} PROPERTIES
    AUTOMOC ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON)

target_sources (
    ${TEXT_RANGE_TEST}
    PRIVATE ../source/Helpers/text/textcursor.cpp
            ../source/Helpers/text/textcursor.h
            ../source/Helpers/text/checkingtextcursor.cpp
            ../source/Helpers/text/checkingtextcursor.h
            ../source/Helpers/text/textrange.cpp
            ../source/Helpers/text/textrange.h
            ../source/Helpers/text/textlink.cpp
            ../source/Helpers/text/textlink.h
            ../source/Helpers/uuid/uuidtools.cpp
            ../source/Helpers/uuid/uuidtools.h
            ../source/Helpers/intmatrix.cpp
            ../source/Helpers/intmatrix.h
            ../source/Helpers/text/chartools.h
            ../source/Helpers/handytypes.h
            ../source/Helpers/validators/uuidvalidator.cpp
            ../source/Helpers/validators/uuidvalidator.h
            ../source/Helpers/validators/linktextvalidator.cpp
            ../source/Helpers/validators/linktextvalidator.h
            textrangetest.cpp)


# TEXT_LINK_TEST
set (TEXT_LINK_TEST "TextLinkTest")

add_executable (${TEXT_LINK_TEST})
target_link_libraries (${TEXT_LINK_TEST} PRIVATE Qt5::Test)
add_test (NAME ${TEXT_LINK_TEST} COMMAND ${TEXT_LINK_TEST})
set_target_properties (${TEXT_LINK_TEST} PROPERTIES
    AUTOMOC ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON)

target_sources (
    ${TEXT_LINK_TEST}
    PRIVATE ../source/Helpers/text/textcursor.cpp
            ../source/Helpers/text/textcursor.h
            ../source/Helpers/text/checkingtextcursor.cpp
            ../source/Helpers/text/checkingtextcursor.h
            ../source/Helpers/text/textrange.cpp
            ../source/Helpers/text/textrange.h
            ../source/Helpers/text/chartools.h
            ../source/Helpers/handytypes.h
            ../source/Helpers/text/textlink.cpp
            ../source/Helpers/text/textlink.h
            ../source/Helpers/text/linkstext.cpp
            ../source/Helpers/text/linkstext.h
            ../source/Helpers/intmatrix.cpp
            ../source/Helpers/intmatrix.h
            ../source/Helpers/uuid/uuidtools.cpp
            ../source/Helpers/uuid/uuidtools.h
            ../source/Helpers/validators/uuidvalidator.cpp
            ../source/Helpers/validators/uuidvalidator.h
            ../source/Helpers/validators/linktextvalidator.cpp
            ../source/Helpers/validators/linktextvalidator.h
            textlinktest.cpp)
