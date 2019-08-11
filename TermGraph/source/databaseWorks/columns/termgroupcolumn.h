#pragma once

#include "source/databaseWorks/columns/tcolumn.h"

class TermGroupColumn {
public:
    constexpr static auto uuid    = TColumn("uuid",    "TEXT PRIMARY KEY NOT NULL");
    constexpr static auto name    = TColumn("name",    "TEXT UNIQUE NOT NULL");
    constexpr static auto comment = TColumn("comment", "TEXT");

    constexpr static TColumn columns[] = {uuid, name, comment};
};
