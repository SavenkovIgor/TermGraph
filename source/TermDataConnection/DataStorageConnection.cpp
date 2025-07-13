// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/TermDataConnection/DataStorageConnection.h"

#include <functional>

#include <QObject>
#include <QSharedPointer>

import CommonTools.NetworkTools;

template<typename T>
Expected<T> toExpected(QNetworkReply* reply, std::function<std::optional<T>(const QByteArray& data)> parseFunc)
{
    if (reply->error() != QNetworkReply::NoError) {
        return std::unexpected(ErrorCode::ConnectionError);
    }

    if (auto obj = parseFunc(reply->readAll())) {
        return *obj;
    }

    return std::unexpected(ErrorCode::JsonParseError);
}

DataStorageConnection::DataStorageConnection(QUrl address)
    : DataStorageInterface()
{
    if (address.scheme() != "https") {
        qWarning("Not https connection!");
    }

    baseUrl = address;

    groupUrl = baseUrl;
    groupUrl.setPath(NetworkTools::groupApiPath);

    termUrl = baseUrl;
    termUrl.setPath(NetworkTools::termApiPath);
}

int DataStorageConnection::storageVersion() const
{
    Q_UNIMPLEMENTED();
    return -1;
}

FutureExpected<GroupSummary> DataStorageConnection::group(const GroupUuid& uuid) const
{
    SharedPromise<GroupSummary> promise(new Promise<GroupSummary>);
    promise->start();

    QUrl url = groupUrl;
    url.setPath(QString("%1/%2").arg(url.path(), uuid.toString()));

    network.get(url, [=](auto* reply) {
        promise->addResult(toExpected<GroupSummary>(reply, [](auto data) { return GroupSummary::from(data); }));
        promise->finish();
    });

    return promise->future();
}

FutureExpected<GroupSummary::List> DataStorageConnection::groups() const
{
    SharedPromise<GroupSummary::List> promise(new Promise<GroupSummary::List>);
    promise->start();

    QUrl url = groupUrl;

    network.get(url, [=](auto* reply) {
        promise->addResult(
            toExpected<GroupSummary::List>(reply, [](auto data) { return GroupSummary::List::from(data); }));
        promise->finish();
    });

    return promise->future();
}

FutureExpected<GroupSummary> DataStorageConnection::addGroup(const GroupSummary& info)
{
    SharedPromise<GroupSummary> promise(new Promise<GroupSummary>);
    promise->start();

    QUrl url = groupUrl;

    network.post(url, static_cast<QByteArray>(info), [=](auto* reply) {
        promise->addResult(toExpected<GroupSummary>(reply, [](auto data) { return GroupSummary::from(data); }));
        promise->finish();
    });

    return promise->future();
}

FutureExpected<GroupSummary> DataStorageConnection::updateGroup(const GroupSummary& info)
{
    SharedPromise<GroupSummary> promise(new Promise<GroupSummary>);
    promise->start();

    QUrl url = groupUrl;
    url.setPath(QString("%1/%2").arg(url.path(), info.uuid->toString()));

    network.put(url, static_cast<QByteArray>(info), [=](auto* reply) {
        promise->addResult(toExpected<GroupSummary>(reply, [](auto data) { return GroupSummary::from(data); }));
        promise->finish();
    });

    return promise->future();
}

FutureExpected<GroupSummary> DataStorageConnection::deleteGroup(const GroupUuid& uuid)
{
    SharedPromise<GroupSummary> promise(new Promise<GroupSummary>);
    promise->start();

    QUrl url = groupUrl;
    url.setPath(QString("%1/%2").arg(url.path(), uuid.toString()));

    network.deleteResource(url, [=](auto* reply) {
        promise->addResult(toExpected<GroupSummary>(reply, [](auto data) { return GroupSummary::from(data); }));
        promise->finish();
    });

    return promise->future();
}

FutureExpected<TermData> DataStorageConnection::term(const TermUuid& uuid) const
{
    SharedPromise<TermData> promise(new Promise<TermData>);
    promise->start();

    QUrl url = termUrl;
    url.setPath(QString("%1/%2").arg(url.path(), uuid.toString()));

    network.get(url, [=](auto* reply) {
        promise->addResult(toExpected<TermData>(reply, [](auto data) {
            return TermData::from(data, TermData::JsonCheckMode::Import);
        }));
        promise->finish();
    });

    return promise->future();
}

FutureExpected<TermData> DataStorageConnection::term(const QString& nodeName, const GroupUuid& uuid) const
{
    SharedPromise<TermData> promise(new Promise<TermData>);
    promise->start();

    QUrl url = termUrl;
    url.setQuery(QString("group_uuid=%1&name=%2").arg(uuid.toString(), nodeName));

    network.get(url, [=](auto* reply) {
        promise->addResult(toExpected<TermData>(reply, [](auto data) {
            return TermData::from(data, TermData::JsonCheckMode::Import);
        }));
        promise->finish();
    });

    return promise->future();
}

FutureExpected<TermData::List> DataStorageConnection::terms(const GroupUuid& uuid) const
{
    SharedPromise<TermData::List> promise(new Promise<TermData::List>);
    promise->start();

    QUrl url = termUrl;
    url.setQuery(QString("group_uuid=%1").arg(uuid.toString()));

    network.get(url, [=](auto* reply) {
        promise->addResult(toExpected<TermData::List>(reply, [](auto data) { return TermData::List::from(data); }));
        promise->finish();
    });

    return promise->future();
}

FutureExpected<TermData> DataStorageConnection::addTerm(const TermData& info)
{
    SharedPromise<TermData> promise(new Promise<TermData>);
    promise->start();

    QUrl url = termUrl;

    network.post(url, static_cast<QByteArray>(info), [=](auto* reply) {
        promise->addResult(toExpected<TermData>(reply, [](auto data) {
            return TermData::from(data, TermData::JsonCheckMode::Import);
        }));
        promise->finish();
    });

    return promise->future();
}

FutureExpected<TermData> DataStorageConnection::updateTerm(
    const TermData&                                       info,
    [[maybe_unused]] DataStorageInterface::LastEditSource lastEditSource,
    [[maybe_unused]] bool                                 checkLastEdit)
{
    SharedPromise<TermData> promise(new Promise<TermData>);
    promise->start();

    QUrl url = termUrl;
    url.setPath(QString("%1/%2").arg(url.path(), info.uuid->toString()));

    network.put(url, static_cast<QByteArray>(info), [=](auto* reply) {
        promise->addResult(toExpected<TermData>(reply, [](auto data) {
            return TermData::from(data, TermData::JsonCheckMode::Import);
        }));
        promise->finish();
    });

    return promise->future();
}

FutureExpected<TermData> DataStorageConnection::deleteTerm(const TermUuid& uuid)
{
    SharedPromise<TermData> promise(new Promise<TermData>);
    promise->start();

    QUrl url = termUrl;
    url.setPath(QString("%1/%2").arg(url.path(), uuid.toString()));

    network.deleteResource(url, [=](auto* reply) {
        promise->addResult(toExpected<TermData>(reply, [](auto data) {
            return TermData::from(data, TermData::JsonCheckMode::Import);
        }));
        promise->finish();
    });

    return promise->future();
}
