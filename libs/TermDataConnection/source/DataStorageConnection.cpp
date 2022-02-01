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

#include "include/TermDataConnection/DataStorageConnection.h"

#include <functional>

#include <QSharedPointer>

#include <QDebug>

template<typename T>
void dataToPromise(QSharedPointer<QPromise<Result<T>>>           promise,
                   QNetworkReply*                                reply,
                   std::function<Opt<T>(const QByteArray& data)> parseFunc)
{
    if (reply->error() == QNetworkReply::NoError) {
        if (auto obj = parseFunc(reply->readAll())) {
            promise->addResult(*obj);
        } else {
            promise->addResult(DbErrorCodes::JsonParseError);
        }
    } else {
        auto err = reply->error();

        if (err == QNetworkReply::ConnectionRefusedError || err == QNetworkReply::HostNotFoundError
            || err == QNetworkReply::TimeoutError || err == QNetworkReply::ProtocolInvalidOperationError)
            promise->addResult(DbErrorCodes::ConnectionError);

        std::string errString = reply->readAll().toStdString();
        auto        dbError   = createDbError(errString);

        qDebug() << QString::fromStdString(errString);
        promise->addResult(dbError);
    }
}

template<typename T>
void dataToPromise(QSharedPointer<QPromise<Result<T>>>              promise,
                   QNetworkReply*                                   reply,
                   std::function<Result<T>(const QByteArray& data)> parseFunc)
{
    if (reply->error() == QNetworkReply::NoError) {
        if (auto result = parseFunc(reply->readAll())) {
            promise->addResult(result.value());
        } else {
            promise->addResult(result.error());
        }
    } else {
        promise->addResult(DbErrorCodes::ConnectionError);
    }
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

FutureRes<GroupUuid::List> DataStorageConnection::getAllGroupsUuids(bool sortByLastEdit) const
{
    SharedPromise<GroupUuid::List> promise(new Promise<GroupUuid::List>);
    promise->start();

    QUrl url = groupUrl;
    url.setQuery("type=uuid_only");

    invokeOnNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            dataToPromise<GroupUuid::List>(promise, reply, [](auto data) { return GroupUuid::List::create(data); });
            promise->finish();
        });
    });

    return promise->future();
}

FutureRes<GroupData> DataStorageConnection::getGroup(const GroupUuid& uuid) const
{
    SharedPromise<GroupData> promise(new Promise<GroupData>);
    promise->start();

    QUrl url = groupUrl;
    url.setPath(QString("%1/%2").arg(url.path()).arg(uuid.toString()));

    invokeOnNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            dataToPromise<GroupData>(promise, reply, [](auto data) { return GroupData::create(data); });
            promise->finish();
        });
    });

    return promise->future();
}

FutureRes<GroupData::List> DataStorageConnection::getGroups() const
{
    SharedPromise<GroupData::List> promise(new Promise<GroupData::List>);
    promise->start();

    QUrl url = groupUrl;

    invokeOnNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            dataToPromise<GroupData::List>(promise, reply, [](auto data) { return GroupData::List::create(data); });
            promise->finish();
        });
    });

    return promise->future();
}

FutureRes<GroupData> DataStorageConnection::addGroup(const GroupData& info)
{
    SharedPromise<GroupData> promise(new Promise<GroupData>);
    promise->start();

    QUrl url = groupUrl;

    invokeOnNetThread([this, promise, url, info]() {
        auto* reply = netThread.manager->post(QNetworkRequest(url), static_cast<QByteArray>(info));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            dataToPromise<GroupData>(promise, reply, [](auto data) { return GroupData::create(data); });
            promise->finish();
        });
    });

    return promise->future();
}

FutureRes<GroupData> DataStorageConnection::updateGroup(const GroupData& info)
{
    SharedPromise<GroupData> promise(new Promise<GroupData>);
    promise->start();

    QUrl url = groupUrl;
    url.setPath(QString("%1/%2").arg(url.path()).arg(info.uuid->toString()));

    invokeOnNetThread([this, promise, url, info]() {
        auto* reply = netThread.manager->put(QNetworkRequest(url), static_cast<QByteArray>(info));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            dataToPromise<GroupData>(promise, reply, [](auto data) { return GroupData::create(data); });
            promise->finish();
        });
    });

    return promise->future();
}

FutureRes<GroupData> DataStorageConnection::deleteGroup(const GroupUuid& uuid)
{
    SharedPromise<GroupData> promise(new Promise<GroupData>);
    promise->start();

    QUrl url = groupUrl;
    url.setPath(QString("%1/%2").arg(url.path()).arg(uuid.toString()));

    invokeOnNetThread([this, promise, url]() {
        auto* reply = netThread.manager->deleteResource(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            dataToPromise<GroupData>(promise, reply, [](auto data) { return GroupData::create(data); });
            promise->finish();
        });
    });

    return promise->future();
}

FutureRes<TermData> DataStorageConnection::getTerm(const TermUuid& uuid) const
{
    SharedPromise<TermData> promise(new Promise<TermData>);
    promise->start();

    QUrl url = termUrl;
    url.setPath(QString("%1/%2").arg(url.path()).arg(uuid.toString()));

    invokeOnNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            dataToPromise<TermData>(promise, reply, [](auto data) {
                return TermData::create(data, TermData::JsonCheckMode::Import);
            });
            promise->finish();
        });
    });

    return promise->future();
}

FutureRes<TermData> DataStorageConnection::getTerm(const QString& nodeName, const GroupUuid& uuid) const
{
    SharedPromise<TermData> promise(new Promise<TermData>);
    promise->start();

    QUrl url = termUrl;
    url.setQuery(QString("group_uuid=%1&name=%2").arg(uuid.toString()).arg(nodeName));

    invokeOnNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            dataToPromise<TermData>(promise, reply, [](auto data) {
                return TermData::create(data, TermData::JsonCheckMode::Import);
            });
            promise->finish();
        });
    });

    return promise->future();
}

FutureRes<TermData::List> DataStorageConnection::getTerms(const GroupUuid& uuid) const
{
    SharedPromise<TermData::List> promise(new Promise<TermData::List>);
    promise->start();

    QUrl url = termUrl;
    url.setQuery(QString("group_uuid=%1").arg(uuid.toString()));

    invokeOnNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            dataToPromise<TermData::List>(promise, reply, [](auto data) { return TermData::List::create(data); });
            promise->finish();
        });
    });

    return promise->future();
}

FutureRes<TermData> DataStorageConnection::addTerm(const TermData& info)
{
    SharedPromise<TermData> promise(new Promise<TermData>);
    promise->start();

    QUrl url = termUrl;

    invokeOnNetThread([this, promise, url, info]() {
        auto* reply = netThread.manager->post(QNetworkRequest(url), static_cast<QByteArray>(info));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            dataToPromise<TermData>(promise, reply, [](auto data) {
                return TermData::create(data, TermData::JsonCheckMode::Import);
            });
            promise->finish();
        });
    });

    return promise->future();
}

FutureRes<TermData> DataStorageConnection::updateTerm(const TermData&                      info,
                                                      DataStorageInterface::LastEditSource lastEditSource,
                                                      bool                                 checkLastEdit)
{
    SharedPromise<TermData> promise(new Promise<TermData>);
    promise->start();

    QUrl url = termUrl;
    url.setPath(QString("%1/%2").arg(url.path()).arg(info.uuid->toString()));

    invokeOnNetThread([this, promise, url, info]() {
        auto* reply = netThread.manager->put(QNetworkRequest(url), static_cast<QByteArray>(info));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            dataToPromise<TermData>(promise, reply, [](auto data) {
                return TermData::create(data, TermData::JsonCheckMode::Import);
            });
            promise->finish();
        });
    });

    return promise->future();
}

FutureRes<TermData> DataStorageConnection::deleteTerm(const TermUuid& uuid)
{
    SharedPromise<TermData> promise(new Promise<TermData>);
    promise->start();

    QUrl url = groupUrl;
    url.setPath(QString("%1/%2").arg(url.path()).arg(uuid.toString()));

    invokeOnNetThread([this, promise, url]() {
        auto* reply = netThread.manager->deleteResource(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            dataToPromise<TermData>(promise, reply, [](auto data) {
                return TermData::create(data, TermData::JsonCheckMode::Import);
            });
            promise->finish();
        });
    });

    return promise->future();
}

void DataStorageConnection::invokeOnNetThread(std::function<void()> netFunc) const
{
    QMetaObject::invokeMethod(netThread.manager.get(), netFunc);
}
