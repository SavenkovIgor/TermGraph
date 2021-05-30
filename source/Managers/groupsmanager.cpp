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

#include "source/Managers/groupsmanager.h"

#include <QFile>
#include <QJsonArray>
#include <QUrl>

#include "source/Helpers/appconfig.h"
#include "source/Managers/jsongroupdataparser.h"
#include "source/Managers/jsontermdataparser.h"

GroupsManager::GroupsManager(DataStorageInterface& dataStorage, QObject* parent)
    : QObject(parent)
    , dataStorage(dataStorage)
{
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
    QJsonDocument doc = QJsonDocument::fromJson(rawJson.toUtf8());
    importGroup(doc);
}

bool GroupsManager::isValidGroupJson(const QJsonDocument json) // TODO: Rework!
{
    QJsonObject jsonGroup = json.object();

    // Checking keys
    if (jsonGroup.contains("name") && jsonGroup.value("name").isString() && jsonGroup.contains("nodesList")
        && jsonGroup.value("nodesList").isArray()) {
        return true;
    }
    return false;
}

TermGroup* GroupsManager::createGroup(const QUuid groupUuid)
{
    if (groupUuid.isNull())
        return nullptr;

    auto info  = dataStorage.getGroup(groupUuid);
    auto terms = dataStorage.getTerms(groupUuid);

    PaintedTerm::List sceneTerms;

    for (const auto& term : terms)
        sceneTerms << new PaintedTerm(term);

    return new TermGroup(info, sceneTerms);
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

QList<QUuid> GroupsManager::getAllUuidsSortedByLastEdit()
{
    QList<QUuid> ret;

    auto groupsUuids = dataStorage.getAllGroupsUuids(true);

    for (const auto& uuid : groupsUuids)
        ret << uuid;

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
    if (!isValidGroupJson(json))
        return;

    QJsonObject jsonGroup = json.object();

    auto info = JsonGroupDataParser::fromJson(jsonGroup);

    if (info.uuid.isNull())
        return;

    QJsonArray nodes = jsonGroup.value("nodesList").toArray();

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
    emit groupsListChanged();
    emit groupAdded();
}

void GroupsManager::importTerm(const QJsonObject& nodeJson)
{
    auto info = JsonTermDataParser::fromJson(nodeJson);

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
    assert(JsonTermDataParser::isValidKeysAndTypes(object));
    auto data = JsonTermDataParser::fromJson(object);

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
    assert(JsonTermDataParser::isValidKeysAndTypes(object));
    auto data = JsonTermDataParser::fromJson(object);

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
    auto groupJson = JsonGroupDataParser::toJson(info);

    auto nodesUuids = dataStorage.getAllTermsUuids(groupUuid);

    QJsonArray nodesArray;

    for (const auto& nodeUuid : nodesUuids) {
        auto nodeJson = JsonTermDataParser::toJson(dataStorage.getTerm(nodeUuid));
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
