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

#include <QSqlDatabase>
#include <QSqlQuery>

class DbTools
{
public:
    // Transaction
    static void startTransaction(QSqlDatabase* base);
    static void endTransaction(QSqlDatabase* base);

    // Database
    static void reduceSpace(QSqlDatabase* base);

    // Query
    static QSqlQuery startQuery(QSqlDatabase* base, const QString& queryString);
    static QSqlQuery startQuery(QSqlQuery query);

    // Common table statistic
    static int recordsCount(const QString& tableName);
};
