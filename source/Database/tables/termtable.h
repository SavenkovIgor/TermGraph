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

#include "source/Helpers/handytypes.h"
#include "source/Managers/datastorageinterface.h"
#include "source/Model/Termin/nodeinfocontainer.h"

class TermTable
{
public:
    TermTable()  = default;
    ~TermTable() = default;

    QUuid nodeUuidForNameAndGroup(const QString& name, const QUuid& groupUuid) const;

    bool addNode(const NodeInfoContainer& info);

    bool nodeExist(const QUuid& nodeUuid);

    UuidList                getAllNodesUuids(const QUuid& groupUuid = QUuid());
    NodeInfoContainer       getNodeInfo(const QUuid& uuid);
    NodeInfoContainer::List getAllNodesInfo(const QUuid& groupUuid);

    QDateTime  getLastEdit(const QUuid& uuid);
    RecordList getAllLastEditRecords();

    bool updateNode(const NodeInfoContainer&             info,
                    DataStorageInterface::LastEditSource lastEditSource,
                    bool                                 checkLastEdit = true);
    void deleteTerm(const QUuid& termUuid);

    void initTable();

private:
    bool isUuidExist(const QUuid& uuid);

    QUuid generateNewUuid();

    static QDateTime getLastEditNow();

    NodeInfoContainer recordToNodeInfo(QSqlRecord& record);
};
