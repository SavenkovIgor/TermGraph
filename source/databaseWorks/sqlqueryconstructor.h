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

#include <QtCore>

#include "source/databaseWorks/columns/tcolumn.h"
#include "source/databaseWorks/tools/insertcontainer.h"
#include "source/databaseWorks/tools/querytools.h"
#include "source/databaseWorks/tools/setexpression.h"
#include "source/databaseWorks/tools/wherecondition.h"

// Задача этого класса - формировать запросы
// Clear static class
class SqlQueryConstructor
{
public:
    // Tables
    static QString createTable(const QString& tableName, const TColumn::List& columns);
    static QString addColumn(const QString& tableName, const TColumn& column);
    static QString dropTable(const QString& tableName);

    static QString recordsCount(const QString& tableName);

    // Data
    static QString selectQuery(const QString&        tableName,
                               const QStringList&    columns,
                               const WhereCondition& where,
                               const QString&        orderBy = "");
    static QString selectOneQuery(const QString& tableName, const WhereCondition& where);
    static QString insertQuery(const QString& tableName, const InsertContainer::List& values);

    static QString updateQuery(const QString& tableName, const SetExpression& set, const WhereCondition& where);

    static QString deleteWhereQuery(const QString& tableName, const WhereCondition& where);
};
