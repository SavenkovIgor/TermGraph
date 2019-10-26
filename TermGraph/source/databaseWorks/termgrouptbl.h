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

#include "source/databaseWorks/tblbase.h"
#include "source/databaseWorks/dbtablenames.h"
#include "source/databaseWorks/columns/tcolumn.h"
#include "source/Model/TerminGroup/groupinfocontainer.h"

class TermGroupTable : public TblBase
{
public:
    TermGroupTable(QSqlDatabase* base)
        : TblBase(base)
    {}
    ~TermGroupTable() override = default;

    // Add, update, delete
    bool addGroup(const GroupInfoContainer& info);
    bool updateGroup(const GroupInfoContainer& info);
    void deleteGroup(const QUuid& uuid);

    // Getters
    GroupInfoContainer getGroup(const QUuid& uuid);
    RecVector getAllUuidsAndNames();
    UuidList getAllUuids();

    QUuid getUuid(const QString& groupName) const;

    // Checkers
    bool hasGroupWithUuid(const QUuid& uuid);
    bool hasGroupWithName(const QString& groupName);

    const char* tableName() const override;
    TColumn primaryKey() const override;
    void initTable() override;

protected:
    TColumn::List getAllColumns() const override;

private:
    QUuid generateNewUuid();

    WhereCondition whereUuidEqual(const QUuid& uuid);
};
