/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
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

#include <QSqlRecord>

#include <CommonTools/HandyTypes.h>
#include <TermDataInterface/DataStorageInterface.h>
#include <TermDataInterface/TermData.h>

class TermTable
{
public:
    using RecordList = std::vector<QSqlRecord>;

    TermTable()  = default;
    ~TermTable() = default;

    // Table stuff
    void initTable();

    // Read
    bool              exist(const TermUuid& uuid);
    Opt<TermUuid>     find(const QString& term, const GroupUuid& uuid) const;
    Result<TermData>  term(const TermUuid& uuid);
    Result<QDateTime> lastEdit(const TermUuid& uuid);

    TermUuid::List         allUuids(Opt<GroupUuid> uuid = std::nullopt);
    Result<TermData::List> allTerms(const GroupUuid& uuid);
    RecordList             allLastEditRecords();

    // Modify
    Result<void> addTerm(const TermData& info);
    Result<void> updateTerm(const TermData&                      info,
                            DataStorageInterface::LastEditSource lastEditSource,
                            bool                                 checkLastEdit = true);
    Result<void> deleteTerm(const TermUuid& uuid);


private:
    TermUuid generateNewUuid();

    static QDateTime now();

    TermData createTermData(QSqlRecord& record);
};
