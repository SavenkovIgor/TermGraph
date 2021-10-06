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

#include <CommonTools/HandyTypes.h>

#include <TermDataInterface/DataStorageInterface.h>
#include <TermDataInterface/GroupData.h>
#include <TermDataInterface/TermData.h>

class StorageImpl;

class LocalDatabaseStorage : public DataStorageInterface
{
public:
    LocalDatabaseStorage(const QString& filePath, const QString& backupFolderPath);

    // DataStorageInterface interface
public:
    int storageVersion() const final;

    QUuid getFreeUuid() const final;

    GroupUuid::List getAllGroupsUuids(bool sortByLastEdit) const final;

    // Add getFreeUuid for groups
    bool              groupExist(const GroupUuid& uuid) const final;
    result<GroupData> getGroup(const GroupUuid& uuid) const final;
    GroupData::List   getGroups() const final;

    result<void> addGroup(const GroupData& info) final;
    result<void> updateGroup(const GroupData& info) final;
    result<void> deleteGroup(const GroupUuid& uuid) final;

    TermUuid::List getAllTermsUuids(opt<GroupUuid> groupUuid = std::nullopt) const final;

    // Add getFreeUuid for nodes
    bool                   termExist(const TermUuid& uuid) const final;
    opt<TermUuid>          findTerm(const QString& termName, const GroupUuid& uuid) const final;
    result<TermData>       getTerm(const TermUuid& uuid) const final;
    result<TermData::List> getTerms(const GroupUuid& uuid) const final;
    result<TermData::List> getTerms(const UuidList& termsUuids) const final;
    result<QDateTime>      getTermLastEdit(const TermUuid& uuid) const final;

    result<void> addTerm(const TermData& info) final;
    result<void> updateTerm(const TermData& info, LastEditSource lastEditSource, bool checkLastEdit) final;
    result<void> deleteTerm(const TermUuid& uuid) final;

private:
    StorageImpl* impl = nullptr;
};
