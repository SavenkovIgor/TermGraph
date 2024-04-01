// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <functional>
#include <memory>

#include <QFileInfoList>
#include <QMap>

#include "source/CommonTools/HandyTypes.h"

#include <source/TermDataInterface/DataStorageInterface.h>
#include <source/TermDataInterface/GroupSummary.h>
#include <source/TermDataInterface/StaticGroupData.h>
#include <source/TermDataInterface/TermData.h>

class StaticDataStorage : public DataStorageInterface
{
public:
    StaticDataStorage();

    static QFileInfoList   files();
    static QByteArray      qrcFileData(const QString& filePath);
    static StaticGroupData prepareForInternalUse(StaticGroupData data);

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
    constexpr static auto            dataFolderPath = ":/data/";
    QMap<GroupUuid, StaticGroupData> mGroups;

    QMap<GroupUuid, QDateTime> termsLastEdit() const;
};
