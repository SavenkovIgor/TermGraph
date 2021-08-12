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

#include <QDateTime>
#include <QString>
#include <QUuid>

#include <TermDataInterface/GroupData.h>
#include <TermDataInterface/TermData.h>

// Interface
class DataStorageInterface
{
public:
    enum LastEditSource { TakeFromTermData = 0, AutoGenerate };

    using UuidList = std::vector<QUuid>;

    // Storage
    virtual int storageVersion() const = 0;

    // Groups
    virtual UuidList getAllGroupsUuids(bool sortByLastEdit = false) const = 0;

    virtual bool            groupExist(const QUuid& groupUuid) const = 0;
    virtual GroupData       getGroup(const QUuid& groupUuid) const   = 0;
    virtual GroupData::List getGroups() const                        = 0;

    virtual bool addGroup(const GroupData& groupInfo) = 0;
    virtual bool updateGroup(const GroupData& info)   = 0;
    virtual void deleteGroup(const QUuid& groupUuid)  = 0;

    // Terms
    virtual UuidList getAllTermsUuids(const QUuid& groupUuid = QUuid()) const = 0;

    virtual bool           termExist(const QUuid& termUuid) const                          = 0;
    virtual QUuid          findTerm(const QString& termName, const QUuid& groupUuid) const = 0;
    virtual TermData       getTerm(const QUuid& termUuid) const                            = 0;
    virtual TermData::List getTerms(const QUuid& groupUuid) const                          = 0;
    virtual TermData::List getTerms(const UuidList& termsUuids) const                      = 0;
    virtual QDateTime      getTermLastEdit(const QUuid& termUuid) const                    = 0;

    virtual bool addTerm(const TermData& data)                                                              = 0;
    virtual bool updateTerm(const TermData& data, LastEditSource lastEditSource, bool checkLastEdit = true) = 0;
    virtual void deleteTerm(const QUuid& termUuid)                                                          = 0;
};
