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

find_package (Qt5Test REQUIRED)

set (TEST_PROJECT_NAME ${PROJECT_NAME}_db_works_test)

add_executable (${TEST_PROJECT_NAME})

include_directories (${CMAKE_SOURCE_DIR})

# TODO Reduce files count
target_sources (
    ${TEST_PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/source/Helpers/fsworks.h
            ${CMAKE_SOURCE_DIR}/source/Helpers/fsworks.cpp
            ${CMAKE_SOURCE_DIR}/source/Helpers/appconfig.h
            ${CMAKE_SOURCE_DIR}/source/Helpers/appconfig.cpp
            ${CMAKE_SOURCE_DIR}/source/Database/appconfigtable.h
            ${CMAKE_SOURCE_DIR}/source/Database/appconfigtable.cpp
            ${CMAKE_SOURCE_DIR}/source/Database/database.h
            ${CMAKE_SOURCE_DIR}/source/Database/database.cpp
            ${CMAKE_SOURCE_DIR}/source/Database/ndtbl.h
            ${CMAKE_SOURCE_DIR}/source/Database/ndtbl.cpp
            ${CMAKE_SOURCE_DIR}/source/Database/sqlqueryconstructor.h
            ${CMAKE_SOURCE_DIR}/source/Database/sqlqueryconstructor.cpp
            ${CMAKE_SOURCE_DIR}/source/Database/tblbase.h
            ${CMAKE_SOURCE_DIR}/source/Database/tblbase.cpp
            ${CMAKE_SOURCE_DIR}/source/Database/termgrouptbl.h
            ${CMAKE_SOURCE_DIR}/source/Database/termgrouptbl.cpp
            ${CMAKE_SOURCE_DIR}/source/Database/tools/dbtools.h
            ${CMAKE_SOURCE_DIR}/source/Database/tools/dbtools.cpp
            ${CMAKE_SOURCE_DIR}/source/Database/tools/insertcontainer.h
            ${CMAKE_SOURCE_DIR}/source/Database/tools/insertcontainer.cpp
            ${CMAKE_SOURCE_DIR}/source/Database/tools/setexpression.h
            ${CMAKE_SOURCE_DIR}/source/Database/tools/setexpression.cpp
            ${CMAKE_SOURCE_DIR}/source/Database/tools/wherecondition.h
            ${CMAKE_SOURCE_DIR}/source/Database/tools/wherecondition.cpp
            ${CMAKE_SOURCE_DIR}/source/Managers/notificationmanager.h
            ${CMAKE_SOURCE_DIR}/source/Managers/notificationmanager.cpp
            ${CMAKE_SOURCE_DIR}/source/Managers/datastorageinterface.h
            ${CMAKE_SOURCE_DIR}/source/Managers/localdatabasestorage.h
            ${CMAKE_SOURCE_DIR}/source/Managers/localdatabasestorage.cpp
            ${CMAKE_SOURCE_DIR}/source/Database/sqlqueries.qrc
            dbworkstest.cpp)

add_test (NAME ${TEST_PROJECT_NAME} COMMAND ${TEST_PROJECT_NAME})

target_link_libraries (${TEST_PROJECT_NAME} PRIVATE Qt5::Test Qt5::Sql)
