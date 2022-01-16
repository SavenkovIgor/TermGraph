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

// TODO: Naming!
class TermTable
{
public:
    using RecordList = std::vector<QSqlRecord>;

    TermTable()  = default;
    ~TermTable() = default;

    Opt<TermUuid> nodeUuidForNameAndGroup(const QString& name, const GroupUuid& uuid) const;

    Result<void> addNode(const TermData& info);

    bool nodeExist(const TermUuid& uuid);

    TermUuid::List         getAllNodesUuids(Opt<GroupUuid> uuid = std::nullopt);
    Result<TermData>       getNodeInfo(const TermUuid& uuid);
    Result<TermData::List> getAllNodesInfo(const GroupUuid& uuid);

    Result<QDateTime> getLastEdit(const TermUuid& uuid);
    RecordList        getAllLastEditRecords();

    Result<void> updateNode(const TermData&                      info,
                            DataStorageInterface::LastEditSource lastEditSource,
                            bool                                 checkLastEdit = true);
    Result<void> deleteTerm(const TermUuid& uuid);

    void initTable();

private:
    bool isUuidExist(const TermUuid& uuid);

    TermUuid generateNewUuid();

    static QDateTime getLastEditNow();

    TermData recordToNodeInfo(QSqlRecord& record);
};
