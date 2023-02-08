// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <memory>
#include <functional>

#include <QMap>

#include <CommonTools/HandyTypes.h>

#include <TermDataInterface/DataStorageInterface.h>
#include <TermDataInterface/GroupSummary.h>
#include <TermDataInterface/TermData.h>
#include <TermDataInterface/GroupData.h>


class StaticDataStorage : public DataStorageInterface
{
public:
    StaticDataStorage();

    int storageVersion() const final;

    FutureResult<GroupSummary>       group(const GroupUuid& uuid) const final;
    FutureResult<GroupSummary::List> groups() const final;

    FutureResult<GroupSummary> addGroup(const GroupSummary& info) final;
    FutureResult<GroupSummary> updateGroup(const GroupSummary& info) final;
    FutureResult<GroupSummary> deleteGroup(const GroupUuid& uuid) final;

    FutureResult<TermData>       term(const TermUuid& uuid) const final;
    FutureResult<TermData>       term(const QString& termName, const GroupUuid& uuid) const final;
    FutureResult<TermData::List> terms(const GroupUuid& uuid) const final;

    FutureResult<TermData> addTerm(const TermData& info) final;
    FutureResult<TermData> updateTerm(const TermData& info, LastEditSource lastEditSource, bool checkLastEdit) final;
    FutureResult<TermData> deleteTerm(const TermUuid& uuid) final;

private:
    constexpr static auto dataFolderPath = ":/data/";
    QMap<GroupUuid, GroupData> mGroups;

    QMap<GroupUuid, QDateTime> termsLastEdit() const;
};
