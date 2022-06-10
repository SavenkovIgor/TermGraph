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
    SSharedPromise<GroupUuid::List> promise(new SPromise<GroupUuid::List>);

    QUrl url = groupUrl;
    url.setQuery("type=uuid_only");

    invokeOnNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            promise->set_value(toResult<GroupUuid::List>(reply, [] (auto data) { return  GroupUuid::List::create(data); }));
        });
    });

    return promise->get_future();
}

FutureResult<GroupData> DataStorageConnection::getGroup(const GroupUuid& uuid) const
{
    SSharedPromise<GroupData> promise(new SPromise<GroupData>);

    QUrl url = groupUrl;
    url.setPath(QString("%1/%2").arg(url.path(), uuid.toString()));

    invokeOnNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            promise->set_value(toResult<GroupData>(reply, [](auto data) { return GroupData::create(data); }));
        });
    });

    return promise->get_future();
}

FutureResult<GroupData::List> DataStorageConnection::getGroups() const
{
    SSharedPromise<GroupData::List> promise(new SPromise<GroupData::List>);

    QUrl url = groupUrl;

    invokeOnNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            promise->set_value(toResult<GroupData::List>(reply, [](auto data) { return GroupData::List::create(data); }));
        });
    });

    return promise->get_future();
}

FutureResult<GroupData> DataStorageConnection::addGroup(const GroupData& info)
{
    SSharedPromise<GroupData> promise(new SPromise<GroupData>);

    QUrl url = groupUrl;

    invokeOnNetThread([this, promise, url, info]() {
        auto* reply = netThread.manager->post(QNetworkRequest(url), static_cast<QByteArray>(info));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            promise->set_value(toResult<GroupData>(reply, [](auto data) { return GroupData::create(data); }));
        });
    });

    return promise->get_future();
}

FutureResult<GroupData> DataStorageConnection::updateGroup(const GroupData& info)
{
    SSharedPromise<GroupData> promise(new SPromise<GroupData>);

    QUrl url = groupUrl;
    url.setPath(QString("%1/%2").arg(url.path(), info.uuid->toString()));

    invokeOnNetThread([this, promise, url, info]() {
        auto* reply = netThread.manager->put(QNetworkRequest(url), static_cast<QByteArray>(info));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            promise->set_value(toResult<GroupData>(reply, [](auto data) { return GroupData::create(data); }));
        });
    });

    return promise->get_future();
}

FutureResult<GroupData> DataStorageConnection::deleteGroup(const GroupUuid& uuid)
{
    SSharedPromise<GroupData> promise(new SPromise<GroupData>);

    QUrl url = groupUrl;
    url.setPath(QString("%1/%2").arg(url.path(), uuid.toString()));

    invokeOnNetThread([this, promise, url]() {
        auto* reply = netThread.manager->deleteResource(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            promise->set_value(toResult<GroupData>(reply, [](auto data) { return GroupData::create(data); }));
        });
    });

    return promise->get_future();
}

FutureResult<TermData> DataStorageConnection::getTerm(const TermUuid& uuid) const
{
    SSharedPromise<TermData> promise(new SPromise<TermData>);

    QUrl url = termUrl;
    url.setPath(QString("%1/%2").arg(url.path(), uuid.toString()));

    invokeOnNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            promise->set_value(toResult<TermData>(reply, [](auto data) {
                return TermData::create(data, TermData::JsonCheckMode::Import);
            }));
        });
    });

    return promise->get_future();
}

FutureResult<TermData> DataStorageConnection::getTerm(const QString& nodeName, const GroupUuid& uuid) const
{
    SSharedPromise<TermData> promise(new SPromise<TermData>);

    QUrl url = termUrl;
    url.setQuery(QString("group_uuid=%1&name=%2").arg(uuid.toString(), nodeName));

    invokeOnNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            promise->set_value(toResult<TermData>(reply, [](auto data) {
                return TermData::create(data, TermData::JsonCheckMode::Import);
            }));
        });
    });

    return promise->get_future();
}

FutureResult<TermData::List> DataStorageConnection::getTerms(const GroupUuid& uuid) const
{
    SSharedPromise<TermData::List> promise(new SPromise<TermData::List>);

    QUrl url = termUrl;
    url.setQuery(QString("group_uuid=%1").arg(uuid.toString()));

    invokeOnNetThread([this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            promise->set_value(toResult<TermData::List>(reply, [](auto data) { return TermData::List::create(data); }));
        });
    });

    return promise->get_future();
}

FutureResult<TermData> DataStorageConnection::addTerm(const TermData& info)
{
    SSharedPromise<TermData> promise(new SPromise<TermData>);

    QUrl url = termUrl;

    invokeOnNetThread([this, promise, url, info]() {
        auto* reply = netThread.manager->post(QNetworkRequest(url), static_cast<QByteArray>(info));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            promise->set_value(toResult<TermData>(reply, [](auto data) {
                return TermData::create(data, TermData::JsonCheckMode::Import);
            }));
        });
    });

    return promise->get_future();
}

FutureResult<TermData> DataStorageConnection::updateTerm(const TermData&                      info,
                                                      DataStorageInterface::LastEditSource lastEditSource,
                                                      bool                                 checkLastEdit)
{
    SSharedPromise<TermData> promise(new SPromise<TermData>);

    QUrl url = termUrl;
    url.setPath(QString("%1/%2").arg(url.path(), info.uuid->toString()));

    invokeOnNetThread([this, promise, url, info]() {
        auto* reply = netThread.manager->put(QNetworkRequest(url), static_cast<QByteArray>(info));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            promise->set_value(toResult<TermData>(reply, [](auto data) {
                return TermData::create(data, TermData::JsonCheckMode::Import);
            }));
        });
    });

    return promise->get_future();
}

FutureResult<TermData> DataStorageConnection::deleteTerm(const TermUuid& uuid)
{
    SSharedPromise<TermData> promise(new SPromise<TermData>);

    QUrl url = termUrl;
    url.setPath(QString("%1/%2").arg(url.path(), uuid.toString()));

    invokeOnNetThread([this, promise, url]() {
        auto* reply = netThread.manager->deleteResource(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            promise->set_value(toResult<TermData>(reply, [](auto data) {
                return TermData::create(data, TermData::JsonCheckMode::Import);
            }));
        });
    });

    return promise->get_future();
}

void DataStorageConnection::invokeOnNetThread(std::function<void()> netFunc) const
{
    QMetaObject::invokeMethod(netThread.manager.get(), netFunc);
}
