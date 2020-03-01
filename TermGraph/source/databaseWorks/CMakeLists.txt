#  TermGraph, build graph of knowledge.
#  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

cmake_minimum_required (VERSION 3.16.0)

add_subdirectory(columns)
add_subdirectory(tools)

target_sources(${PROJECT_NAME}
    PRIVATE
        appconfigtable.cpp
        appconfigtable.h
        cloudservices.cpp
        cloudservices.h
        database.cpp
        database.h
        dbtablenames.h
        dropboxconnection.cpp
        dropboxconnection.h
        edgtbl.cpp
        edgtbl.h
        ndtbl.cpp
        ndtbl.h
        sqlqueryconstructor.cpp
        sqlqueryconstructor.h
        tblbase.cpp
        tblbase.h
        termgrouptbl.cpp
        termgrouptbl.h
)
