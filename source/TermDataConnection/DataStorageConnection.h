// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QHostAddress>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "source/CommonTools/HandyTypes.h"
#include "source/CommonTools/NetworkTools.h"

#include <TermDataInterface/DataStorageInterface.h>
#include <TermDataInterface/GroupSummary.h>
#include <TermDataInterface/TermData.h>

#include "NetWrapper.h"

class DataStorageConnection : public DataStorageInterface
{
public:
    DataStorageConnection(QUrl address);

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
    template<typename T>
    using Promise = QPromise<Result<T>>;

    template<typename T>
    using SharedPromise = QSharedPointer<Promise<T>>;

    QUrl baseUrl;
    QUrl groupUrl;
    QUrl termUrl;

    NetWrapper network;
};
