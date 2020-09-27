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

#include <QSqlQuery>
#include <QString>
#include <QtCore>

#include "source/Database/columns/tcolumn.h"
#include "source/Database/tools/querytools.h"
#include "source/Database/tools/wherecondition.h"
#include "source/Model/Termin/nodeinfocontainer.h"
#include "source/Model/TerminGroup/groupinfocontainer.h"

// Задача этого класса - формировать запросы
// Clear static class
class SqlQueryConstructor
{
public:
    // Deprecation
    static QString selectQuery(const QString&        tableName,
                               const QStringList&    columns,
                               const WhereCondition& where,
                               const QString&        orderBy = "");

    // AppConfig table
    static QSqlQuery createAppConfigTable();
    static QSqlQuery insertConfigParameter(const QString& parameter, const QString& value);
    static QSqlQuery selectOneConfigParameter(const QString& parameter);
    static QSqlQuery updateConfigParameter(const QString& parameter, const QString& newValue);

    // Groups table
    static QSqlQuery createGroupsTable();
    static QSqlQuery insertGroup(const GroupInfoContainer& groupInfo);
    static QSqlQuery selectOneGroup(const QUuid& groupUuid);
    static QSqlQuery updateGroup(const GroupInfoContainer& groupInfo);
    static QSqlQuery deleteGroup(const QUuid& groupUuid);

    // Terms table
    static QSqlQuery createTermsTable();
    static QSqlQuery insertTerm(const NodeInfoContainer& termInfo);
    static QSqlQuery selectOneTerm(const QUuid& termUuid);
    static QSqlQuery selectOneTerm(const QString& term, const QUuid& groupUuid);
    static QSqlQuery updateTerm(const NodeInfoContainer& termInfo);
    static QSqlQuery deleteTerm(const QUuid& termUuid);

    // Common
    static QSqlQuery loadQuery(const QString& queryPath);
    static QSqlQuery recordsCount(const QString& tableName);
    static QSqlQuery dropTable(const QString& tableName);

private: // Methods
    static QString loadQueryString(const QString& queryPath);
};
