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

#include "source/helpers/appconfig.h"

GroupsManager::GroupsManager(DataStorageInterface& dataStorage, QObject* parent)
    : QObject(parent)
    , dataStorage(dataStorage)
{
    connect(this, &GroupsManager::groupAdded, this, &GroupsManager::groupsListChanged);
    connect(this, &GroupsManager::groupDeleted, this, &GroupsManager::groupsListChanged);
    connect(this, &GroupsManager::nodeChanged, this, &GroupsManager::groupsListChanged);
}

QString GroupsManager::getGroupName(const QUuid& groupUuid) const
{
    if (uuidToNames.contains(groupUuid)) {
        return uuidToNames[groupUuid];
    }

    return "";
}

QUuid GroupsManager::getGroupUuid(const QString& groupName) const
{
    if (namesToUuid.contains(groupName)) {
        return namesToUuid[groupName];
    }

    return QUuid();
}

QString GroupsManager::getLastEditString(QUuid groupUuid) { return getLastEdit(groupUuid).toString(); }

int GroupsManager::getNodesCount(QUuid groupUuid) { return dataStorage.getAllTermsUuids(groupUuid).size(); }

void GroupsManager::addNewGroup(const QString& name, const QString& comment)
{
    if (name.simplified().isEmpty()) {
        NotificationManager::showError("Название группы не может быть пустым");
        return;
    }

    GroupData info;

    info.name    = name;
    info.comment = comment;

    if (dataStorage.addGroup(info)) {
        updateGroupUuidNameMaps();
        emit groupAdded();
    } else {
        NotificationManager::showError("Название группы не уникально");
    }
}

void GroupsManager::deleteGroup(const QString& groupUuid)
{
    dataStorage.deleteGroup(QUuid(groupUuid));
    updateGroupUuidNameMaps();
    emit groupDeleted();
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

bool GroupsManager::isValidGroupJson(const QJsonObject json)
{
    return GroupData::isValidJson(json) && json["nodesList"].isArray();
}

TermGroup::OptPtr GroupsManager::createGroup(const QUuid groupUuid)
{
    if (groupUuid.isNull())
        return std::nullopt;

    auto groupData = dataStorage.getGroup(groupUuid);
    auto termsData = dataStorage.getTerms(groupUuid);

    return std::make_shared<TermGroup>(groupData, termsData);
}

bool GroupsManager::isEmptyGroup(const QString& groupUuid)
{
    QUuid uuid(groupUuid);

    if (uuid.isNull())
        return true;

    return getNodesCount(uuid) == 0;
}

bool GroupsManager::getHasAnyGroup() const { return !dataStorage.getAllGroupsUuids().empty(); }

QDateTime GroupsManager::getLastEdit(QUuid groupUuid)
{
    QDateTime lastEdit;
    for (auto& nodeUuid : dataStorage.getAllTermsUuids(groupUuid)) {
        QDateTime currNodeLastEdit = dataStorage.getTermLastEdit(nodeUuid);
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
    UuidList ret;

    auto groupsUuids = dataStorage.getAllGroupsUuids(true);

    for (const auto& uuid : groupsUuids)
        ret.push_back(uuid);

    return ret;
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

    if (!isValidGroupJson(jsonGroup))
        return;

    auto optGroup = GroupData::fromJson(jsonGroup);

    if (!optGroup.has_value())
        return;

    auto info = optGroup.value();

    QJsonArray nodes = jsonGroup["nodesList"].toArray();

    // Searching for existed group
    if (!dataStorage.groupExist(info.uuid)) { // Group found
        dataStorage.addGroup(info);
    } else {
        dataStorage.updateGroup(info);
    }

    // Importing nodes
    for (const auto nodeValue : nodes) {
        importTerm(nodeValue.toObject());
    }

    updateGroupUuidNameMaps();
    NotificationManager::showInfo(info.name + " синхронизировано");
    emit groupAdded();
}

void GroupsManager::importTerm(const QJsonObject& nodeJson)
{
    auto optInfo = TermData::fromJson(nodeJson);
    assert(optInfo.has_value());

    auto info = optInfo.value();

    if (info.uuid.isNull())
        return;

    if (info.groupUuid.isNull())
        return;

    // Create
    if (!dataStorage.termExist(info.uuid)) {
        dataStorage.addTerm(info);
    } else {
        dataStorage.updateTerm(info, DataStorageInterface::LastEditSource::TakeFromTermData);
    }
}

int GroupsManager::dbVersion() { return dataStorage.storageVersion(); }

bool GroupsManager::addNode(const QJsonObject& object)
{
    auto optData = TermData::fromJson(object);

    assert(optData.has_value());

    auto data = optData.value();

    assert(!data.groupUuid.isNull());

    if (!groupExist(data.groupUuid)) {
        NotificationManager::showError("Группа " + data.groupUuid.toString() + " не найдена");
        return false;
    }

    if (termExist(data.term, data.groupUuid)) {
        NotificationManager::showError("Термин с таким названием уже существует в этой группе");
        return false;
    }

    dataStorage.addTerm(data);

    emit nodeChanged();
    return true;
}

bool GroupsManager::updateNode(const QJsonObject& object)
{
    auto optData = TermData::fromJson(object);

    assert(optData.has_value());

    auto data = optData.value();

    assert(!data.uuid.isNull());
    assert(!data.groupUuid.isNull());

    if (!groupExist(data.groupUuid)) {
        NotificationManager::showError("Группа " + data.groupUuid.toString() + " не найдена");
        return false;
    }

    if (data.uuid.isNull()) {
        NotificationManager::showWarning("Пустой uuid термина при попытке изменения");
        return false;
    }

    // Check for already existing node with same name
    auto alterNodeUuid = dataStorage.findTerm(data.term, data.groupUuid);
    if (!alterNodeUuid.isNull() && alterNodeUuid != data.uuid) {
        NotificationManager::showWarning("Термин с таким названием уже существует в этой группе");
        return false;
    }

    dataStorage.updateTerm(data, DataStorageInterface::LastEditSource::AutoGenerate, false);

    emit nodeChanged();
    return true;
}

void GroupsManager::deleteNode(const QUuid uuid)
{
    dataStorage.deleteTerm(uuid);
    emit nodeChanged();
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

bool GroupsManager::groupExist(const QUuid& groupUuid)
{
    assert(!groupUuid.isNull());
    return dataStorage.groupExist(groupUuid);
}

bool GroupsManager::termExist(const QString& term, QUuid& groupUuid)
{
    assert(!term.isEmpty());
    assert(!groupUuid.isNull());

    return !dataStorage.findTerm(term, groupUuid).isNull();
}

QJsonDocument GroupsManager::getGroupForExport(const QUuid& groupUuid) const
{
    auto info      = dataStorage.getGroup(groupUuid);
    auto groupJson = info.toJson();

    auto nodesUuids = dataStorage.getAllTermsUuids(groupUuid);

    QJsonArray nodesArray;

    for (const auto& nodeUuid : nodesUuids) {
        auto nodeJson = dataStorage.getTerm(nodeUuid).toJson();
        nodesArray.append(nodeJson);
    }

    groupJson.insert("nodesList", nodesArray);

    return QJsonDocument(groupJson);
}

void GroupsManager::updateGroupUuidNameMaps()
{
    qInfo("List updated");

    uuidToNames.clear();
    namesToUuid.clear();

    for (const auto& groupInfo : dataStorage.getGroups()) {
        uuidToNames.insert(groupInfo.uuid, groupInfo.name);
        namesToUuid.insert(groupInfo.name, groupInfo.uuid);
    }
}
