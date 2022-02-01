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

#include <QHostAddress>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <CommonTools/HandyTypes.h>
#include <CommonTools/JsonTools.h>
#include <CommonTools/NetworkTools.h>

#include <TermDataInterface/DataStorageInterface.h>
#include <TermDataInterface/GroupData.h>
#include <TermDataInterface/TermData.h>

#include "NetworkThread.h"

class DataStorageConnection : public DataStorageInterface
{
public:
    DataStorageConnection(QHostAddress address, quint16 port = NetworkTools::defaultPort);

    int storageVersion() const final;

    FutureRes<GroupUuid::List> getAllGroupsUuids(bool sortByLastEdit = false) const final;

    FutureRes<GroupData>       getGroup(const GroupUuid& uuid) const final;
    FutureRes<GroupData::List> getGroups() const final;

    FutureRes<GroupData> addGroup(const GroupData& info) final;
    FutureRes<GroupData> updateGroup(const GroupData& info) final;
    FutureRes<GroupData> deleteGroup(const GroupUuid& uuid) final;

    FutureRes<TermData>       getTerm(const TermUuid& uuid) const final;
    FutureRes<TermData>       getTerm(const QString& termName, const GroupUuid& uuid) const final;
    FutureRes<TermData::List> getTerms(const GroupUuid& uuid) const final;

    FutureRes<TermData> addTerm(const TermData& info) final;
    FutureRes<TermData> updateTerm(const TermData& info, LastEditSource lastEditSource, bool checkLastEdit) final;
    FutureRes<TermData> deleteTerm(const TermUuid& uuid) final;

private:
    QUrl baseUrl;
    QUrl groupUrl;
    QUrl termUrl;

    NetworkThread netThread;
};
