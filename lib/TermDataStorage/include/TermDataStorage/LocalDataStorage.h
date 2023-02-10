// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <functional>

#include <CommonTools/HandyTypes.h>

#include <TermDataInterface/DataStorageInterface.h>
#include <TermDataInterface/GroupSummary.h>
#include <TermDataInterface/TermData.h>

class StorageImpl;

class LocalDatabaseStorage : public DataStorageInterface
{
public:
    LocalDatabaseStorage(const QString& filePath, const QString& backupFolderPath);

public:
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
    StorageImpl* impl = nullptr;

    QMap<GroupUuid, QDateTime> nodesLastEdit() const;
};
