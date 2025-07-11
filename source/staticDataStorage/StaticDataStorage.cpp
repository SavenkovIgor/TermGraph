// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/staticDataStorage/StaticDataStorage.h"

#include <QDir>
#include <QMap>

namespace rng = std::ranges;

StaticDataStorage::StaticDataStorage()
    : DataStorageInterface()
{
    Q_INIT_RESOURCE(Data);

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
    [[maybe_unused]] auto result = file.open(QIODevice::ReadOnly);
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
        term.uuid      = TermUuid::generate();
        term.groupUuid = data.uuid.value();
        term.lastEdit  = lastEdit;
    }

    return data;
}

int StaticDataStorage::storageVersion() const { return 1; }

FutureExpected<GroupSummary> StaticDataStorage::group(const GroupUuid& uuid) const
{
    return toFuture<Expected<GroupSummary>>([this, &uuid]() { return mGroups.value(uuid); });
}

FutureExpected<GroupSummary::List> StaticDataStorage::groups() const
{
    return toFuture<Expected<GroupSummary::List>>([this]() -> GroupSummary::List {
        auto groups = mGroups.values();

        // Sorting this structure
        rng::sort(groups, [](const auto& g1, const auto& g2) { return g1.termsLastEdit() > g2.termsLastEdit(); });

        GroupSummary::List result;

        for (const auto& group : groups) {
            result.push_back(group);
        }

        return result;
    });
}

FutureExpected<GroupSummary> StaticDataStorage::addGroup([[maybe_unused]] const GroupSummary& info)
{
    qWarning("Not implemented");
    return toFuture<Expected<GroupSummary>>([] { return std::unexpected(ErrorCode::UnknownError); });
}

FutureExpected<GroupSummary> StaticDataStorage::updateGroup([[maybe_unused]] const GroupSummary& info)
{
    qWarning("Not implemented");
    return toFuture<Expected<GroupSummary>>([] { return std::unexpected(ErrorCode::UnknownError); });
}

FutureExpected<GroupSummary> StaticDataStorage::deleteGroup([[maybe_unused]] const GroupUuid& uuid)
{
    qWarning("Not implemented");
    return toFuture<Expected<GroupSummary>>([] { return std::unexpected(ErrorCode::UnknownError); });
}

FutureExpected<TermData> StaticDataStorage::term(const QString& nodeName, const GroupUuid& uuid) const
{
    if (!mGroups.contains(uuid)) {
        return toFuture<Expected<TermData>>([] { return std::unexpected(ErrorCode::GroupUuidNotFound); });
    }

    return toFuture<Expected<TermData>>([this, nodeName, uuid]() -> Expected<TermData> {
        auto term = mGroups.value(uuid).term(nodeName);
        if (!term) {
            return std::unexpected(ErrorCode::TermNotFound);
        }

        return term.value();
    });
}

FutureExpected<TermData> StaticDataStorage::term([[maybe_unused]] const TermUuid& uuid) const
{
    qWarning("Not implemented");
    return toFuture<Expected<TermData>>([] { return std::unexpected(ErrorCode::UnknownError); });
}

FutureExpected<TermData::List> StaticDataStorage::terms(const GroupUuid& uuid) const
{
    if (!mGroups.contains(uuid)) {
        return toFuture<Expected<TermData::List>>([] { return std::unexpected(ErrorCode::GroupUuidNotFound); });
    }

    return toFuture<Expected<TermData::List>>([this, uuid]() -> Expected<TermData::List> {
        if (!mGroups.contains(uuid)) {
            return std::unexpected(ErrorCode::GroupUuidNotFound);
        }

        return mGroups.value(uuid).terms;
    });
}

FutureExpected<TermData> StaticDataStorage::addTerm([[maybe_unused]] const TermData& info)
{
    qWarning("Not implemented");
    return toFuture<Expected<TermData>>([] { return std::unexpected(ErrorCode::UnknownError); });
}

FutureExpected<TermData> StaticDataStorage::updateTerm(
    [[maybe_unused]] const TermData&                      info,
    [[maybe_unused]] DataStorageInterface::LastEditSource lastEditSource,
    [[maybe_unused]] bool                                 checkLastEdit)
{
    qWarning("Not implemented");
    return toFuture<Expected<TermData>>([] { return std::unexpected(ErrorCode::UnknownError); });
}

FutureExpected<TermData> StaticDataStorage::deleteTerm([[maybe_unused]] const TermUuid& uuid)
{
    qWarning("Not implemented");
    return toFuture<Expected<TermData>>([] { return std::unexpected(ErrorCode::UnknownError); });
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
