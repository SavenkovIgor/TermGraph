// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/managers/groupsmanager.h"

#include <QFile>
#include <QJsonArray>
#include <QUrl>

#include <QDebug>

#include "source/helpers/appconfig.h"

GroupsManager::GroupsManager(std::unique_ptr<DataStorageInterface> dataSource,
                             NotifyInterface&                      notifier,
                             QObject*                              parent)
    : QObject(parent)
    , dataSource(std::move(dataSource))
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
        return dataSource->getGroup(*uuid).result().value().size;

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

    if (dataSource->addGroup(info).result()) {
        updateGroupUuidNameMaps();
        emit groupAdded();
    } else {
        notifier.showError("Название группы не уникально");
    }
}

void GroupsManager::deleteGroup(const QString& groupUuid)
{
    if (auto uuid = GroupUuid::create(groupUuid)) {
        if (auto delResult = dataSource->deleteGroup(*uuid).result()) {
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
        auto groupData = dataSource->getGroup(*uuid).result();
        auto termsData = dataSource->getTerms(*uuid).result();

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
    auto groupsRes = dataSource->getGroups().result();
    return groupsRes.has_value() ? (!groupsRes.value().empty()) : false;
}

QDateTime GroupsManager::getLastEdit(QUuid groupUuid)
{
    assert(!groupUuid.isNull());
    auto uuid = GroupUuid::create(groupUuid).value();

    QDateTime lastEdit;
    auto      terms = dataSource->getTerms(uuid).result().value();
    for (auto& term : terms) {
        QDateTime currNodeLastEdit = term.lastEdit;
        if (lastEdit.isNull()) {
            lastEdit = currNodeLastEdit;
        } else {
            lastEdit = std::max(lastEdit, currNodeLastEdit);
        }
    }
    return lastEdit;
}

GroupUuid::List GroupsManager::getAllUuidsSortedByLastEdit()
{
    return dataSource->getAllGroupsUuids().result().value();
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
    if (dataSource->getGroup(*groupData->uuid).result().has_value()) { // Group found
        if (!dataSource->updateGroup(*groupData).result()) {
            return;
        }
    } else {
        if (!dataSource->addGroup(*groupData).result()) {
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
        auto addResult = dataSource->addTerm(*data).result();
        if (!addResult) {
            // If can't add, try to update exist term
            if (addResult.error() == DbErrorCodes::TermUuidAlreadyExist) {
                auto updateResult = dataSource->updateTerm(*data,
                                                           DataStorageInterface::LastEditSource::TakeFromTermData).result();
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

int GroupsManager::dbVersion() { return dataSource->storageVersion(); }

bool GroupsManager::addNode(QJsonObject object)
{
    auto data = TermData::create(object, TermData::JsonCheckMode::AddTerm);
    assert(data.has_value());

    assert(!data->isNull());

    if (auto addResult = dataSource->addTerm(*data).result()) {
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
    auto alterNode = dataSource->getTerm((*data).term, (*data).groupUuid);
    if (alterNode.result().has_value() && alterNode.result().value().uuid != (*data).uuid) {
        notifier.showWarning("Термин с таким названием уже существует в этой группе");
        return false;
    }

    dataSource->updateTerm((*data), DataStorageInterface::LastEditSource::AutoGenerate, false);

    emit nodeChanged();
    return true;
}

void GroupsManager::deleteNode(const QUuid uuid)
{
    if (auto tUuid = TermUuid::create(uuid)) {
        auto _ = dataSource->deleteTerm(*tUuid).result();
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

bool GroupsManager::groupExist(const GroupUuid& uuid) { return dataSource->getGroup(uuid).result().has_value(); }

QJsonDocument GroupsManager::getGroupForExport(const QUuid& groupUuid) const
{
    assert(!groupUuid.isNull());
    auto uuid = GroupUuid::create(groupUuid).value();

    QJsonObject groupJson = dataSource->getGroup(uuid).result().value();

    QJsonArray termArray;

    auto terms = dataSource->getTerms(uuid).result().value();

    for (const auto& term : terms)
        termArray.append(static_cast<QJsonObject>(term));

    groupJson.insert("nodesList", termArray);

    return QJsonDocument(groupJson);
}

void GroupsManager::updateGroupUuidNameMaps()
{
    uuidToNames.clear();

    if (auto groups = dataSource->getGroups().result()) {
        for (const auto& groupInfo : groups.value()) {
            uuidToNames.insert(*groupInfo.uuid, groupInfo.name);
        }
    }
}
