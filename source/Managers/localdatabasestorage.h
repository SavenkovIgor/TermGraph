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

#include "source/Database/database.h"
#include "source/Helpers/appconfig.h"
#include "source/Managers/datastorageinterface.h"
#include "source/Model/Termin/termdata.h"
#include "source/Model/TerminGroup/groupdata.h"

class LocalDatabaseStorage : public DataStorageInterface
{
public:
    LocalDatabaseStorage(const QString& filePath = AppSettings::StdPaths::defaultDatabaseFilePath());

    // DataStorageInterface interface
public:
    int storageVersion() const override;

    UuidList getAllGroupsUuids(bool sortByLastEdit) const final;

    // Add getFreeUuid for groups
    bool            groupExist(const QUuid& groupUuid) const final;
    GroupData       getGroup(const QUuid& groupUuid) const final;
    GroupData::List getGroups() const final;

    bool addGroup(const GroupData& groupInfo) final;
    bool updateGroup(const GroupData& info) final;
    void deleteGroup(const QUuid& groupUuid) final;

    UuidList getAllTermsUuids(const QUuid& groupUuid) const final;

    // Add getFreeUuid for nodes
    bool           termExist(const QUuid& termUuid) const final;
    QUuid          findTerm(const QString& termName, const QUuid& groupUuid) const final;
    TermData       getTerm(const QUuid& termUuid) const final;
    TermData::List getTerms(const QUuid& groupUuid) const final;
    TermData::List getTerms(const UuidList& termsUuids) const final;
    QDateTime      getTermLastEdit(const QUuid& termUuid) const final;

    bool addTerm(const TermData& info) final;
    bool updateTerm(const TermData& info, LastEditSource lastEditSource, bool checkLastEdit) final;
    void deleteTerm(const QUuid& termUuid) final;

private:
    Database db;
};
