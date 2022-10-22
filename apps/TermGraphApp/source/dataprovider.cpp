// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "dataprovider.h"

#include "source/datasourcefactory.h"

DataProvider::DataProvider(QObject *parent)
    : QObject{parent},
    dataStorage{DataSourceFactory::defaultSource()}
{
}

void DataProvider::loadGroups()
{
    dataStorage->groups().then([this](Result<GroupData::List> result){
        if (result.has_value()) {
            mGroups = result.value();
            if (!mGroups->empty() && mGroups->front().uuid.has_value())
                loadGroup(mGroups->front().uuid.value());
            emit groupListLoaded();
        }
    });
}

bool DataProvider::isReady() const
{
    return mGroups.has_value();
}

int DataProvider::dbVersion() const
{
    return dataStorage->storageVersion();
}

Opt<GroupUuid> DataProvider::currentGroup() const
{
    return mCurrentGroup;
}

void DataProvider::loadGroup(GroupUuid uuid)
{
    dataStorage->terms(uuid).then([this, uuid](Result<TermData::List> result){
        if (result.has_value()) {
            mCurrentGroup = uuid;
            mTerms = result.value();
            emit groupLoaded();
        }
    });
}

const GroupData::List &DataProvider::groups() const
{
    static GroupData::List dummyEmptyGroups;
    return mGroups ? mGroups.value() : dummyEmptyGroups;
}

Opt<GroupData> DataProvider::group(GroupUuid uuid) const
{
    assert(isReady());
    for (const auto& group : mGroups.value()) {
        if (group.uuid == uuid) {
            return group;
        }
    }
    return std::nullopt;
}

void DataProvider::addGroup(const GroupData &info)
{
    dataStorage->addGroup(info).then([this](Result<GroupData> result){
        if (result.has_value()) {
            emit groupAdded(result.value());
        } else {
            emit showError(result.error().value());
        }
    });
}

void DataProvider::updateGroup(const GroupData &info)
{
    dataStorage->updateGroup(info).then([this](Result<GroupData> result) {
        if (result.has_value()) {
            emit groupUpdated(result.value());
        } else {
            emit showError(result.error().value());
        }
    });
}

void DataProvider::deleteGroup(const GroupUuid &uuid)
{
    dataStorage->deleteGroup(uuid).then([this](Result<GroupData> result){
        if (result.has_value()) {
            emit groupDeleted(result.value().uuid.value());
        } else {
            emit showError(result.error().value());
        }
    });
}

bool DataProvider::hasTerm(const TermUuid &uuid) const
{
    auto iter = std::find_if(mTerms.begin(), mTerms.end(), [uuid](const auto& term){ return term.uuid == uuid; });
    return iter  != mTerms.end();
}

const TermData::List &DataProvider::terms() const
{
    assert(isReady());
    return mTerms;
}

Opt<TermData> DataProvider::term(const QString &definition, GroupUuid uuid) const
{
    assert(uuid == mCurrentGroup);
    for (const auto& term : mTerms) {
        if (term.definition == definition)
            return term;
    }

    return std::nullopt;
}

void DataProvider::addTerm(const TermData &data)
{
    dataStorage->addTerm(data).then([this](Result<TermData> result){
        if (result) {
            emit termAdded(result.value());
        } else {
            emit showError(result.error().value());
        }
    });
}

void DataProvider::updateTerm(const TermData &data, DataStorageInterface::LastEditSource lastEditSource, bool checkLastEdit)
{
    dataStorage->updateTerm(data, lastEditSource, checkLastEdit).then([this](Result<TermData> result){
        if (result) {
            emit termUpdated(result.value());
        } else {
            emit showError(result.error().value());
        }
    });
}

void DataProvider::deleteTerm(const TermUuid &uuid)
{
    dataStorage->deleteTerm(uuid).then([this](Result<TermData> result){
        if (result) {
            emit termDeleted(result.value().uuid.value());
        } else {
            emit showError(result.error().value());
        }
    });
}

