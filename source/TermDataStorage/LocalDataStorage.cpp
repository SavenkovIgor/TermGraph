// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QMap>

#include "source/TermDataInterface/DataStorageInterface.h"
#include "source/TermDataInterface/GroupSummary.h"
#include "source/TermDataInterface/TermData.h"

export module LocalDatabaseStorage;

import AppConfigTable;
import Database;
import TermGroupTable;
import TermTable;

namespace rng = std::ranges;

class StorageImpl
{
public:
    StorageImpl(const QString& filePath, const QString& backupFolderPath)
        : db(filePath, backupFolderPath)
    {}

    Database db;
};

export class LocalDatabaseStorage : public DataStorageInterface
{
public:
    LocalDatabaseStorage(const QString& filePath, const QString& backupFolderPath);

public:
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
    StorageImpl* impl = nullptr;

    QMap<GroupUuid, QDateTime> nodesLastEdit() const;
};

LocalDatabaseStorage::LocalDatabaseStorage(const QString& filePath, const QString& backupFolderPath)
    : DataStorageInterface()
    , impl(new StorageImpl(filePath, backupFolderPath))
{}

int LocalDatabaseStorage::storageVersion() const { return impl->db.appConfigTable->getDbVersion(); }

FutureExpected<GroupSummary> LocalDatabaseStorage::group(const GroupUuid& uuid) const
{
    return toFuture<Expected<GroupSummary>>([this, &uuid] { return impl->db.groupTable->group(uuid); });
}

FutureExpected<GroupSummary::List> LocalDatabaseStorage::groups() const
{
    return toFuture<Expected<GroupSummary::List>>([this] {
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

FutureExpected<GroupSummary> LocalDatabaseStorage::addGroup(const GroupSummary& info)
{
    return toFuture<Expected<GroupSummary>>([this, info] { return impl->db.groupTable->addGroup(info); });
}

FutureExpected<GroupSummary> LocalDatabaseStorage::updateGroup(const GroupSummary& info)
{
    return toFuture<Expected<GroupSummary>>([this, info] { return impl->db.groupTable->updateGroup(info); });
}

FutureExpected<GroupSummary> LocalDatabaseStorage::deleteGroup(const GroupUuid& uuid)
{
    return toFuture<Expected<GroupSummary>>([this, uuid] { return impl->db.groupTable->deleteGroup(uuid); });
}

FutureExpected<TermData> LocalDatabaseStorage::term(const QString& nodeName, const GroupUuid& uuid) const
{
    if (!impl->db.groupTable->exist(uuid)) {
        return toFuture<Expected<TermData>>([] { return std::unexpected(ErrorCode::GroupUuidNotFound); });
    }

    return toFuture<Expected<TermData>>([this, nodeName, uuid] { return impl->db.termTable->term(nodeName, uuid); });
}

FutureExpected<TermData> LocalDatabaseStorage::term(const TermUuid& uuid) const
{
    return toFuture<Expected<TermData>>([this, uuid] { return impl->db.termTable->term(uuid); });
}

FutureExpected<TermData::List> LocalDatabaseStorage::terms(const GroupUuid& uuid) const
{
    if (!impl->db.groupTable->exist(uuid)) {
        return toFuture<Expected<TermData::List>>([] { return std::unexpected(ErrorCode::GroupUuidNotFound); });
    }

    return toFuture<Expected<TermData::List>>([this, uuid] { return impl->db.termTable->allTerms(uuid); });
}

FutureExpected<TermData> LocalDatabaseStorage::addTerm(const TermData& info)
{
    if (!impl->db.groupTable->exist(info.groupUuid)) {
        return toFuture<Expected<TermData>>([] { return std::unexpected(ErrorCode::GroupUuidNotFound); });
    }

    return toFuture<Expected<TermData>>([this, info] { return impl->db.termTable->addTerm(info); });
}

FutureExpected<TermData> LocalDatabaseStorage::updateTerm(const TermData&                      info,
                                                          DataStorageInterface::LastEditSource lastEditSource,
                                                          bool                                 checkLastEdit)
{
    if (!impl->db.groupTable->exist(info.groupUuid)) {
        return toFuture<Expected<TermData>>([] { return std::unexpected(ErrorCode::GroupUuidNotFound); });
    }

    return toFuture<Expected<TermData>>([this, info, lastEditSource, checkLastEdit] {
        return impl->db.termTable->updateTerm(info, lastEditSource, checkLastEdit);
    });
}

FutureExpected<TermData> LocalDatabaseStorage::deleteTerm(const TermUuid& uuid)
{
    return toFuture<Expected<TermData>>([this, uuid] { return impl->db.termTable->deleteTerm(uuid); });
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
