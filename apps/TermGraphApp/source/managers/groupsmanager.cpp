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

#include "source/managers/groupsmanager.h"

#include <QFile>
#include <QJsonArray>
#include <QUrl>

#include <QDebug>

#include "source/helpers/appconfig.h"

GroupsManager::GroupsManager(DataStorageInterface& dataSource, NotifyInterface& notifier, QObject* parent)
    : QObject(parent)
    , dataSource(dataSource)
    , notifier(notifier)
{
    connect(this, &GroupsManager::groupAdded, this, &GroupsManager::groupsListChanged);
    connect(this, &GroupsManager::groupDeleted, this, &GroupsManager::groupsListChanged);
    connect(this, &GroupsManager::nodeChanged, this, &GroupsManager::groupsListChanged);
}

QString GroupsManager::getGroupName(const QUuid& groupUuid) const { return uuidToNames.value(groupUuid); }

QString GroupsManager::getLastEditString(QUuid groupUuid) { return getLastEdit(groupUuid).toString(); }

int GroupsManager::getNodesCount(QUuid groupUuid)
{
    if (auto uuid = GroupUuid::create(groupUuid))
        return dataSource.getAllTermsUuids(*uuid).result().value().size();

    Q_UNREACHABLE();
    return 0;
}

void GroupsManager::addNewGroup(const QString& name, const QString& comment)
{
    if (name.simplified().isEmpty()) {
        notifier.showError("Название группы не может быть пустым");
        return;
    }

    GroupData info;

    info.name    = name;
    info.comment = comment;

    if (dataSource.addGroup(info).result()) {
        updateGroupUuidNameMaps();
        emit groupAdded();
    } else {
        notifier.showError("Название группы не уникально");
    }
}

void GroupsManager::deleteGroup(const QString& groupUuid)
{
    if (auto uuid = GroupUuid::create(groupUuid)) {
        if (auto delResult = dataSource.deleteGroup(*uuid).result()) {
            updateGroupUuidNameMaps();
            emit groupDeleted();
            return;
        }
    }
    Q_UNREACHABLE();
}

void GroupsManager::importGroupFromJsonFile(const QString& filename)
{
    QUrl  url(filename);
    QFile file(url.toLocalFile());
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray arr = file.readAll();
        importGroupFromJsonString(QString(arr));
    }
}

void GroupsManager::importGroupFromJsonString(const QString& rawJson)
{
    QJsonDocument doc = QJsonDocument::fromJson(rawJson.toUtf8());
    importGroup(doc);
}

TermGroup::OptPtr GroupsManager::createGroup(const QUuid groupUuid)
{
    if (auto uuid = GroupUuid::create(groupUuid)) {
        auto groupData = dataSource.getGroup(*uuid).result();
        auto termsData = dataSource.getTerms(*uuid).result();

        if (groupData.has_error() || termsData.has_error())
            return std::nullopt;

        return std::make_shared<TermGroup>(groupData.value(), termsData.value());
    }

    return std::nullopt;
}

bool GroupsManager::isEmptyGroup(const QString& groupUuid)
{
    QUuid uuid(groupUuid);

    if (uuid.isNull())
        return true;

    return getNodesCount(uuid) == 0;
}

bool GroupsManager::getHasAnyGroup() const
{
    auto groupsRes = dataSource.getAllGroupsUuids().result();
    return groupsRes.has_value() ? (!groupsRes.value().empty()) : false;
}

QDateTime GroupsManager::getLastEdit(QUuid groupUuid)
{
    assert(!groupUuid.isNull());
    auto uuid = GroupUuid::create(groupUuid).value();

    QDateTime lastEdit;
    for (auto& nodeUuid : dataSource.getAllTermsUuids(uuid).result().value()) {
        QDateTime currNodeLastEdit = dataSource.getTermLastEdit(nodeUuid).value();
        if (lastEdit.isNull()) {
            lastEdit = currNodeLastEdit;
        } else {
            lastEdit = std::max(lastEdit, currNodeLastEdit);
        }
    }
    return lastEdit;
}

UuidList GroupsManager::getAllUuidsSortedByLastEdit()
{
    if (auto groupsUuids = dataSource.getAllGroupsUuids(true).result()) {
        UuidList ret;
        for (const auto& uuid : groupsUuids.value())
            ret.push_back(uuid);

        return ret;
    }

    return {};
}

QStringList GroupsManager::getAllUuidStringsSortedByLastEdit()
{
    QStringList ret;
    for (auto uuid : getAllUuidsSortedByLastEdit())
        ret << uuid.toString();

    return ret;
}

void GroupsManager::importGroup(const QJsonDocument& json)
{
    QJsonObject jsonGroup = json.object();

    if (!GroupJsonValidator::importChecks().check(jsonGroup))
        return;

    auto groupData = GroupData::create(jsonGroup);

    if (!groupData || !groupData->uuid)
        return;

    // Searching for existed group
    if (dataSource.getGroup(*groupData->uuid).result().has_value()) { // Group found
        if (!dataSource.updateGroup(*groupData).result()) {
            return;
        }
    } else {
        if (!dataSource.addGroup(*groupData).result()) {
            return;
        }
    }

    QJsonArray nodes = jsonGroup["nodesList"].toArray();

    // Importing nodes
    for (const auto nodeValue : nodes) {
        importTerm(nodeValue.toObject());
    }

    updateGroupUuidNameMaps();
    notifier.showInfo(groupData->name + " синхронизировано");
    emit groupAdded();
}

void GroupsManager::importTerm(const QJsonObject& nodeJson)
{
    if (auto data = TermData::create(nodeJson, TermData::JsonCheckMode::Import)) {
        auto addResult = dataSource.addTerm(*data);
        if (!addResult) {
            // If can't add, try to update exist term
            if (addResult.error() == DbErrorCodes::TermUuidAlreadyExist) {
                auto updateResult = dataSource.updateTerm(*data, DataStorageInterface::LastEditSource::TakeFromTermData);
                if (!updateResult) {
                    qWarning() << QString::fromStdString(updateResult.error().message());
                }
            } else {
                qWarning() << QString::fromStdString(addResult.error().message());
            }
        }
    } else {
        qWarning("Can't create TermData on import");
    }
}

int GroupsManager::dbVersion() { return dataSource.storageVersion(); }

bool GroupsManager::addNode(QJsonObject object)
{
    auto data = TermData::create(object, TermData::JsonCheckMode::AddTerm);
    assert(data.has_value());

    assert(!data->isNull());

    if (auto addResult = dataSource.addTerm(*data)) {
        emit nodeChanged();
        return true;
    } else {
        if (addResult.error() == DbErrorCodes::GroupUuidNotFound)
            notifier.showError("Группа " + data->groupUuid.toString() + " не найдена");

        if (addResult.error() == DbErrorCodes::TermAlreadyExist)
            notifier.showError("Термин с таким названием уже существует в этой группе");
    }

    return false;
}

bool GroupsManager::updateNode(const QJsonObject& object)
{
    auto data = TermData::create(object, TermData::JsonCheckMode::UpdateTerm);

    assert(data.has_value());
    assert(data->uuid);

    if (!groupExist((*data).groupUuid)) {
        notifier.showError("Группа " + (*data).groupUuid.toString() + " не найдена");
        return false;
    }

    if (!((*data).uuid)) {
        notifier.showWarning("Пустой uuid термина при попытке изменения");
        return false;
    }

    // Check for already existing node with same name
    auto alterNode = dataSource.getTerm((*data).term, (*data).groupUuid).result();
    if (alterNode.has_value() && alterNode.value().uuid != (*data).uuid) {
        notifier.showWarning("Термин с таким названием уже существует в этой группе");
        return false;
    }

    dataSource.updateTerm((*data), DataStorageInterface::LastEditSource::AutoGenerate, false);

    emit nodeChanged();
    return true;
}

void GroupsManager::deleteNode(const QUuid uuid)
{
    if (auto tUuid = TermUuid::create(uuid)) {
        dataSource.deleteTerm(*tUuid);
        emit nodeChanged();
    }
}

QString GroupsManager::getExportPath() const { return AppSettings::StdPaths::groupsJsonFolder(); }

void GroupsManager::exportGrpToJson(const QString& groupUuid)
{
    saveGroupInFolder(createGroup(QUuid(groupUuid)));

    // QJsonDocument document = group->getJsonDoc();
    // QClipboard* clipboard = qApp->clipboard();
    // clipboard->setText(document.toJson());
}

void GroupsManager::saveGroupInFolder(TermGroup::OptPtr group)
{
    if (group) {
        //        QString fileName = group->getName() + " " + group->getUuid().toString() + ".grp";
        //        FSWorks::saveFile(AppConfig::StdFolderPaths::groupsJsonFolder(), fileName, group->getJsonDoc().toJson());
    }
}

bool GroupsManager::groupExist(const GroupUuid& uuid) { return dataSource.getGroup(uuid).result().has_value(); }

bool GroupsManager::termExist(const QString& term, QUuid& groupUuid)
{
    assert(!term.isEmpty());
    assert(!groupUuid.isNull());
    auto uuid = GroupUuid::create(groupUuid).value();

    return dataSource.getTerm(term, uuid).result().has_value();
}

QJsonDocument GroupsManager::getGroupForExport(const QUuid& groupUuid) const
{
    assert(!groupUuid.isNull());
    auto uuid = GroupUuid::create(groupUuid).value();

    QJsonObject groupJson = dataSource.getGroup(uuid).result().value();

    auto nodesUuids = dataSource.getAllTermsUuids(uuid).result().value();

    QJsonArray termArray;

    for (const auto& nodeUuid : nodesUuids) {
        auto term = dataSource.getTerm(nodeUuid).result().value();
        termArray.append(static_cast<QJsonObject>(term));
    }

    groupJson.insert("nodesList", termArray);

    return QJsonDocument(groupJson);
}

void GroupsManager::updateGroupUuidNameMaps()
{
    uuidToNames.clear();

    auto groups = dataSource.getGroups().result();

    if (groups.has_value()) {
        for (const auto& groupInfo : groups.value()) {
            uuidToNames.insert(*groupInfo.uuid, groupInfo.name);
        }
    }
}
