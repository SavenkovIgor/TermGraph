// Copyright © 2016-2025. Savenkov Igor
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
    virtual FutureExpected<GroupSummary>       group(const GroupUuid& uuid) const = 0;
    virtual FutureExpected<GroupSummary::List> groups() const                     = 0;

    virtual FutureExpected<GroupSummary> addGroup(const GroupSummary& info)    = 0;
    virtual FutureExpected<GroupSummary> updateGroup(const GroupSummary& info) = 0;
    virtual FutureExpected<GroupSummary> deleteGroup(const GroupUuid& uuid)    = 0;

    // Terms
    virtual FutureExpected<TermData>       term(const TermUuid& uuid) const                           = 0;
    virtual FutureExpected<TermData>       term(const QString& termName, const GroupUuid& uuid) const = 0;
    virtual FutureExpected<TermData::List> terms(const GroupUuid& uuid) const                         = 0;

    virtual FutureExpected<TermData> addTerm(const TermData& data) = 0;
    virtual FutureExpected<TermData> updateTerm(const TermData& data, LastEditSource lastEditSource, bool checkLastEdit)
        = 0;
    virtual FutureExpected<TermData> deleteTerm(const TermUuid& uuid) = 0;

    virtual ~DataStorageInterface() = default;
};
