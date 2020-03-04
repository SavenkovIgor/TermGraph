/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "source/databaseWorks/columns/tcolumn.h"

class NodeColumn
{
public:
    constexpr static auto uuid        = TColumn("uuid", "TEXT PRIMARY KEY NOT NULL");
    constexpr static auto term        = TColumn("term", "TEXT NOT NULL");
    constexpr static auto definition  = TColumn("definition", "TEXT");
    constexpr static auto description = TColumn("description", "TEXT");
    constexpr static auto examples    = TColumn("examples", "TEXT");
    constexpr static auto wikiUrl     = TColumn("wikiUrl", "TEXT");
    constexpr static auto wikiImage   = TColumn("wikiImage", "TEXT");
    constexpr static auto groupUuid   = TColumn("groupUuid", "TEXT NOT NULL");
    constexpr static auto lastEdit    = TColumn("lastEdit", "TEXT NOT NULL");

    constexpr static TColumn columns[]
        = {uuid, term, definition, description, examples, wikiUrl, wikiImage, groupUuid, lastEdit};
};
