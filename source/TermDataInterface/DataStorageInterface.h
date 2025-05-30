// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QDateTime>
#include <QString>
#include <QUuid>

#include <PlatformTypes.h>

#include "source/CommonTools/GroupUuid.h"
#include "source/CommonTools/TermUuid.h"
#include "source/TermDataInterface/GroupSummary.h"
#include "source/TermDataInterface/TermData.h"

// Interface
class DataStorageInterface
{
public:
    enum LastEditSource { FromData = 0, Now };

    // Storage
    virtual int storageVersion() const = 0;

    // Groups
    virtual FutureResult<GroupSummary>       group(const GroupUuid& uuid) const = 0;
    virtual FutureResult<GroupSummary::List> groups() const                     = 0;

    virtual FutureResult<GroupSummary> addGroup(const GroupSummary& info)    = 0;
    virtual FutureResult<GroupSummary> updateGroup(const GroupSummary& info) = 0;
    virtual FutureResult<GroupSummary> deleteGroup(const GroupUuid& uuid)    = 0;

    // Terms
    virtual FutureResult<TermData>       term(const TermUuid& uuid) const                           = 0;
    virtual FutureResult<TermData>       term(const QString& termName, const GroupUuid& uuid) const = 0;
    virtual FutureResult<TermData::List> terms(const GroupUuid& uuid) const                         = 0;

    virtual FutureResult<TermData> addTerm(const TermData& data) = 0;
    virtual FutureResult<TermData> updateTerm(const TermData& data, LastEditSource lastEditSource, bool checkLastEdit)
        = 0;
    virtual FutureResult<TermData> deleteTerm(const TermUuid& uuid) = 0;

    virtual ~DataStorageInterface() = default;
};
