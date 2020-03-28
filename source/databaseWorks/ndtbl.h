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

#include "source/Helpers/handytypes.h"
#include "source/Model/Termin/nodeinfocontainer.h"
#include "source/databaseWorks/dbtablenames.h"
#include "source/databaseWorks/tblbase.h"

#include "source/databaseWorks/columns/tcolumn.h"

class NodeTable : public TblBase
{
public:
    enum LastEditSource { TakeFromNodeInfo = 0, AutoGenerate };

    NodeTable(QSqlDatabase* base)
        : TblBase(base)
    {}
    ~NodeTable() override = default;

    QUuid nodeUuidForNameAndGroup(const QString& name, const QUuid& groupUuid) const;

    QUuid addNode(const QString& name, const QUuid& groupUuid);
    QUuid addNode(const QUuid& uuid, const QString& name, const QUuid& groupUuid);
    QUuid addNode(const NodeInfoContainer& info);  // TODO: remove all other functioins and change to bool

    bool nodeExist(const QUuid& nodeUuid);

    UuidList                getAllNodesUuids(const QUuid& groupUuid = QUuid());
    NodeInfoContainer       getNode(const QUuid& uuid);
    NodeInfoContainer::List getAllNodesInfo(const QUuid& groupUuid);

    QDateTime getLastEdit(const QUuid& uuid);
    RecVector getAllLastEditRecords();

    bool updateNode(const NodeInfoContainer& info, LastEditSource lastEditSource, bool checkLastEdit = true);
    void deleteNode(const QUuid& uuid);

    const char* tableName() const override;
    TColumn     primaryKey() const override;
    void        initTable() override;

protected:
    TColumn::List getAllColumns() const override;

private:
    void setFieldUpdateLastEdit(const TColumn& column, const QUuid& uuid, const QString& val);
    void updateLastEdit(const QUuid& uuid);
    bool isUuidExist(const QUuid& uuid);

    QUuid generateNewUuid();

    static QDateTime getLastEditNow();
    static QString   getLastEditNowString();

    QSqlRecord getNodeSqlRecord(const QUuid& uuid);  // TODO: Delete!

    NodeInfoContainer recordToNodeInfo(QSqlRecord& record);

    WhereCondition whereUuidEqual(const QUuid& uuid);
};
