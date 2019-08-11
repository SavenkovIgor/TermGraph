#pragma once

#include "source/databaseWorks/columns/tcolumn.h"

class NodeColumn
{
public:
    constexpr static auto uuid        = TColumn("uuid",        "TEXT PRIMARY KEY NOT NULL");
    constexpr static auto term        = TColumn("term",        "TEXT NOT NULL");
    constexpr static auto definition  = TColumn("definition",  "TEXT");
    constexpr static auto description = TColumn("description", "TEXT");
    constexpr static auto examples    = TColumn("examples",    "TEXT");
    constexpr static auto wikiUrl     = TColumn("wikiUrl",     "TEXT");
    constexpr static auto wikiImage   = TColumn("wikiImage",   "TEXT");
    constexpr static auto groupUuid   = TColumn("groupUuid",   "TEXT NOT NULL");
    constexpr static auto lastEdit    = TColumn("lastEdit",    "TEXT NOT NULL");

    constexpr static TColumn columns[]
        = {uuid, term, definition, description, examples, wikiUrl, wikiImage, groupUuid, lastEdit};
};
