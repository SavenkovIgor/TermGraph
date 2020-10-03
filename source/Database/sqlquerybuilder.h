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

#include <QLatin1String>
#include <QSqlQuery>
#include <QString>
#include <QtCore>

#include "source/Database/dbinfo.h"
#include "source/Database/tools/querytools.h"
#include "source/Database/tools/wherecondition.h"
#include "source/Model/Termin/nodeinfocontainer.h"
#include "source/Model/TerminGroup/groupinfocontainer.h"

// Задача этого класса - формировать запросы
// Clear static class
class SqlQueryBuilder
{
public:
    SqlQueryBuilder(const char* const connectionName = DbConnectionName::defaultConnection);

    // Deprecation
    QString selectQuery(const QString&        tableName,
                        const QStringList&    columns,
                        const WhereCondition& where,
                        const QString&        orderBy = "") const;

    // AppConfig table
    QSqlQuery createAppConfigTable() const;
    QSqlQuery insertConfigParameter(const QString& parameter, const QString& value) const;
    QSqlQuery selectOneConfigParameter(const QString& parameter) const;
    QSqlQuery selectConfigParameter(const QString& parameter) const;
    QSqlQuery updateConfigParameter(const QString& parameter, const QString& newValue) const;

    // Groups table
    QSqlQuery createGroupsTable() const;
    QSqlQuery insertGroup(const GroupInfoContainer& groupInfo) const;
    QSqlQuery selectGroup(const QUuid& groupUuid) const;
    QSqlQuery selectGroup(const QString& groupName) const;
    QSqlQuery selectOneGroup(const QUuid& groupUuid) const;
    QSqlQuery selectAllGroupUuids();
    QSqlQuery selectAllGroups();
    QSqlQuery updateGroup(const GroupInfoContainer& groupInfo) const;
    QSqlQuery deleteGroup(const QUuid& groupUuid) const;

    // Terms table
    QSqlQuery createTermsTable() const;
    QSqlQuery insertTerm(const NodeInfoContainer& termInfo) const;
    QSqlQuery selectTerm(const QUuid& termUuid) const;
    QSqlQuery selectOneTerm(const QUuid& termUuid) const;
    QSqlQuery selectOneTerm(const QString& term, const QUuid& groupUuid) const;
    QSqlQuery selectAllTermUuids() const;
    QSqlQuery selectAllTermUuids(const QUuid& groupUuid) const;
    QSqlQuery selectAllTerms(const QUuid& groupUuid) const;
    QSqlQuery selectLastEdit(const QUuid& termUuid) const;
    QSqlQuery selectAllLastEditAndGroupUuid() const;
    QSqlQuery updateTerm(const NodeInfoContainer& termInfo) const;
    QSqlQuery deleteTerm(const QUuid& termUuid) const;

    // Common
    QSqlQuery loadQuery(const QString& queryPath) const;
    QSqlQuery recordsCount(const QString& tableName) const;
    QSqlQuery dropTable(const QString& tableName) const;

private: // Methods
    QString loadQueryString(const QString& queryPath) const;

    const QLatin1String mConnectionName;
};
