// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "include/TermDataStorage/LocalDataStorage.h"

#include <QMap>

#include "source/Database.h"

template<typename T>
static QFuture<T> toFuture(const std::function<T()>& func)
{
    QPromise<T> promise;
    promise.start();
    promise.addResult(func());
    promise.finish();
    return promise.future();
}

class StorageImpl
{
public:
    StorageImpl(const QString& filePath, const QString& backupFolderPath)
        : db(filePath, backupFolderPath)
    {}

    Database db;
};

LocalDatabaseStorage::LocalDatabaseStorage(const QString& filePath, const QString& backupFolderPath)
    : DataStorageInterface()
    , impl(new StorageImpl(filePath, backupFolderPath))
{}

int LocalDatabaseStorage::storageVersion() const { return impl->db.appConfigTable->getDbVersion(); }

FutureResult<GroupUuid::List> LocalDatabaseStorage::allGroupsUuids() const
{
    // Load info from groups table - need if any group is empty and has no lastEdit value
    QMap<QUuid, QDateTime> groupsLastEdit;

    for (const auto& uuid : impl->db.groupTable->allUuids())
        groupsLastEdit.insert(uuid.get(), QDateTime());

    for (const auto& record : impl->db.termTable->allLastEditRecords()) {
        QUuid     groupUuid = QUuid(record.value("groupUuid").toString());
        QDateTime lastEdit  = QDateTime::fromString(record.value("lastEdit").toString(), Qt::ISODate);

        if (groupsLastEdit.contains(groupUuid)) {
            if (groupsLastEdit[groupUuid].isNull()) {
                groupsLastEdit[groupUuid] = lastEdit;
            } else {
                groupsLastEdit[groupUuid] = std::max(groupsLastEdit[groupUuid], lastEdit);
            }
        }
    }

    // Casting to pairs
    QList<QPair<QUuid, QDateTime>> groupSorting;

    // Forming structure with group uuids and last edit times
    for (auto& [groupUuid, lastEdit] : groupsLastEdit.toStdMap()) {
        groupSorting.append(QPair<QUuid, QDateTime>(groupUuid, lastEdit));
    }

    // Sorting this structure
    auto groupOrdering = [](const auto& g1, const auto& g2) { return g1.second > g2.second; };

    std::sort(groupSorting.begin(), groupSorting.end(), groupOrdering);

    // Casting back to uuids only
    GroupUuid::List ret;

    for (const auto& group : groupSorting)
        if (auto gUuid = GroupUuid::create(group.first))
            ret.push_back(*gUuid);

    return toFuture<Result<GroupUuid::List>>([&ret] { return ret; });
}

FutureResult<GroupData> LocalDatabaseStorage::group(const GroupUuid& uuid) const
{
    return toFuture<Result<GroupData>>([this, &uuid] { return impl->db.groupTable->group(uuid); });
}

FutureResult<GroupData::List> LocalDatabaseStorage::groups() const
{
    return toFuture<Result<GroupData::List>>([this] { return impl->db.groupTable->allGroups(); });
}

FutureResult<GroupData> LocalDatabaseStorage::addGroup(const GroupData& info)
{
    return toFuture<Result<GroupData>>([this, info] { return impl->db.groupTable->addGroup(info); });
}

FutureResult<GroupData> LocalDatabaseStorage::updateGroup(const GroupData& info)
{
    return toFuture<Result<GroupData>>([this, info] { return impl->db.groupTable->updateGroup(info); });
}

FutureResult<GroupData> LocalDatabaseStorage::deleteGroup(const GroupUuid& uuid)
{
    return toFuture<Result<GroupData>>([this, uuid] { return impl->db.groupTable->deleteGroup(uuid); });
}

FutureResult<TermData> LocalDatabaseStorage::term(const QString& nodeName, const GroupUuid& uuid) const
{
    if (!impl->db.groupTable->exist(uuid))
        return toFuture<Result<TermData>>([] { return DbErrorCodes::GroupUuidNotFound; });

    return toFuture<Result<TermData>>([this, nodeName, uuid] { return impl->db.termTable->term(nodeName, uuid); });
}

FutureResult<TermData> LocalDatabaseStorage::term(const TermUuid& uuid) const
{
    return toFuture<Result<TermData>>([this, uuid] { return impl->db.termTable->term(uuid); });
}

FutureResult<TermData::List> LocalDatabaseStorage::terms(const GroupUuid& uuid) const
{
    if (!impl->db.groupTable->exist(uuid))
        return toFuture<Result<TermData::List>>([] { return DbErrorCodes::GroupUuidNotFound; });

    return toFuture<Result<TermData::List>>([this, uuid] { return impl->db.termTable->allTerms(uuid); });
}

FutureResult<TermData> LocalDatabaseStorage::addTerm(const TermData& info)
{
    if (!impl->db.groupTable->exist(info.groupUuid))
        return toFuture<Result<TermData>>([] { return DbErrorCodes::GroupUuidNotFound; });

    return toFuture<Result<TermData>>([this, info] { return impl->db.termTable->addTerm(info); });
}

FutureResult<TermData> LocalDatabaseStorage::updateTerm(const TermData&                      info,
                                                     DataStorageInterface::LastEditSource lastEditSource,
                                                     bool                                 checkLastEdit)
{
    if (!impl->db.groupTable->exist(info.groupUuid))
        return toFuture<Result<TermData>>([] { return DbErrorCodes::GroupUuidNotFound; });

    return toFuture<Result<TermData>>([this, info, lastEditSource, checkLastEdit] {
        return impl->db.termTable->updateTerm(info, lastEditSource, checkLastEdit);
    });
}

FutureResult<TermData> LocalDatabaseStorage::deleteTerm(const TermUuid& uuid)
{
    return toFuture<Result<TermData>>([this, uuid] { return impl->db.termTable->deleteTerm(uuid); });
}
