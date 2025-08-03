// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/managers/groupsmanager.h"

#include <QFile>
#include <QJsonArray>
#include <QUrl>

#include <QDebug>

import AppSettings;
import CommonTools.JsonTools;
import TermDataInterface.GroupValidator;
import TermGroup;

GroupsManager::GroupsManager(NotifyInterface& notifier, QObject* parent)
    : QObject(parent)
    , notifier(notifier)
{
    connect(this, &GroupsManager::groupAdded, this, &GroupsManager::groupsListChanged);
    connect(this, &GroupsManager::groupDeleted, this, &GroupsManager::groupsListChanged);
    connect(this, &GroupsManager::termChanged, this, &GroupsManager::groupsListChanged);

    connect(&provider, &DataProvider::groupListLoaded, this, &GroupsManager::groupListLoaded);
    connect(&provider, &DataProvider::groupLoaded, this, &GroupsManager::groupLoaded);
    connect(&provider, &DataProvider::groupListLoaded, this, &GroupsManager::groupsListChanged);
    connect(&provider, &DataProvider::showError, this, &GroupsManager::showError);

    connect(&provider, &DataProvider::groupAdded, this, &GroupsManager::groupAdded);
    connect(&provider, &DataProvider::groupUpdated, this, &GroupsManager::groupUpdated);
    connect(&provider, &DataProvider::groupDeleted, this, &GroupsManager::groupDeleted);

    connect(&provider, &DataProvider::termAdded, this, &GroupsManager::termChanged);
    connect(&provider, &DataProvider::termUpdated, this, &GroupsManager::termChanged);
    connect(&provider, &DataProvider::termDeleted, this, &GroupsManager::termChanged);

    connect(&provider, &DataProvider::termAdded, this, &GroupsManager::termAdded);
    connect(&provider, &DataProvider::termUpdated, this, &GroupsManager::termUpdated);
    connect(&provider, &DataProvider::termDeleted, this, &GroupsManager::termDeleted);

    connect(&provider, &DataProvider::exportGroupReady, this, &GroupsManager::exportGroupReady);
}

QString GroupsManager::getGroupName(const QUuid& groupUuid) const
{
    if (auto gUuid = GroupUuid::from(groupUuid)) {
        for (const auto& group : provider.groups()) {
            if (group.uuid == gUuid) {
                return group.name;
            }
        }
    }

    return {};
}

QString GroupsManager::getGroupUuid(const QString& groupName)
{
    for (const auto& group : provider.groups()) {
        if (group.name == groupName) {
            if (group.uuid.has_value())
                return group.uuid.value().toString();
        }
    }

    return "";
}

QString GroupsManager::getLastEditString(QUuid groupUuid) { return getLastEdit(groupUuid).toString(); }

int GroupsManager::getNodesCount(QUuid groupUuid)
{
    if (auto uuid = GroupUuid::from(groupUuid)) {
        if (auto group = provider.group(*uuid)) {
            return group->size;
        }
    }

    return 0;
}

void GroupsManager::addNewGroup(const QString& name, const QString& comment)
{
    assert(provider.isReady());
    if (name.simplified().isEmpty()) {
        notifier.showError("Название группы не может быть пустым");
        return;
    }

    GroupSummary info;

    info.name    = name;
    info.comment = comment;

    provider.addGroup(info);
}

void GroupsManager::deleteGroup(const QString& groupUuid)
{
    assert(provider.isReady());
    if (auto uuid = GroupUuid::from(groupUuid)) {
        provider.deleteGroup(*uuid);
    }
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
    importGroup(doc.object());
}

std::optional<std::shared_ptr<TermGroup>> GroupsManager::createGroup(std::optional<GroupUuid> uuid)
{
    std::optional<GroupSummary> groupInfo;
    if (uuid) {
        groupInfo = provider.group(*uuid);
    } else if (auto loadedUuid = provider.currentGroup()) {
        groupInfo = provider.group(*loadedUuid);
    } else {
        Q_UNREACHABLE();
        return nullptr;
    }

    auto termsData = provider.terms();
    assert(groupInfo);

    return std::make_shared<TermGroup>(*groupInfo, termsData);
}

bool GroupsManager::isEmptyGroup(const QString& groupUuid)
{
    assert(provider.isReady());
    return getNodesCount(QUuid(groupUuid)) == 0;
}

bool GroupsManager::getHasAnyGroup() const
{
    if (!provider.isReady()) {
        return false;
    }

    return !(provider.groups().empty());
}

QDateTime GroupsManager::getLastEdit(QUuid groupUuid)
{
    auto uuid = GroupUuid::from(groupUuid);
    assert(uuid.has_value());

    auto groupInfo = provider.group(*uuid);

    return groupInfo ? groupInfo->lastEdit : QDateTime();
}

GroupUuid::List GroupsManager::getAllUuidsSortedByLastEdit()
{
    GroupUuid::List ret;
    for (const auto& group : provider.groups()) {
        if (group.uuid.has_value()) {
            ret.push_back(*group.uuid);
        }
    }

    return ret;
}

QStringList GroupsManager::getAllUuidStringsSortedByLastEdit()
{
    QStringList ret;
    for (auto uuid : getAllUuidsSortedByLastEdit()) {
        ret << uuid.toString();
    }

    return ret;
}

void GroupsManager::importGroup(QJsonObject json)
{
    // Some import data fixes
    // Update of termsKey if need
    if (json.contains(jsonTools::oldTermsKey)) {
        json.insert(jsonTools::termsKey, json[jsonTools::oldTermsKey]);
        json.remove(jsonTools::oldTermsKey);
    }

    // Add size field
    if (json[jsonTools::termsKey].isArray() && !json.contains(jsonTools::sizeKey)) {
        json.insert(jsonTools::sizeKey, json[jsonTools::termsKey].toArray().size());
    }

    if (!GroupJsonValidator::fullChecks().check(json)) {
        return;
    }

    auto groupInfo = GroupSummary::from(json);

    if (!groupInfo || !groupInfo->uuid) {
        return;
    }

    // Searching for existed group
    if (provider.group(*groupInfo->uuid).has_value()) { // Group found
        provider.updateGroup(*groupInfo);
    } else {
        provider.addGroup(*groupInfo);
    }

    QJsonArray nodes = json[jsonTools::termsKey].toArray();

    // Importing nodes
    for (const auto& node : nodes) {
        importTerm(node.toObject());
    }

    notifier.showInfo(groupInfo->name + " синхронизировано");
    emit groupAdded(*groupInfo);
}

void GroupsManager::importTerm(const QJsonObject& nodeJson)
{
    if (auto data = TermData::from(nodeJson, TermData::JsonCheckMode::Import)) {
        provider.importTerm(*data);
    } else {
        qWarning("Can't create TermData on import");
    }
}

int GroupsManager::dbVersion() { return provider.dbVersion(); }

void GroupsManager::addNode(QJsonObject object)
{
    auto data = TermData::from(object, TermData::JsonCheckMode::AddTerm);
    assert(data.has_value());
    assert(!data->isNull());

    provider.addTerm(*data);
}

void GroupsManager::updateNode(const QJsonObject& object)
{
    auto data = TermData::from(object, TermData::JsonCheckMode::UpdateTerm);

    assert(data.has_value());
    assert(data->uuid);

    provider.updateTerm((*data), DataStorageInterface::LastEditSource::Now, false);
}

void GroupsManager::deleteNode(const QUuid uuid)
{
    if (auto tUuid = TermUuid::from(uuid)) {
        provider.deleteTerm(*tUuid);
    }
}

void GroupsManager::showError(int code)
{
    auto eCode = static_cast<ErrorCode>(code);
    switch (eCode) {
    case GroupUuidNotFound: notifier.showError("Группа не найдена"); break;
    case GroupNameAlreadyExist: notifier.showError("Название группы не уникально"); break;
    case TermUuidInvalid: notifier.showWarning("Пустой uuid термина при попытке изменения"); break;
    case TermAlreadyExist: notifier.showError("Термин с таким названием уже существует в этой группе"); break;
    case GroupUuidInvalid:
    case GroupUuidAlreadyExist:
    case GroupNameEmpty:
    case TermUuidAlreadyExist:
    case TermUuidNotFound:
    case TermEmpty:
    case TermNotFound:
    case LastEditInvalid:
    case NewerTermVersionFound:
    case NodesLastEditInvalid:
    case ConnectionError:
    case UnknownError:
    case JsonParseError:
    case JsonNameFieldMissedOrWrongType:
    case JsonCommentFieldMissedOrWrongType:
    case JsonSizeFieldMissedOrWrongType:
    case JsonNodesFieldMissedOrWrongType:
    case JsonNodesLastEditFieldMissedOrWrongType:
    case JsonUuidFieldMissedOrWrongType:
    case JsonTermFieldMissedOrWrongType:
    case JsonTermDefFieldWrongContentOrType:
    case JsonDefinitionFieldMissedOrWrongType:
    case JsonDescriptionFieldMissedOrWrongType:
    case JsonExamplesFieldMissedOrWrongType:
    case JsonWikiUrlFieldMissedOrWrongType:
    case JsonWikiImageFieldMissedOrWrongType:
    case JsonGroupUuidFieldMissedOrWrongType:
    case JsonLastEditFieldMissedOrWrongType: notifier.showError(Errors::toQString(eCode)); break;
    }
}

QString GroupsManager::getExportPath() const { return AppSettings::Paths::groupsJsonFolder(); }

void GroupsManager::exportGrpToJson(const QString& groupUuid)
{
    Q_UNIMPLEMENTED();
    if (auto uuid = GroupUuid::from(groupUuid)) {
        saveGroupInFolder(createGroup(*uuid));
    }

    // QJsonDocument document = group->getJsonDoc();
    // QClipboard* clipboard = qApp->clipboard();
    // clipboard->setText(document.toJson());
}

void GroupsManager::init() { provider.loadGroups(); }

void GroupsManager::loadGroup(const GroupUuid& uuid) { provider.loadGroup(uuid); }

void GroupsManager::saveGroupInFolder(std::optional<std::shared_ptr<TermGroup>> group)
{
    Q_UNIMPLEMENTED();
    if (group) {
        // QString fileName = group->getName() + " " + group->getUuid().toString() + ".grp";
        // Save file
    }
}

bool GroupsManager::groupExist(const GroupUuid& uuid) { return provider.group(uuid).has_value(); }

void GroupsManager::requestGroupExport(const QUuid& groupUuid)
{
    assert(!groupUuid.isNull());
    auto uuid = GroupUuid::from(groupUuid).value();
    provider.requestGroupExport(uuid);
}
