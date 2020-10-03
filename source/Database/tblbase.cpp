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

#include "source/Database/tblbase.h"

#include "source/Database/tools/dbtools.h"

QSqlQuery TblBase::startQuery(const QString& queryString) const { return DbTools::startQuery(base, queryString); }

bool TblBase::hasErrors(const QString& errString) const
{
    bool ret;
    ret = (errString != "" && errString != " ");
    return ret;
}

TblBase::TblBase(QSqlDatabase* base)
    : base(base)
{}

bool TblBase::startQuery(QSqlQuery query) const { return DbTools::startQuery2(query); }

QSqlRecord TblBase::getRecord(QSqlQuery&& q)
{
    auto nextValid = q.next();
    assert(nextValid);

    return q.record();
}

RecVector TblBase::getAllRecords(QSqlQuery&& q)
{
    RecVector ret;

    if (auto size = q.size(); size > 0) {
        ret.reserve(size);
    }

    for (;;) { // BUG: multithreading & database works
        if (!q.next()) {
            break;
        }

        ret << q.record();
    }

    return ret;
}
