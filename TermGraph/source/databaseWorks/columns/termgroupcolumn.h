#pragma once

#include "source/databaseWorks/columns/tcolumn.h"

class TermGroupColumn {
public:
    constexpr static auto uuid    = TColumn("longUID", "TEXT");
    constexpr static auto name    = TColumn("name",    "TEXT UNIQUE");
    constexpr static auto comment = TColumn("comment", "TEXT");
    constexpr static auto type    = TColumn("type",    "INTEGER DEFAULT '-1'");

    constexpr static TColumn columns[] = {
        uuid,
        name,
        comment,
        type
    };
};
