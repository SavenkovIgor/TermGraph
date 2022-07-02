// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QHostAddress>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <CommonTools/HandyTypes.h>
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
    template<typename T>
    using Promise = std::promise<Result<T>>;

    template<typename T>
    using SharedPromise = QSharedPointer<Promise<T>>;

    QUrl baseUrl;
    QUrl groupUrl;
    QUrl termUrl;

    NetworkThread netThread;

    void onNetThread(std::function<void()> netFunc) const;
};
