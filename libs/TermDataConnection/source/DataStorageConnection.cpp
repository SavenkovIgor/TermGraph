// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "include/TermDataConnection/DataStorageConnection.h"

#include <functional>

#include <QObject>
#include <QSharedPointer>


template<typename T>
Result<T> toResult(QNetworkReply*                                reply,
                   std::function<Opt<T>(const QByteArray& data)> parseFunc)
{
    if (reply->error() != QNetworkReply::NoError)
        return DbErrorCodes::ConnectionError;

    if (auto obj = parseFunc(reply->readAll()))
        return *obj;

    return DbErrorCodes::JsonParseError;
}

DataStorageConnection::DataStorageConnection(QHostAddress address, quint16 port)
    : DataStorageInterface()
{
    // TODO: Switch to https
    baseUrl.setScheme("http");
    baseUrl.setHost(address.toString());
    baseUrl.setPort(port);

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

FutureResult<GroupUuid::List> DataStorageConnection::getAllGroupsUuids() const
{
    SharedPromise<GroupUuid::List> promise(new Promise<GroupUuid::List>);
    promise->start();

    QUrl url = groupUrl;
    url.setQuery("type=uuid_only");

    netThread.get(url, [=](auto* reply) {
        promise->addResult(toResult<GroupUuid::List>(reply, [] (auto data) { return  GroupUuid::List::create(data); }));
        promise->finish();
    });

    return promise->future();
}

FutureResult<GroupData> DataStorageConnection::getGroup(const GroupUuid& uuid) const
{
    SharedPromise<GroupData> promise(new Promise<GroupData>);
    promise->start();

    QUrl url = groupUrl;
    url.setPath(QString("%1/%2").arg(url.path(), uuid.toString()));

    netThread.get(url, [=](auto* reply) {
        promise->addResult(toResult<GroupData>(reply, [](auto data) { return GroupData::create(data); }));
        promise->finish();
    });

    return promise->future();
}

FutureResult<GroupData::List> DataStorageConnection::getGroups() const
{
    SharedPromise<GroupData::List> promise(new Promise<GroupData::List>);
    promise->start();

    QUrl url = groupUrl;

    netThread.get(url, [=](auto* reply) {
        promise->addResult(toResult<GroupData::List>(reply, [](auto data) { return GroupData::List::create(data); }));
        promise->finish();
    });

    return promise->future();
}

FutureResult<GroupData> DataStorageConnection::addGroup(const GroupData& info)
{
    SharedPromise<GroupData> promise(new Promise<GroupData>);
    promise->start();

    QUrl url = groupUrl;

    netThread.post(url, static_cast<QByteArray>(info), [=](auto* reply) {
        promise->addResult(toResult<GroupData>(reply, [](auto data) { return GroupData::create(data); }));
        promise->finish();
    });

    return promise->future();
}

FutureResult<GroupData> DataStorageConnection::updateGroup(const GroupData& info)
{
    SharedPromise<GroupData> promise(new Promise<GroupData>);
    promise->start();

    QUrl url = groupUrl;
    url.setPath(QString("%1/%2").arg(url.path(), info.uuid->toString()));

    netThread.put(url, static_cast<QByteArray>(info), [=](auto* reply) {
        promise->addResult(toResult<GroupData>(reply, [](auto data) { return GroupData::create(data); }));
        promise->finish();
    });

    return promise->future();
}

FutureResult<GroupData> DataStorageConnection::deleteGroup(const GroupUuid& uuid)
{
    SharedPromise<GroupData> promise(new Promise<GroupData>);
    promise->start();

    QUrl url = groupUrl;
    url.setPath(QString("%1/%2").arg(url.path(), uuid.toString()));

    netThread.deleteResource(url, [=](auto* reply) {
        promise->addResult(toResult<GroupData>(reply, [](auto data) { return GroupData::create(data); }));
        promise->finish();
    });

    return promise->future();
}

FutureResult<TermData> DataStorageConnection::getTerm(const TermUuid& uuid) const
{
    SharedPromise<TermData> promise(new Promise<TermData>);
    promise->start();

    QUrl url = termUrl;
    url.setPath(QString("%1/%2").arg(url.path(), uuid.toString()));

    netThread.get(url, [=](auto* reply) {
        promise->addResult(toResult<TermData>(reply, [](auto data) {
            return TermData::create(data, TermData::JsonCheckMode::Import);
        }));
        promise->finish();
    });

    return promise->future();
}

FutureResult<TermData> DataStorageConnection::getTerm(const QString& nodeName, const GroupUuid& uuid) const
{
    SharedPromise<TermData> promise(new Promise<TermData>);
    promise->start();

    QUrl url = termUrl;
    url.setQuery(QString("group_uuid=%1&name=%2").arg(uuid.toString(), nodeName));

    netThread.get(url, [=](auto* reply){
        promise->addResult(toResult<TermData>(reply, [](auto data) {
            return TermData::create(data, TermData::JsonCheckMode::Import);
        }));
        promise->finish();
    });

    return promise->future();
}

FutureResult<TermData::List> DataStorageConnection::getTerms(const GroupUuid& uuid) const
{
    SharedPromise<TermData::List> promise(new Promise<TermData::List>);
    promise->start();

    QUrl url = termUrl;
    url.setQuery(QString("group_uuid=%1").arg(uuid.toString()));

    netThread.get(url, [=](auto* reply){
        promise->addResult(toResult<TermData::List>(reply, [](auto data) { return TermData::List::create(data); }));
        promise->finish();
    });

    return promise->future();
}

FutureResult<TermData> DataStorageConnection::addTerm(const TermData& info)
{
    SharedPromise<TermData> promise(new Promise<TermData>);
    promise->start();

    QUrl url = termUrl;

    netThread.post(url, static_cast<QByteArray>(info), [=](auto* reply) {
        promise->addResult(toResult<TermData>(reply, [](auto data) {
            return TermData::create(data, TermData::JsonCheckMode::Import);
        }));
        promise->finish();
    });

    return promise->future();
}

FutureResult<TermData> DataStorageConnection::updateTerm(const TermData&                      info,
                                                      DataStorageInterface::LastEditSource lastEditSource,
                                                      bool                                 checkLastEdit)
{
    SharedPromise<TermData> promise(new Promise<TermData>);
    promise->start();

    QUrl url = termUrl;
    url.setPath(QString("%1/%2").arg(url.path(), info.uuid->toString()));

    netThread.put(url, static_cast<QByteArray>(info), [=](auto* reply) {
        promise->addResult(toResult<TermData>(reply, [](auto data) {
            return TermData::create(data, TermData::JsonCheckMode::Import);
        }));
        promise->finish();
    });

    return promise->future();
}

FutureResult<TermData> DataStorageConnection::deleteTerm(const TermUuid& uuid)
{
    SharedPromise<TermData> promise(new Promise<TermData>);
    promise->start();

    QUrl url = termUrl;
    url.setPath(QString("%1/%2").arg(url.path(), uuid.toString()));

    netThread.deleteResource(url, [=](auto* reply) {
        promise->addResult(toResult<TermData>(reply, [](auto data) {
            return TermData::create(data, TermData::JsonCheckMode::Import);
        }));
        promise->finish();
    });

    return promise->future();
}
