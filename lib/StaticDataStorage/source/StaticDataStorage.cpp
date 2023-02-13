// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "include/StaticDataStorage/StaticDataStorage.h"

#include <QMap>
#include <QDir>

template<typename T>
static QFuture<T> toFuture(const std::function<T()>& func)
{
    QPromise<T> promise;
    promise.start();
    promise.addResult(func());
    promise.finish();
    return promise.future();
}

StaticDataStorage::StaticDataStorage()
    : DataStorageInterface()
{
    Q_INIT_RESOURCE(StaticData);

    // Take all files from data folder with extension .json
    for (const auto& fileInfo : files()) {

        auto fileData = qrcFileData(fileInfo.absoluteFilePath());

        auto group = StaticGroupData::from(fileData);

        if (group) {
            auto preparedGroup = prepareForInternalUse(group.value());
            mGroups.insert(preparedGroup.uuid.value(), preparedGroup);
        }
    }
}

QFileInfoList StaticDataStorage::files()
{
    QDir dataDir(dataFolderPath);
    auto formats = QStringList() << "*.json";
    return dataDir.entryInfoList(formats, QDir::Files);
}

QByteArray StaticDataStorage::qrcFileData(const QString& filePath)
{
    QFile file(filePath);
    assert(file.exists());
    auto result = file.open(QIODevice::ReadOnly);
    assert(result);
    return file.readAll();
}

StaticGroupData StaticDataStorage::prepareForInternalUse(StaticGroupData data)
{
    if (!data.uuid.has_value()) {
        data.uuid = GroupUuid::generate();
    }

    auto lastEdit = QDateTime::currentDateTime();

    for (auto& term : data.terms) {
        term.groupUuid = data.uuid.value();
        term.lastEdit = lastEdit;
    }

    return data;
}

int StaticDataStorage::storageVersion() const { return 1; }

FutureResult<GroupSummary> StaticDataStorage::group(const GroupUuid& uuid) const
{
    return toFuture<Result<GroupSummary>>([this, &uuid] () { return mGroups.value(uuid); });
}

FutureResult<GroupSummary::List> StaticDataStorage::groups() const
{
    return toFuture<Result<GroupSummary::List>>([this] () -> GroupSummary::List {

        auto lastEdits = termsLastEdit();
        auto groups = mGroups.values();

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
    if (!mGroups.contains(uuid)) {
        return toFuture<Result<TermData>>([] { return ErrorCodes::GroupUuidNotFound; });
    }

    return toFuture<Result<TermData>>([this, nodeName, uuid] () -> Result<TermData> {
        auto term = mGroups.value(uuid).term(nodeName);
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
    if (!mGroups.contains(uuid)) {
        return toFuture<Result<TermData::List>>([] { return ErrorCodes::GroupUuidNotFound; });
    }

    return toFuture<Result<TermData::List>>([this, uuid] () -> Result<TermData::List> {
        if (!mGroups.contains(uuid)) {
            return ErrorCodes::GroupUuidNotFound;
        }

        return mGroups.value(uuid).terms;
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

    for (const auto& group : mGroups) {
        assert(group.uuid.has_value());
        ret.insert(group.uuid.value(), group.termsLastEdit().value_or(QDateTime()));
    }

    return ret;
}
