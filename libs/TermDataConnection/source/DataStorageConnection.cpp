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

#include <QSharedPointer>

#include <QDebug>

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
    QSharedPointer<QPromise<Result<GroupUuid::List>>> promise(new QPromise<Result<GroupUuid::List>>);
    promise->start();

    QUrl url = groupUrl;
    url.setQuery("type=uuid_only");

    QMetaObject::invokeMethod(netThread.manager.get(), [this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            if (reply->error() == QNetworkReply::NoError) {
                if (auto list = GroupUuid::List::create(reply->readAll())) {
                    promise->addResult(*list);
                } else {
                    promise->addResult(DbErrorCodes::JsonParseError);
                }
            } else {
                promise->addResult(DbErrorCodes::ConnectionError);
            }

            promise->finish();
        });
    });

    return promise->future();
}

FutureRes<GroupData> DataStorageConnection::getGroup(const GroupUuid& uuid) const
{
    QSharedPointer<QPromise<Result<GroupData>>> promise(new QPromise<Result<GroupData>>);
    promise->start();

    QUrl url = groupUrl;
    url.setPath(QString("%1/%2").arg(url.path()).arg(uuid.toString()));

    QMetaObject::invokeMethod(netThread.manager.get(), [this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            if (reply->error() == QNetworkReply::NoError) {
                if (auto group = GroupData::create(reply->readAll())) {
                    promise->addResult(*group);
                } else {
                    promise->addResult(DbErrorCodes::JsonParseError);
                }
            } else {
                promise->addResult(DbErrorCodes::ConnectionError);
            }

            promise->finish();
        });
    });

    return promise->future();
}

FutureRes<GroupData::List> DataStorageConnection::getGroups() const
{
    QSharedPointer<QPromise<Result<GroupData::List>>> promise(new QPromise<Result<GroupData::List>>);
    promise->start();

    QUrl url = groupUrl;

    QMetaObject::invokeMethod(netThread.manager.get(), [this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            if (reply->error() == QNetworkReply::NoError) {
                if (auto groups = GroupData::List::create(reply->readAll())) {
                    promise->addResult(*groups);
                } else {
                    promise->addResult(DbErrorCodes::JsonParseError);
                }
            } else {
                promise->addResult(DbErrorCodes::ConnectionError);
            }

            promise->finish();
        });
    });

    return promise->future();
}

Result<void> DataStorageConnection::addGroup(const GroupData& info)
{
    Q_UNIMPLEMENTED();
    return outcome::success();
}

Result<void> DataStorageConnection::updateGroup(const GroupData& info)
{
    Q_UNIMPLEMENTED();
    return outcome::success();
}

Result<void> DataStorageConnection::deleteGroup(const GroupUuid& uuid)
{
    Q_UNIMPLEMENTED();
    return outcome::success();
}

TermUuid::List DataStorageConnection::getAllTermsUuids(Opt<GroupUuid> uuid) const
{
    Q_UNIMPLEMENTED();
    return {};
}

Opt<TermUuid> DataStorageConnection::findTerm(const QString& nodeName, const GroupUuid& uuid) const
{
    Q_UNIMPLEMENTED();
    return std::nullopt;
}

Result<TermData> DataStorageConnection::getTerm(const TermUuid& uuid) const
{
    Q_UNIMPLEMENTED();
    //    return TermData();
}

FutureRes<TermData::List> DataStorageConnection::getTerms(const GroupUuid& uuid) const
{
    QSharedPointer<QPromise<Result<TermData::List>>> promise(new QPromise<Result<TermData::List>>);
    promise->start();

    QUrl url = termUrl;
    url.setQuery(QString("group_uuid=%1").arg(uuid.toString(QUuid::StringFormat::WithoutBraces)));

    QMetaObject::invokeMethod(netThread.manager.get(), [this, promise, url]() {
        auto* reply = netThread.manager->get(QNetworkRequest(url));

        QtFuture::connect(reply, &QNetworkReply::finished).then([=] {
            if (reply->error() == QNetworkReply::NoError) {
                if (auto terms = TermData::List::create(reply->readAll())) {
                    promise->addResult(*terms);
                } else {
                    promise->addResult(DbErrorCodes::JsonParseError);
                }
            } else {
                promise->addResult(DbErrorCodes::ConnectionError);
            }

            promise->finish();
        });
    });

    return promise->future();
}

Result<QDateTime> DataStorageConnection::getTermLastEdit(const TermUuid& uuid) const
{
    Q_UNIMPLEMENTED();
    return QDateTime();
}

Result<void> DataStorageConnection::addTerm(const TermData& info)
{
    Q_UNIMPLEMENTED();
    return outcome::success();
}

Result<void> DataStorageConnection::updateTerm(const TermData&                      info,
                                               DataStorageInterface::LastEditSource lastEditSource,
                                               bool                                 checkLastEdit)
{
    Q_UNIMPLEMENTED();
    return outcome::success();
}

Result<void> DataStorageConnection::deleteTerm(const TermUuid& uuid)
{
    Q_UNIMPLEMENTED();
    return outcome::success();
}
