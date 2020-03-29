/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

#include "source/Managers/groupsmanager.h"

#include <QFile>
#include <QJsonArray>
#include <QUrl>

#include "source/Helpers/appconfig.h"
#include "source/Managers/jsongroupinfocontainerparser.h"

GroupsManager::GroupsManager(DataStorageInterface& dataStorage, NodesManager* nodesMgr, QObject* parent)
    : QObject(parent)
    , dataStorage(dataStorage)
{
    this->nodesMgr = nodesMgr;
    connect(nodesMgr, &NodesManager::nodeChanged, this, &GroupsManager::groupsListChanged);
}

QList<TermGroup*> GroupsManager::getAllGroups()
{
    QList<TermGroup*> ret;
    for (QUuid groupUuid : getAllUuidsSortedByLastEdit()) {
        TermGroup* group = createGroup(groupUuid);

        if (group == nullptr) {
            auto errorMsg = QString("Cant create group with Uuid: %1").arg(groupUuid.toString());
            NotificationManager::showDebug(errorMsg);
            continue;
        }

        ret << group;
    }
    return ret;
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

QString GroupsManager::getLastEditString(QUuid groupUuid)
{
    return getLastEdit(groupUuid).toString();
}

int GroupsManager::getNodesCount(QUuid groupUuid)
{
    return nodesMgr->getAllNodesUuidsInGroup(groupUuid).size();
}

QStringList GroupsManager::getGroupNames(const QList<QUuid>& groupUuids)
{
    QStringList ret;
    for (QUuid uuid : groupUuids) {
        ret << getGroupName(uuid);
    }
    return ret;
}

void GroupsManager::addNewGroup(const QString& name, const QString& comment)
{
    if (name.simplified().isEmpty()) {
        NotificationManager::showError("Название группы не может быть пустым");
        return;
    }

    GroupInfoContainer info;

    info.name    = name;
    info.comment = comment;

    if (dataStorage.addGroup(info)) {
        updateGroupUuidNameMaps();
        emit groupsListChanged();
        emit groupAdded();
    } else {
        NotificationManager::showError("Название группы не уникально");
    }
}

void GroupsManager::deleteGroup(const QString& groupUuid)
{
    dataStorage.deleteGroup(QUuid(groupUuid));
    updateGroupUuidNameMaps();
    emit groupsListChanged();
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
    QByteArray byteArr;
    byteArr.append(rawJson);
    QJsonDocument doc = QJsonDocument::fromJson(byteArr);
    importGroupFromJson(doc);
}

bool GroupsManager::isValidGroupJson(const QJsonDocument json)  // TODO: Rework!
{
    QJsonObject jsonGroup = json.object();

    // Checking keys
    if (jsonGroup.contains("name") && jsonGroup.value("name").isString() && jsonGroup.contains("nodesList")
        && jsonGroup.value("nodesList").isArray()) {
        return true;
    }
    return false;
}

TermGroup* GroupsManager::createGroup(const QString& groupName)
{
    return createGroup(getGroupUuid(groupName));
}

TermGroup* GroupsManager::createGroup(const QUuid groupUuid)
{
    if (groupUuid.isNull())
        return nullptr;

    auto       info  = dataStorage.getGroup(groupUuid);
    TermGroup* group = new TermGroup(info);
    group->loadNodes(nodesMgr->getAllNodesForGroup(groupUuid));
    return group;
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
    return !dataStorage.getAllGroupsUuids().empty();
}

QDateTime GroupsManager::getLastEdit(QUuid groupUuid)
{
    QDateTime lastEdit;
    for (auto& nodeUuid : nodesMgr->getAllNodesUuidsInGroup(groupUuid)) {
        QDateTime currNodeLastEdit = nodesMgr->getLastEdit(nodeUuid);
        if (lastEdit.isNull()) {
            lastEdit = currNodeLastEdit;
        } else {
            lastEdit = std::max(lastEdit, currNodeLastEdit);
        }
    }
    return lastEdit;
}

QList<QUuid> GroupsManager::getAllUuidsSortedByLastEdit()
{
    QList<QUuid> ret;

    auto groupsUuids = dataStorage.getAllGroupsUuids(true);

    for (const auto& uuid : groupsUuids)
        ret << uuid;

    return ret;
}

QUuid GroupsManager::getLastEditedGroupUuid()
{
    auto allGroupsUuids = getAllUuidsSortedByLastEdit();
    if (!allGroupsUuids.isEmpty())
        return allGroupsUuids.first();

    return QUuid();
}

QStringList GroupsManager::getAllUuidStringsSortedByLastEdit()
{
    QStringList ret;
    for (auto uuid : getAllUuidsSortedByLastEdit())
        ret << uuid.toString();

    return ret;
}

void GroupsManager::importGroupFromJson(const QJsonDocument& json)
{
    if (!isValidGroupJson(json))
        return;

    QJsonObject jsonGroup = json.object();

    auto info = JsonGroupInfoContainerParser::fromJson(jsonGroup);

    if (info.uuid.isNull())
        return;

    QJsonArray nodes = jsonGroup.value("nodesList").toArray();

    // Searching for existed group
    if (!dataStorage.groupExist(info.uuid)) {  // Group found
        dataStorage.addGroup(info);
    } else {
        dataStorage.updateGroup(info);
    }

    // Importing nodes
    for (const auto nodeValue : nodes) {
        nodesMgr->importNodeFromJson(nodeValue.toObject());
    }

    updateGroupUuidNameMaps();
    NotificationManager::showInfo(info.name + " синхронизировано");
    emit groupsListChanged();
    emit groupAdded();
}

QString GroupsManager::getExportPath() const
{
    return AppSettings::StdPaths::groupsJsonFolder();
}

void GroupsManager::exportGrpToJson(const QString& groupUuid)
{
    if (auto group = createGroup(QUuid(groupUuid))) {
        saveGroupInFolder(group);

        //            QJsonDocument document = group->getJsonDoc();
        //            QClipboard* clipboard = qApp->clipboard();
        //            clipboard->setText(document.toJson());

        delete group;
    }
}

void GroupsManager::saveGroupInFolder(TermGroup* group)
{
    if (group != nullptr) {
        //        QString fileName = group->getName() + " " + group->getUuid().toString() + ".grp";
        //        FSWorks::saveFile(AppConfig::StdFolderPaths::groupsJsonFolder(), fileName, group->getJsonDoc().toJson());
    }
}

QJsonDocument GroupsManager::getGroupForExport(const QUuid& groupUuid)
{
    auto info      = dataStorage.getGroup(groupUuid);
    auto groupJson = JsonGroupInfoContainerParser::toJson(info);

    auto nodesUuids = dataStorage.getAllNodesUuids(groupUuid);

    QJsonArray nodesArray;

    for (const auto& nodeUuid : nodesUuids) {
        auto nodeJson = nodesMgr->getNodeJson(nodeUuid);
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
