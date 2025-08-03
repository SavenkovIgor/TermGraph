// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/DataProvider.h"

import CommonTools.JsonKeys;
import DataSourceFactory;

DataProvider::DataProvider(QObject *parent)
    : QObject{parent}
    , dataStorage{DataSourceFactory::defaultSource()}
{}

void DataProvider::loadGroups()
{
    dataStorage->groups().then([this](Expected<GroupSummary::List> result) {
        if (!result.has_value()) {
            emit showError(static_cast<int>(result.error()));
            return;
        }

        auto groups = result.value();

        // Find global group
        auto globalGroup = std::find_if(groups.begin(), groups.end(), [](const auto &group) {
            return group.name == "Global";
        });

        if (globalGroup != groups.end()) {
            // Move global group to the front
            std::iter_swap(globalGroup, groups.begin());
        }

        mGroups = groups;

        if (!mGroups->empty() && mGroups->front().uuid.has_value()) {
            loadGroup(mGroups->front().uuid.value());
        }

        emit groupListLoaded();
    });
}

bool DataProvider::isReady() const { return mGroups.has_value(); }

int DataProvider::dbVersion() const { return dataStorage->storageVersion(); }

std::optional<GroupUuid> DataProvider::currentGroup() const { return mCurrentGroup; }

void DataProvider::loadGroup(GroupUuid uuid)
{
    dataStorage->terms(uuid).then([this, uuid](Expected<TermData::List> result) {
        if (result.has_value()) {
            mCurrentGroup = uuid;
            mTerms        = result.value();
            emit groupLoaded();
        }
    });
}

const GroupSummary::List &DataProvider::groups() const
{
    static GroupSummary::List dummyEmptyGroups;
    return mGroups ? mGroups.value() : dummyEmptyGroups;
}

std::optional<GroupSummary> DataProvider::group(GroupUuid uuid) const
{
    assert(isReady());
    for (const auto &group : mGroups.value()) {
        if (group.uuid == uuid) {
            return group;
        }
    }
    return std::nullopt;
}

void DataProvider::addGroup(const GroupSummary &info)
{
    dataStorage->addGroup(info).then([this](Expected<GroupSummary> result) {
        if (result.has_value()) {
            emit groupAdded(result.value());
        } else {
            emit showError(static_cast<int>(result.error()));
        }
    });
}

void DataProvider::updateGroup(const GroupSummary &info)
{
    dataStorage->updateGroup(info).then([this](Expected<GroupSummary> result) {
        if (result.has_value()) {
            emit groupUpdated(result.value());
        } else {
            emit showError(static_cast<int>(result.error()));
        }
    });
}

void DataProvider::deleteGroup(const GroupUuid &uuid)
{
    dataStorage->deleteGroup(uuid).then([this](Expected<GroupSummary> result) {
        if (result.has_value()) {
            emit groupDeleted(result.value().uuid.value());
        } else {
            emit showError(static_cast<int>(result.error()));
        }
    });
}

const TermData::List &DataProvider::terms() const
{
    assert(isReady());
    return mTerms;
}

std::optional<TermData> DataProvider::term(const QString &definition, [[maybe_unused]] GroupUuid uuid) const
{
    assert(uuid == mCurrentGroup);
    for (const auto &term : mTerms) {
        if (term.definition == definition) {
            return term;
        }
    }

    return std::nullopt;
}

void DataProvider::addTerm(const TermData &data)
{
    dataStorage->addTerm(data).then([this](Expected<TermData> result) {
        if (result) {
            emit termAdded(result.value());
        } else {
            emit showError(static_cast<int>(result.error()));
        }
    });
}

void DataProvider::updateTerm(const TermData                      &data,
                              DataStorageInterface::LastEditSource lastEditSource,
                              bool                                 checkLastEdit)
{
    dataStorage->updateTerm(data, lastEditSource, checkLastEdit).then([this](Expected<TermData> result) {
        if (result) {
            emit termUpdated(result.value());
        } else {
            emit showError(static_cast<int>(result.error()));
        }
    });
}

void DataProvider::deleteTerm(const TermUuid &uuid)
{
    dataStorage->deleteTerm(uuid).then([this](Expected<TermData> result) {
        if (result) {
            emit termDeleted(result.value().uuid.value());
        } else {
            emit showError(static_cast<int>(result.error()));
        }
    });
}

void DataProvider::importTerm(const TermData &data)
{
    assert(data.uuid);
    dataStorage->term(*data.uuid).then([this, data](Expected<TermData> result) {
        if (result.has_value()) {
            dataStorage->updateTerm(data, DataStorageInterface::LastEditSource::FromData, true);
        } else {
            dataStorage->addTerm(data);
        }
    });
}

void DataProvider::requestGroupExport(const GroupUuid &uuid)
{
    dataStorage->group(uuid).then([this](Expected<GroupSummary> result) {
        if (result.has_value()) {
            auto groupInfo = result.value();
            assert(groupInfo.uuid.has_value());

            dataStorage->terms(groupInfo.uuid.value()).then([this, groupInfo](Expected<TermData::List> termsFuture) {
                if (!termsFuture.has_value()) {
                    return;
                }

                auto       terms = termsFuture.value();
                QJsonArray termArray;

                for (const auto &term : terms) {
                    termArray.append(static_cast<QJsonObject>(term));
                }

                QJsonObject groupJson = static_cast<QJsonObject>(groupInfo);
                groupJson.insert(jsonKeys::termsKey, termArray);

                emit exportGroupReady(QJsonDocument(groupJson));
            });
        }
    });
}
