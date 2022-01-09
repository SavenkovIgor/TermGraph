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

#include <QNetworkReply>
#include <QSharedPointer>

#include <QDebug>

DataStorageConnection::DataStorageConnection(QHostAddress address, quint16 port)
    : DataStorageInterface()
{
}

int DataStorageConnection::storageVersion() const
{
    Q_UNIMPLEMENTED();
    return -1;
}

QUuid DataStorageConnection::getFreeUuid() const
{
    Q_UNIMPLEMENTED();
    return QUuid();
}

GroupUuid::List DataStorageConnection::getAllGroupsUuids(bool sortByLastEdit) const
{
    Q_UNIMPLEMENTED();
    return {};
}

FutureRes<GroupUuid::List> DataStorageConnection::getAllGroupsUuidsAsync(bool sortByLastEdit) const
{
    QSharedPointer<QPromise<Result<GroupUuid::List>>> promise(new QPromise<Result<GroupUuid::List>>);
    promise->start();

    QMetaObject::invokeMethod(netThread.manager.get(), [this, promise]() {
        QUrl url("http://localhost:26748/api/v1/global/groups?type=uuid_only");

        auto  request = QNetworkRequest(url);
        auto* reply   = netThread.manager->get(request);

        QtFuture::connect(reply, &QNetworkReply::finished)
            .then([=] {
                if (reply->error() != QNetworkReply::NoError) {
                    promise->addResult(DbErrorCodes::ConnectionError);
                    qDebug() << "Connection err";
                } else {
                    auto jsonObj = requestBodyToJsonObject(reply->readAll());
                    qDebug() << jsonObj.value();

                    // promise->addResult(GroupUuid::List{});
                }

                promise->finish();
            })
            .onFailed([=] {
                promise->addResult(DbErrorCodes::ConnectionError);
                promise->finish();
            });
    });

    return promise->future();
}

bool DataStorageConnection::groupExist(const GroupUuid& uuid) const
{
    Q_UNIMPLEMENTED();
    return {};
}

Result<GroupData> DataStorageConnection::getGroup(const GroupUuid& uuid) const
{
    Q_UNIMPLEMENTED();
    return GroupData();
}

GroupData::List DataStorageConnection::getGroups() const
{
    Q_UNIMPLEMENTED();
    return {};
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

bool DataStorageConnection::termExist(const TermUuid& uuid) const
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
    return TermData();
}

Result<TermData::List> DataStorageConnection::getTerms(const GroupUuid& uuid) const
{
    Q_UNIMPLEMENTED();
    return TermData::List();
}

Result<TermData::List> DataStorageConnection::getTerms(const UuidList& termsUuids) const
{
    Q_UNIMPLEMENTED();
    return TermData::List();
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

Opt<QJsonObject> DataStorageConnection::requestBodyToJsonObject(const QByteArray& data)
{
    auto doc = QJsonDocument::fromJson(data);

    if (doc.isNull())
        return std::nullopt;

    return doc.object();
}
