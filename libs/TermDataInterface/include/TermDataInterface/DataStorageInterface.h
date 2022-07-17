// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

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
