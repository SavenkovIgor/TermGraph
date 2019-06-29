#pragma once

#include "source/databaseWorks/columns/tcolumn.h"

class NodeColumn {
public:
    // TODO: Грохнуть uid
    constexpr static auto uid         = TColumn("uid",         "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL");
    constexpr static auto uuid        = TColumn("longUID",     "TEXT");
    constexpr static auto term        = TColumn("term",        "TEXT");
    constexpr static auto termForms   = TColumn("termForms",   "TEXT");
    constexpr static auto definition  = TColumn("definition",  "TEXT");
    constexpr static auto description = TColumn("description", "TEXT");
    constexpr static auto examples    = TColumn("examples",    "TEXT");
    constexpr static auto wikiUrl     = TColumn("wikiRef",     "TEXT");
    constexpr static auto wikiImage   = TColumn("wikiImg",     "TEXT");
    constexpr static auto groupUuid   = TColumn("termGroup",   "INTEGER DEFAULT '-1'");
    constexpr static auto lastRemind  = TColumn("lastRemind",  "TEXT");
    constexpr static auto remindNum   = TColumn("remindNum",   "INTEGER DEFAULT '0'");
    constexpr static auto atLearn     = TColumn("atLearn",     "INTEGER DEFAULT '0'");
    constexpr static auto lastEdit    = TColumn("lastEdit",    "TEXT");

    constexpr static TColumn columns[] = {
        uid,
        uuid,
        term,
        termForms,
        definition,
        description,
        examples,
        wikiUrl,
        wikiImage,
        groupUuid,
        lastRemind,
        remindNum,
        atLearn,
        lastEdit
    };
};
