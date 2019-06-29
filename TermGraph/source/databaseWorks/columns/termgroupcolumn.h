#pragma once

#include "source/databaseWorks/columns/tcolumn.h"

class TermGroupColumn {
public:
    // TODO: Грохнуть короткий uid
    constexpr static auto uid     = TColumn("uid",     "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL");
    constexpr static auto longUID = TColumn("longUID", "TEXT");
    constexpr static auto name    = TColumn("name",    "TEXT UNIQUE");
    constexpr static auto comment = TColumn("comment", "TEXT");
    constexpr static auto type    = TColumn("type",    "INTEGER DEFAULT '-1'");

    constexpr static TColumn columns[] = {
        uid,
        longUID,
        name,
        comment,
        type
    };
};
