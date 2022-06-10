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

#include <QDateTime>
#include <QString>
#include <QUuid>

#include <CommonTools/GroupUuid.h>
#include <CommonTools/TermUuid.h>
#include <TermDataInterface/DataStorageErrors.h>
#include <TermDataInterface/GroupData.h>
#include <TermDataInterface/TermData.h>

// Interface
class DataStorageInterface
{
public:
    enum LastEditSource { TakeFromTermData = 0, AutoGenerate };

    using UuidList = std::vector<QUuid>;

    // Storage
    virtual int storageVersion() const = 0;

    // Groups
    virtual FutureResult<GroupUuid::List> getAllGroupsUuids(bool sortByLastEdit = false) const = 0;

    virtual FutureResult<GroupData>       getGroup(const GroupUuid& uuid) const   = 0;
    virtual FutureResult<GroupData::List> getGroups() const                       = 0;

    virtual FutureResult<GroupData> addGroup(const GroupData& info)    = 0;
    virtual FutureResult<GroupData> updateGroup(const GroupData& info) = 0;
    virtual FutureResult<GroupData> deleteGroup(const GroupUuid& uuid) = 0;

    // Terms
    virtual FutureResult<TermData>       getTerm(const TermUuid& uuid) const                            = 0;
    virtual FutureResult<TermData>       getTerm(const QString& termName, const GroupUuid& uuid) const  = 0;
    virtual FutureResult<TermData::List> getTerms(const GroupUuid& uuid) const                          = 0;

    virtual FutureResult<TermData> addTerm(const TermData& data) = 0;
    virtual FutureResult<TermData> updateTerm(const TermData& data,
                                           LastEditSource  lastEditSource,
                                           bool            checkLastEdit = true)
        = 0;
    virtual FutureResult<TermData> deleteTerm(const TermUuid& uuid) = 0;

    virtual ~DataStorageInterface() = default;
};
