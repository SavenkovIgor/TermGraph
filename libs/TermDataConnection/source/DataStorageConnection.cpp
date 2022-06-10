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

FutureResult<GroupUuid::List> DataStorageConnection::getAllGroupsUuids(bool sortByLastEdit) const
{
    SharedPromise<GroupUuid::List> promise(new Promise<GroupUuid::List>);

    QUrl url = groupUrl;
    url.setQuery("type=uuid_only");

    onNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QObject::connect(reply, &QNetworkReply::finished ,[=] {
            promise->set_value(toResult<GroupUuid::List>(reply, [] (auto data) { return  GroupUuid::List::create(data); }));
            reply->deleteLater();
        });
    });

    return promise->get_future();
}

FutureResult<GroupData> DataStorageConnection::getGroup(const GroupUuid& uuid) const
{
    SharedPromise<GroupData> promise(new Promise<GroupData>);

    QUrl url = groupUrl;
    url.setPath(QString("%1/%2").arg(url.path(), uuid.toString()));

    onNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QObject::connect(reply, &QNetworkReply::finished, [=] {
            promise->set_value(toResult<GroupData>(reply, [](auto data) { return GroupData::create(data); }));
            reply->deleteLater();
        });
    });

    return promise->get_future();
}

FutureResult<GroupData::List> DataStorageConnection::getGroups() const
{
    SharedPromise<GroupData::List> promise(new Promise<GroupData::List>);

    QUrl url = groupUrl;

    onNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QObject::connect(reply, &QNetworkReply::finished, [=] {
            promise->set_value(toResult<GroupData::List>(reply, [](auto data) { return GroupData::List::create(data); }));
            reply->deleteLater();
        });
    });

    return promise->get_future();
}

FutureResult<GroupData> DataStorageConnection::addGroup(const GroupData& info)
{
    SharedPromise<GroupData> promise(new Promise<GroupData>);

    QUrl url = groupUrl;

    onNetThread([this, promise, url, info]() {
        auto* reply = netThread.manager->post(QNetworkRequest(url), static_cast<QByteArray>(info));

        QObject::connect(reply, &QNetworkReply::finished, [=] {
            promise->set_value(toResult<GroupData>(reply, [](auto data) { return GroupData::create(data); }));
            reply->deleteLater();
        });
    });

    return promise->get_future();
}

FutureResult<GroupData> DataStorageConnection::updateGroup(const GroupData& info)
{
    SharedPromise<GroupData> promise(new Promise<GroupData>);

    QUrl url = groupUrl;
    url.setPath(QString("%1/%2").arg(url.path(), info.uuid->toString()));

    onNetThread([this, promise, url, info]() {
        auto* reply = netThread.manager->put(QNetworkRequest(url), static_cast<QByteArray>(info));

        QObject::connect(reply, &QNetworkReply::finished, [=] {
            promise->set_value(toResult<GroupData>(reply, [](auto data) { return GroupData::create(data); }));
            reply->deleteLater();
        });
    });

    return promise->get_future();
}

FutureResult<GroupData> DataStorageConnection::deleteGroup(const GroupUuid& uuid)
{
    SharedPromise<GroupData> promise(new Promise<GroupData>);

    QUrl url = groupUrl;
    url.setPath(QString("%1/%2").arg(url.path(), uuid.toString()));

    onNetThread([this, promise, url]() {
        auto* reply = netThread.manager->deleteResource(QNetworkRequest(url));

        QObject::connect(reply, &QNetworkReply::finished, [=] {
            promise->set_value(toResult<GroupData>(reply, [](auto data) { return GroupData::create(data); }));
            reply->deleteLater();
        });
    });

    return promise->get_future();
}

FutureResult<TermData> DataStorageConnection::getTerm(const TermUuid& uuid) const
{
    SharedPromise<TermData> promise(new Promise<TermData>);

    QUrl url = termUrl;
    url.setPath(QString("%1/%2").arg(url.path(), uuid.toString()));

    onNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QObject::connect(reply, &QNetworkReply::finished, [=] {
            promise->set_value(toResult<TermData>(reply, [](auto data) {
                return TermData::create(data, TermData::JsonCheckMode::Import);
            }));
            reply->deleteLater();
        });
    });

    return promise->get_future();
}

FutureResult<TermData> DataStorageConnection::getTerm(const QString& nodeName, const GroupUuid& uuid) const
{
    SharedPromise<TermData> promise(new Promise<TermData>);

    QUrl url = termUrl;
    url.setQuery(QString("group_uuid=%1&name=%2").arg(uuid.toString(), nodeName));

    onNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QObject::connect(reply, &QNetworkReply::finished, [=] {
            promise->set_value(toResult<TermData>(reply, [](auto data) {
                return TermData::create(data, TermData::JsonCheckMode::Import);
            }));
            reply->deleteLater();
        });
    });

    return promise->get_future();
}

FutureResult<TermData::List> DataStorageConnection::getTerms(const GroupUuid& uuid) const
{
    SharedPromise<TermData::List> promise(new Promise<TermData::List>);

    QUrl url = termUrl;
    url.setQuery(QString("group_uuid=%1").arg(uuid.toString()));

    onNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QObject::connect(reply, &QNetworkReply::finished, [=] {
            promise->set_value(toResult<TermData::List>(reply, [](auto data) { return TermData::List::create(data); }));
            reply->deleteLater();
        });
    });

    return promise->get_future();
}

FutureResult<TermData> DataStorageConnection::addTerm(const TermData& info)
{
    SharedPromise<TermData> promise(new Promise<TermData>);

    QUrl url = termUrl;

    onNetThread([this, promise, url, info]() {
        auto* reply = netThread.manager->post(QNetworkRequest(url), static_cast<QByteArray>(info));

        QObject::connect(reply, &QNetworkReply::finished, [=] {
            promise->set_value(toResult<TermData>(reply, [](auto data) {
                return TermData::create(data, TermData::JsonCheckMode::Import);
            }));
            reply->deleteLater();
        });
    });

    return promise->get_future();
}

FutureResult<TermData> DataStorageConnection::updateTerm(const TermData&                      info,
                                                      DataStorageInterface::LastEditSource lastEditSource,
                                                      bool                                 checkLastEdit)
{
    SharedPromise<TermData> promise(new Promise<TermData>);

    QUrl url = termUrl;
    url.setPath(QString("%1/%2").arg(url.path(), info.uuid->toString()));

    onNetThread([this, promise, url, info]() {
        auto* reply = netThread.manager->put(QNetworkRequest(url), static_cast<QByteArray>(info));

        QObject::connect(reply, &QNetworkReply::finished, [=] {
            promise->set_value(toResult<TermData>(reply, [](auto data) {
                return TermData::create(data, TermData::JsonCheckMode::Import);
            }));
            reply->deleteLater();
        });
    });

    return promise->get_future();
}

FutureResult<TermData> DataStorageConnection::deleteTerm(const TermUuid& uuid)
{
    SharedPromise<TermData> promise(new Promise<TermData>);

    QUrl url = termUrl;
    url.setPath(QString("%1/%2").arg(url.path(), uuid.toString()));

    onNetThread([this, promise, url]() {
        auto* reply = netThread.manager->deleteResource(QNetworkRequest(url));

        QObject::connect(reply, &QNetworkReply::finished, [=] {
            promise->set_value(toResult<TermData>(reply, [](auto data) {
                return TermData::create(data, TermData::JsonCheckMode::Import);
            }));
            reply->deleteLater();
        });
    });

    return promise->get_future();
}

void DataStorageConnection::onNetThread(std::function<void()> netFunc) const
{
    QMetaObject::invokeMethod(netThread.manager.get(), netFunc);
}
