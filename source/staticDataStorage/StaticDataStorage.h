// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QFileInfoList>
#include <QMap>

#include "source/TermDataInterface/DataStorageInterface.h"
#include "source/TermDataInterface/GroupSummary.h"
#include "source/TermDataInterface/StaticGroupData.h"
#include "source/TermDataInterface/TermData.h"

class StaticDataStorage : public DataStorageInterface
{
public:
    StaticDataStorage();

    static QFileInfoList   files();
    static QByteArray      qrcFileData(const QString& filePath);
    static StaticGroupData prepareForInternalUse(StaticGroupData data);

    int storageVersion() const final;

    FutureExpected<GroupSummary>       group(const GroupUuid& uuid) const final;
    FutureExpected<GroupSummary::List> groups() const final;

    FutureExpected<GroupSummary> addGroup(const GroupSummary& info) final;
    FutureExpected<GroupSummary> updateGroup(const GroupSummary& info) final;
    FutureExpected<GroupSummary> deleteGroup(const GroupUuid& uuid) final;

    FutureExpected<TermData>       term(const TermUuid& uuid) const final;
    FutureExpected<TermData>       term(const QString& termName, const GroupUuid& uuid) const final;
    FutureExpected<TermData::List> terms(const GroupUuid& uuid) const final;

    FutureExpected<TermData> addTerm(const TermData& info) final;
    FutureExpected<TermData> updateTerm(const TermData& info, LastEditSource lastEditSource, bool checkLastEdit) final;
    FutureExpected<TermData> deleteTerm(const TermUuid& uuid) final;

private:
    constexpr static auto            dataFolderPath = ":/data/";
    QMap<GroupUuid, StaticGroupData> mGroups;

    QMap<GroupUuid, QDateTime> termsLastEdit() const;
};
