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

#include "source/Database/tools/wherecondition.h"

WhereCondition::WhereCondition(const ConditionType& type) { this->type = type; }

WhereCondition WhereCondition::columnEqual(const TColumn& column, const QString& value)
{
    WhereCondition where;
    where.equal(column, value);
    return where;
}

WhereCondition WhereCondition::columnEqual(const TColumn& column, const int& value)
{
    WhereCondition where;
    where.equal(column, value);
    return where;
}

void WhereCondition::notEqual(const QString& column, const int& value) { notEqual(column, QString::number(value)); }

void WhereCondition::notEqual(const QString& column, const QString& value)
{
    conditions << concat(column, "!=", QueryTools::vv(value));
}

void WhereCondition::equal(const TColumn& column, const int& value) { equal(column, QString::number(value)); }

void WhereCondition::equal(const TColumn& column, const QString& value)
{
    conditions << concat(column, "=", QueryTools::vv(value));
}

void WhereCondition::isNull(const QString& column) { conditions << column + " IS NULL"; }

void WhereCondition::isNotNull(const QString& column) { conditions << column + " IS NOT NULL"; }

QString WhereCondition::getJoinedConditions() const
{
    switch (type) {
    case ConditionType::And: return conditions.join(" AND ");
    case ConditionType::Or: return conditions.join(" OR ");
    }

    return conditions.join("");
}
