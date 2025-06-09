// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QHostAddress>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "source/CommonTools/HandyTypes.h"

#include "source/TermDataInterface/DataStorageInterface.h"
#include "source/TermDataInterface/GroupSummary.h"
#include "source/TermDataInterface/TermData.h"

#include "NetWrapper.h"

class DataStorageConnection : public DataStorageInterface
{
public:
    DataStorageConnection(QUrl address);

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
    template<typename T>
    using Promise = QPromise<Expected<T>>;

    template<typename T>
    using SharedPromise = QSharedPointer<Promise<T>>;

    QUrl baseUrl;
    QUrl groupUrl;
    QUrl termUrl;

    NetWrapper network;
};
