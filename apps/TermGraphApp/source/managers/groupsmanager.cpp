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
        return dataSource.getAllTermsUuids(*uuid).size();

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

    if (dataSource.addGroup(info)) {
        updateGroupUuidNameMaps();
        emit groupAdded();
    } else {
        notifier.showError("Название группы не уникально");
    }
}

void GroupsManager::deleteGroup(const QString& groupUuid)
{
    if (auto uuid = GroupUuid::create(groupUuid)) {
        if (auto delResult = dataSource.deleteGroup(*uuid)) {
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
    for (auto& nodeUuid : dataSource.getAllTermsUuids(uuid)) {
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
    UuidList ret;

    auto groupsUuids = dataSource.getAllGroupsUuids(true).result();

    if (!groupsUuids.has_value())
        return {};

    for (auto i : groupsUuids.value())
        qDebug() << i;

    for (const auto& uuid : groupsUuids.value())
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

    if (!GroupJsonValidator::importChecks().check(jsonGroup))
        return;

    auto optGroup = GroupData::create(jsonGroup);

    if (!optGroup.has_value())
        return;

    auto info = optGroup.value();

    QJsonArray nodes = jsonGroup["nodesList"].toArray();

    // Searching for existed group
    if (!dataSource.groupExist(GroupUuid::create(info.uuid).value())) { // Group found
        dataSource.addGroup(info);
    } else {
        dataSource.updateGroup(info);
    }

    // Importing nodes
    for (const auto nodeValue : nodes) {
        importTerm(nodeValue.toObject());
    }

    updateGroupUuidNameMaps();
    notifier.showInfo(info.name + " синхронизировано");
    emit groupAdded();
}

void GroupsManager::importTerm(const QJsonObject& nodeJson)
{
    auto optInfo = TermData::create(nodeJson, TermData::JsonCheckMode::Import);
    assert(optInfo.has_value());

    auto info = optInfo.value();

    // TODO: Return error
    if (info.uuid.isNull())
        return;

    // TODO: Return error
    if (info.groupUuid.isNull())
        return;

    // Create
    if (!dataSource.termExist(TermUuid::create(info.uuid).value())) {
        dataSource.addTerm(info);
    } else {
        dataSource.updateTerm(info, DataStorageInterface::LastEditSource::TakeFromTermData);
    }
}

int GroupsManager::dbVersion() { return dataSource.storageVersion(); }

bool GroupsManager::addNode(QJsonObject object)
{
    object.insert("uuid", dataSource.getFreeUuid().toString());

    auto optData = TermData::create(object, TermData::JsonCheckMode::AddNode);
    assert(optData.has_value());

    auto data = optData.value();

    assert(!data.isNull());
    assert(!data.groupUuid.isNull());

    if (!groupExist(GroupUuid::create(data.groupUuid).value())) {
        notifier.showError("Группа " + data.groupUuid.toString() + " не найдена");
        return false;
    }

    if (termExist(data.term, data.groupUuid)) {
        notifier.showError("Термин с таким названием уже существует в этой группе");
        return false;
    }

    dataSource.addTerm(data);

    emit nodeChanged();
    return true;
}

bool GroupsManager::updateNode(const QJsonObject& object)
{
    auto optData = TermData::create(object, TermData::JsonCheckMode::UpdateNode);

    assert(optData.has_value());

    auto data = optData.value();

    assert(!data.uuid.isNull());
    assert(!data.groupUuid.isNull());

    if (!groupExist(GroupUuid::create(data.groupUuid).value())) {
        notifier.showError("Группа " + data.groupUuid.toString() + " не найдена");
        return false;
    }

    if (data.uuid.isNull()) {
        notifier.showWarning("Пустой uuid термина при попытке изменения");
        return false;
    }

    // Check for already existing node with same name
    auto alterNodeUuid = dataSource.findTerm(data.term, *GroupUuid::create(data.groupUuid));
    if (alterNodeUuid.has_value() && alterNodeUuid.value() != data.uuid) {
        notifier.showWarning("Термин с таким названием уже существует в этой группе");
        return false;
    }

    dataSource.updateTerm(data, DataStorageInterface::LastEditSource::AutoGenerate, false);

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

bool GroupsManager::groupExist(const GroupUuid& uuid) { return dataSource.groupExist(uuid); }

bool GroupsManager::termExist(const QString& term, QUuid& groupUuid)
{
    assert(!term.isEmpty());
    assert(!groupUuid.isNull());
    auto uuid = GroupUuid::create(groupUuid).value();

    return dataSource.findTerm(term, uuid).has_value();
}

QJsonDocument GroupsManager::getGroupForExport(const QUuid& groupUuid) const
{
    assert(!groupUuid.isNull());
    auto uuid = GroupUuid::create(groupUuid).value();

    QJsonObject groupJson = dataSource.getGroup(uuid).result().value();

    auto nodesUuids = dataSource.getAllTermsUuids(uuid);

    QJsonArray termArray;

    for (const auto& nodeUuid : nodesUuids) {
        auto term = dataSource.getTerm(nodeUuid).value();
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
            uuidToNames.insert(groupInfo.uuid, groupInfo.name);
        }
    }
}
