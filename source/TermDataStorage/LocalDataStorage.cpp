// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/TermDataStorage/LocalDataStorage.h"

#include <QMap>

#include "source/TermDataStorage/Database.h"

namespace rng = std::ranges;

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

FutureResult<GroupSummary> LocalDatabaseStorage::group(const GroupUuid& uuid) const
{
    return toFuture<Result<GroupSummary>>([this, &uuid] {
        auto res = impl->db.groupTable->group(uuid);
        return res ? *res : Result<GroupSummary>(res.error());
    });
}

FutureResult<GroupSummary::List> LocalDatabaseStorage::groups() const
{
    return toFuture<Result<GroupSummary::List>>([this] {
        auto lastEdits = nodesLastEdit();
        auto groups    = impl->db.groupTable->allGroups();

        for (auto& group : groups) {
            assert(group.uuid.has_value());
            auto uuid     = group.uuid.value();
            auto lastEdit = lastEdits.value(uuid, QDateTime());

            if (!lastEdit.isNull()) {
                group.nodesLastEdit = lastEdit;
            } else {
                group.nodesLastEdit = std::nullopt;
            }
        }

        // Sorting this structure
        rng::sort(groups, [](const auto& g1, const auto& g2) { return g1.nodesLastEdit > g2.nodesLastEdit; });

        return groups;
    });
}

FutureResult<GroupSummary> LocalDatabaseStorage::addGroup(const GroupSummary& info)
{
    return toFuture<Result<GroupSummary>>([this, info] {
        auto res = impl->db.groupTable->addGroup(info);
        return res ? *res : Result<GroupSummary>(res.error());
    });
}

FutureResult<GroupSummary> LocalDatabaseStorage::updateGroup(const GroupSummary& info)
{
    return toFuture<Result<GroupSummary>>([this, info] {
        auto res = impl->db.groupTable->updateGroup(info);
        return res ? *res : Result<GroupSummary>(res.error());
    });
}

FutureResult<GroupSummary> LocalDatabaseStorage::deleteGroup(const GroupUuid& uuid)
{
    return toFuture<Result<GroupSummary>>([this, uuid] {
        auto res = impl->db.groupTable->deleteGroup(uuid);
        return res ? *res : Result<GroupSummary>(res.error());
    });
}

FutureResult<TermData> LocalDatabaseStorage::term(const QString& nodeName, const GroupUuid& uuid) const
{
    if (!impl->db.groupTable->exist(uuid)) {
        return toFuture<Result<TermData>>([] { return ErrorCode::GroupUuidNotFound; });
    }

    return toFuture<Result<TermData>>([this, nodeName, uuid] {
        auto res = impl->db.termTable->term(nodeName, uuid);
        return res ? *res : Result<TermData>(res.error());
    });
}

FutureResult<TermData> LocalDatabaseStorage::term(const TermUuid& uuid) const
{
    return toFuture<Result<TermData>>([this, uuid] {
        auto res = impl->db.termTable->term(uuid);
        return res ? *res : Result<TermData>(res.error());
    });
}

FutureResult<TermData::List> LocalDatabaseStorage::terms(const GroupUuid& uuid) const
{
    if (!impl->db.groupTable->exist(uuid)) {
        return toFuture<Result<TermData::List>>([] { return ErrorCode::GroupUuidNotFound; });
    }

    return toFuture<Result<TermData::List>>([this, uuid] {
        auto res = impl->db.termTable->allTerms(uuid);
        return res ? *res : Result<TermData::List>(res.error());
    });
}

FutureResult<TermData> LocalDatabaseStorage::addTerm(const TermData& info)
{
    if (!impl->db.groupTable->exist(info.groupUuid)) {
        return toFuture<Result<TermData>>([] { return ErrorCode::GroupUuidNotFound; });
    }

    return toFuture<Result<TermData>>([this, info] {
        auto res = impl->db.termTable->addTerm(info);
        return res ? *res : Result<TermData>(res.error());
    });
}

FutureResult<TermData> LocalDatabaseStorage::updateTerm(const TermData&                      info,
                                                        DataStorageInterface::LastEditSource lastEditSource,
                                                        bool                                 checkLastEdit)
{
    if (!impl->db.groupTable->exist(info.groupUuid)) {
        return toFuture<Result<TermData>>([] { return ErrorCode::GroupUuidNotFound; });
    }

    return toFuture<Result<TermData>>([this, info, lastEditSource, checkLastEdit] {
        auto res = impl->db.termTable->updateTerm(info, lastEditSource, checkLastEdit);
        return res ? *res : Result<TermData>(res.error());
    });
}

FutureResult<TermData> LocalDatabaseStorage::deleteTerm(const TermUuid& uuid)
{
    return toFuture<Result<TermData>>([this, uuid] {
        auto res = impl->db.termTable->deleteTerm(uuid);
        return res ? *res : Result<TermData>(res.error());
    });
}

QMap<GroupUuid, QDateTime> LocalDatabaseStorage::nodesLastEdit() const
{
    QMap<GroupUuid, QDateTime> ret;

    for (const auto& group : impl->db.groupTable->allGroups()) {
        assert(group.uuid.has_value());
        ret.insert(group.uuid.value(), QDateTime());
    }

    for (const auto& record : impl->db.termTable->allLastEditRecords()) {
        std::optional<GroupUuid> uuid     = GroupUuid::from(record.value("groupUuid").toString());
        QDateTime                lastEdit = QDateTime::fromString(record.value("lastEdit").toString(), Qt::ISODate);

        assert(uuid.has_value());
        assert(!lastEdit.isNull());

        if (ret.contains(*uuid)) {
            if (ret[*uuid].isNull()) {
                ret[*uuid] = lastEdit;
            } else {
                ret[*uuid] = std::max(ret[*uuid], lastEdit);
            }
        }
    }

    return ret;
}
