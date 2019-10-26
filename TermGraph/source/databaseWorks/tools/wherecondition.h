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

#include <QString>
#include <QStringList>
#include <QUuid>

#include "source/databaseWorks/columns/tcolumn.h"
#include "source/databaseWorks/tools/querytools.h"

enum class ConditionType { And, Or };

// TODO: Make chain calls!
class WhereCondition
{
public:
    WhereCondition(const ConditionType& type = ConditionType::And);

    // Build methods
    static WhereCondition columnEqual(const TColumn& column, const QString& value);
    static WhereCondition columnEqual(const TColumn& column, const int& value);

    void notEqual(const QString& column, const int& value);
    void notEqual(const QString& column, const QString& value);

    void equal(const TColumn& column, const int& value);
    void equal(const TColumn& column, const QString& value);

    void isNull(const QString& column);
    void isNotNull(const QString& column);

    QString getJoinedConditions() const;

private:
    QStringList conditions;
    ConditionType type;

    inline QString concat(const QString& str1, const QString& str2, const QString& str3)
    {
        return str1 + " " + str2 + " " + str3;
    }
};
