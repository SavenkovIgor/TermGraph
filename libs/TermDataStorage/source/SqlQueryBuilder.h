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

#include <CommonTools/GroupUuid.h>
#include <CommonTools/TermUuid.h>

#include <TermDataInterface/GroupData.h>
#include <TermDataInterface/TermData.h>

#include "source/DbInfo.h"

// Задача этого класса - формировать запросы
class SqlQueryBuilder
{
public:
    SqlQueryBuilder() = default;

    // AppConfig table
    QSqlQuery createAppConfigTable() const;
    QSqlQuery insertConfigParameter(const QString& parameter, const QString& value) const;
    QSqlQuery selectOneConfigParameter(const QString& parameter) const;
    QSqlQuery selectConfigParameter(const QString& parameter) const;
    QSqlQuery updateConfigParameter(const QString& parameter, const QString& newValue) const;

    // Groups table
    QSqlQuery createGroupsTable() const;
    QSqlQuery insertGroup(const GroupData& groupInfo) const;
    QSqlQuery selectGroup(const GroupUuid& uuid) const;
    QSqlQuery selectGroup(const QString& groupName) const;
    QSqlQuery selectOneGroup(const GroupUuid& uuid) const;
    QSqlQuery selectAllGroupUuids();
    QSqlQuery selectAllGroups();
    QSqlQuery updateGroup(const GroupData& groupInfo) const;
    QSqlQuery deleteGroup(const GroupUuid& uuid) const;

    // Terms table
    QSqlQuery createTermsTable() const;
    QSqlQuery insertTerm(const TermData& termInfo) const;
    QSqlQuery selectTerm(const TermUuid& uuid) const;
    QSqlQuery selectOneTerm(const TermUuid& uuid) const;
    QSqlQuery selectOneTerm(const QString& term, const GroupUuid& uuid) const;
    QSqlQuery selectAllTerms(const GroupUuid& uuid) const;
    QSqlQuery selectLastEdit(const TermUuid& uuid) const;
    QSqlQuery selectAllLastEditAndGroupUuid() const;
    QSqlQuery updateTerm(const TermData& termInfo) const;
    QSqlQuery deleteTerm(const TermUuid& uuid) const;

    // Common
    QSqlQuery loadQuery(const QString& queryPath) const;
    QSqlQuery recordsCount(const QString& tableName) const;
    QSqlQuery dropTable(const QString& tableName) const;

private: // Methods
    QString loadQueryString(const QString& queryPath) const;

    static QLatin1String connectionNameForCurrentThread();
    static QSqlDatabase  getDbForConnection(QLatin1String connectionName);
};
