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
#include "source/Database/tools/insertcontainer.h"
#include "source/Database/tools/querytools.h"
#include "source/Database/tools/setexpression.h"
#include "source/Database/tools/wherecondition.h"

// Задача этого класса - формировать запросы
// Clear static class
class SqlQueryConstructor
{
public:
    // Tables
    static QString   createTable(const QString& tableName, const TColumn::List& columns);
    static QString   addColumn(const QString& tableName, const TColumn& column);
    static QSqlQuery dropTable(const QString& tableName);

    static QString recordsCount(const QString& tableName);

    // Data
    static QString selectQuery(const QString&        tableName,
                               const QStringList&    columns,
                               const WhereCondition& where,
                               const QString&        orderBy = "");
    static QString selectOneQuery(const QString& tableName, const WhereCondition& where);
    static QString insertQuery(const QString& tableName, const InsertContainer::List& values);

    static QString updateQuery(const QString& tableName, const SetExpression& set, const WhereCondition& where);

    static QSqlQuery deleteGroup(const QUuid& groupUuid);
    static QSqlQuery deleteTerm(const QUuid& termUuid);

private: // Methods
    static QString   loadQueryString(const QString& queryPath);
    static QSqlQuery loadQuery(const QString& queryPath);
};
