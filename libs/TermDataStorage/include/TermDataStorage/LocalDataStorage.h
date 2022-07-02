// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <functional>

#include <CommonTools/HandyTypes.h>

#include <TermDataInterface/DataStorageInterface.h>
#include <TermDataInterface/GroupData.h>
#include <TermDataInterface/TermData.h>

class StorageImpl;

class LocalDatabaseStorage : public DataStorageInterface
{
public:
    LocalDatabaseStorage(const QString& filePath, const QString& backupFolderPath);

public:
    int storageVersion() const final;

    FutureResult<GroupUuid::List> getAllGroupsUuids(bool sortByLastEdit = false) const final;

    FutureResult<GroupData>       getGroup(const GroupUuid& uuid) const final;
    FutureResult<GroupData::List> getGroups() const final;

    FutureResult<GroupData> addGroup(const GroupData& info) final;
    FutureResult<GroupData> updateGroup(const GroupData& info) final;
    FutureResult<GroupData> deleteGroup(const GroupUuid& uuid) final;

    FutureResult<TermData>       getTerm(const TermUuid& uuid) const final;
    FutureResult<TermData>       getTerm(const QString& termName, const GroupUuid& uuid) const final;
    FutureResult<TermData::List> getTerms(const GroupUuid& uuid) const final;

    FutureResult<TermData> addTerm(const TermData& info) final;
    FutureResult<TermData> updateTerm(const TermData& info, LastEditSource lastEditSource, bool checkLastEdit) final;
    FutureResult<TermData> deleteTerm(const TermUuid& uuid) final;

private:
    StorageImpl* impl = nullptr;
};
