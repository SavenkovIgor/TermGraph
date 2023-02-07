// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "include/StaticDataStorage/StaticDataStorage.h"

#include <QMap>

#include <TermDataInterface/GroupData.h>

template<typename T>
static QFuture<T> toFuture(const std::function<T()>& func)
{
    QPromise<T> promise;
    promise.start();
    promise.addResult(func());
    promise.finish();
    return promise.future();
}

class StaticStorageImpl
{
public:
    StaticStorageImpl()
    {
        // Take all files from data folder with extension .json
        QDir dataDir(dataFolderPath);
        auto files = dataDir.entryInfoList(QStringList() << "*.json", QDir::Files);

        for (const auto& fileInfo : files) {

            QFile file(fileInfo.absoluteFilePath());
            file.open(QIODevice::ReadOnly);
            auto fileData = file.readAll();

            auto group = GroupData::from(fileData);

            if (group) {
                assert(group->uuid.has_value());
                mGroups.insert(group->uuid.value(), group.value());
            }
        }
    }

    constexpr static auto dataFolderPath = ":/data/";
    QMap<GroupUuid, GroupData> mGroups;
};

StaticDataStorage::StaticDataStorage()
    : DataStorageInterface()
    , impl(new StaticStorageImpl())
{
}

int StaticDataStorage::storageVersion() const { return 1; }

FutureResult<GroupSummary> StaticDataStorage::group(const GroupUuid& uuid) const
{
    return toFuture<Result<GroupSummary>>([this, &uuid] () { return impl->mGroups.value(uuid); });
}

FutureResult<GroupSummary::List> StaticDataStorage::groups() const
{
    return toFuture<Result<GroupSummary::List>>([this] () -> GroupSummary::List {

        auto lastEdits = termsLastEdit();
        auto groups = impl->mGroups.values();

        // Sorting this structure
        auto groupOrdering = [&lastEdits](const auto& g1, const auto& g2) {
            return g1.termsLastEdit() > g2.termsLastEdit();
        };

        std::sort(groups.begin(), groups.end(), groupOrdering);

        GroupSummary::List result;

        for (const auto& group : groups) {
            result.push_back(group);
        }

        return result;
    });
}

FutureResult<GroupSummary> StaticDataStorage::addGroup([[maybe_unused]] const GroupSummary& info)
{
    qWarning("Not implemented");
    return toFuture<Result<GroupSummary>>([] { return ErrorCodes::UnknownError; });
}

FutureResult<GroupSummary> StaticDataStorage::updateGroup([[maybe_unused]]const GroupSummary& info)
{
    qWarning("Not implemented");
    return toFuture<Result<GroupSummary>>([] { return ErrorCodes::UnknownError; });
}

FutureResult<GroupSummary> StaticDataStorage::deleteGroup([[maybe_unused]]const GroupUuid& uuid)
{
    qWarning("Not implemented");
    return toFuture<Result<GroupSummary>>([] { return ErrorCodes::UnknownError; });
}

FutureResult<TermData> StaticDataStorage::term(const QString& nodeName, const GroupUuid& uuid) const
{
    if (!impl->mGroups.contains(uuid)) {
        return toFuture<Result<TermData>>([] { return ErrorCodes::GroupUuidNotFound; });
    }

    return toFuture<Result<TermData>>([this, nodeName, uuid] () -> Result<TermData> {
        auto term = impl->mGroups.value(uuid).term(nodeName);
        if (!term) {
            return ErrorCodes::TermNotFound;
        }

        return term.value();
    });
}

FutureResult<TermData> StaticDataStorage::term([[maybe_unused]] const TermUuid& uuid) const
{
    qWarning("Not implemented");
    return toFuture<Result<TermData>>([] { return ErrorCodes::UnknownError; });
}

FutureResult<TermData::List> StaticDataStorage::terms(const GroupUuid& uuid) const
{
    if (!impl->mGroups.contains(uuid)) {
        return toFuture<Result<TermData::List>>([] { return ErrorCodes::GroupUuidNotFound; });
    }

    return toFuture<Result<TermData::List>>([this, uuid] () -> Result<TermData::List> {
        if (!impl->mGroups.contains(uuid)) {
            return ErrorCodes::GroupUuidNotFound;
        }

        return impl->mGroups.value(uuid).terms;
    });
}

FutureResult<TermData> StaticDataStorage::addTerm([[maybe_unused]] const TermData& info)
{
    qWarning("Not implemented");
    return toFuture<Result<TermData>>([] { return ErrorCodes::UnknownError; });
}

FutureResult<TermData> StaticDataStorage::updateTerm([[maybe_unused]] const TermData&                      info,
                                                     [[maybe_unused]] DataStorageInterface::LastEditSource lastEditSource,
                                                     [[maybe_unused]] bool                                 checkLastEdit)
{
    qWarning("Not implemented");
    return toFuture<Result<TermData>>([] { return ErrorCodes::UnknownError; });
}

FutureResult<TermData> StaticDataStorage::deleteTerm([[maybe_unused]] const TermUuid& uuid)
{
    qWarning("Not implemented");
    return toFuture<Result<TermData>>([] { return ErrorCodes::UnknownError; });
}

QMap<GroupUuid, QDateTime> StaticDataStorage::termsLastEdit() const
{
    QMap<GroupUuid, QDateTime> ret;

    for (const auto& group : impl->mGroups) {
        assert(group.uuid.has_value());
        ret.insert(group.uuid.value(), group.termsLastEdit().value_or(QDateTime()));
    }

    return ret;
}