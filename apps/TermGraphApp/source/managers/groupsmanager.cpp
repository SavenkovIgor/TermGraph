// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/managers/groupsmanager.h"

#include <QFile>
#include <QJsonArray>
#include <QUrl>

#include <QDebug>

#include "source/helpers/appsettings.h"

GroupsManager::GroupsManager(NotifyInterface&                      notifier,
                             QObject*                              parent)
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
}

QString GroupsManager::getGroupName(const QUuid& groupUuid) const {
    if (auto gUuid = GroupUuid::create(groupUuid)) {
        for (const auto& groupData : provider.groups()) {
            if (groupData.uuid == gUuid) {
                return groupData.name;
            }
        }
    }

    return {};
}

QString GroupsManager::getLastEditString(QUuid groupUuid) { return getLastEdit(groupUuid).toString(); }

int GroupsManager::getNodesCount(QUuid groupUuid)
{
    if (auto uuid = GroupUuid::create(groupUuid)) {
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

    GroupData info;

    info.name    = name;
    info.comment = comment;

    provider.addGroup(info);
}

void GroupsManager::deleteGroup(const QString& groupUuid)
{
    assert(provider.isReady());
    if (auto uuid = GroupUuid::create(groupUuid))
        provider.deleteGroup(*uuid);
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

TermGroup::OptPtr GroupsManager::createGroup(Opt<GroupUuid> uuid)
{
    Opt<GroupData> groupData;
    if (uuid) {
        groupData = provider.group(*uuid);
    } else if (auto loadedUuid = provider.currentGroup()) {
        groupData = provider.group(*loadedUuid);
    } else {
        Q_UNREACHABLE();
        return nullptr;
    }

    auto termsData = provider.terms();
    assert(groupData);

    return std::make_shared<TermGroup>(*groupData, termsData);
}

bool GroupsManager::isEmptyGroup(const QString& groupUuid) {
    assert(provider.isReady());
    return getNodesCount(QUuid(groupUuid)) == 0;
}

bool GroupsManager::getHasAnyGroup() const
{
    if (!provider.isReady())
        return false;

    return !(provider.groups().empty());
}

QDateTime GroupsManager::getLastEdit(QUuid groupUuid)
{
    auto uuid = GroupUuid::create(groupUuid);
    assert(uuid.has_value());

    auto groupData = provider.group(*uuid);

    return groupData ? groupData->lastEdit : QDateTime();
}

GroupUuid::List GroupsManager::getAllUuidsSortedByLastEdit()
{
    GroupUuid::List ret;
    for (const auto& group : provider.groups())
        if (group.uuid.has_value())
            ret.push_back(*group.uuid);

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
    Q_UNIMPLEMENTED();
//    QJsonObject jsonGroup = json.object();

//    if (!GroupJsonValidator::importChecks().check(jsonGroup))
//        return;

//    auto groupData = GroupData::create(jsonGroup);

//    if (!groupData || !groupData->uuid)
//        return;

//    // Searching for existed group
//    if (dataSource->getGroup(*groupData->uuid).get().has_value()) { // Group found
//        if (!dataSource->updateGroup(*groupData).get()) {
//            return;
//        }
//    } else {
//        if (!dataSource->addGroup(*groupData).get()) {
//            return;
//        }
//    }

//    QJsonArray nodes = jsonGroup["nodesList"].toArray();

//    // Importing nodes
//    for (const auto nodeValue : nodes) {
//        importTerm(nodeValue.toObject());
//    }

//    notifier.showInfo(groupData->name + " синхронизировано");
//    emit groupAdded();
}

void GroupsManager::importTerm(const QJsonObject& nodeJson)
{
    Q_UNIMPLEMENTED();
//    if (auto data = TermData::create(nodeJson, TermData::JsonCheckMode::Import)) {
//        auto addResult = dataSource->addTerm(*data).get();
//        if (!addResult) {
//            // If can't add, try to update exist term
//            if (addResult.error() == DbErrorCodes::TermUuidAlreadyExist) {
//                auto updateResult = dataSource->updateTerm(*data,
//                                                           DataStorageInterface::LastEditSource::FromData).get();
//                if (!updateResult) {
//                    qWarning() << Errors::toQString(updateResult.error().value());
//                }
//            } else {
//                qWarning() << Errors::toQString(addResult.error().value());
//            }
//        }
//    } else {
//        qWarning("Can't create TermData on import");
//    }
}

int GroupsManager::dbVersion() {
    return provider.dbVersion();
}

void GroupsManager::addNode(QJsonObject object)
{
    auto data = TermData::create(object, TermData::JsonCheckMode::AddTerm);
    assert(data.has_value());
    assert(!data->isNull());

    provider.addTerm(*data);
}

void GroupsManager::updateNode(const QJsonObject& object)
{
    auto data = TermData::create(object, TermData::JsonCheckMode::UpdateTerm);

    assert(data.has_value());
    assert(data->uuid);

    provider.updateTerm((*data), DataStorageInterface::LastEditSource::Now, false);
}

void GroupsManager::deleteNode(const QUuid uuid)
{
    if (auto tUuid = TermUuid::create(uuid))
        provider.deleteTerm(*tUuid);
}

void GroupsManager::showError(int code) {
    auto eCode = static_cast<ErrorCodes>(code);
    switch (eCode) {
    case GroupUuidNotFound:
        notifier.showError("Группа не найдена");
        break;
    case GroupNameAlreadyExist:
        notifier.showError("Название группы не уникально");
        break;
    case TermUuidInvalid:
        notifier.showWarning("Пустой uuid термина при попытке изменения");
        break;
    case TermAlreadyExist:
        notifier.showError("Термин с таким названием уже существует в этой группе");
        break;
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
    case JsonDefinitionFieldMissedOrWrongType:
    case JsonDescriptionFieldMissedOrWrongType:
    case JsonExamplesFieldMissedOrWrongType:
    case JsonWikiUrlFieldMissedOrWrongType:
    case JsonWikiImageFieldMissedOrWrongType:
    case JsonGroupUuidFieldMissedOrWrongType:
    case JsonLastEditFieldMissedOrWrongType:
        notifier.showError(Errors::toQString(eCode));
        break;
    }
}

QString GroupsManager::getExportPath() const { return AppSettings::Paths::groupsJsonFolder(); }

void GroupsManager::exportGrpToJson(const QString& groupUuid)
{
    Q_UNIMPLEMENTED();
    if (auto uuid = GroupUuid::create(groupUuid))
        saveGroupInFolder(createGroup(*uuid));

    // QJsonDocument document = group->getJsonDoc();
    // QClipboard* clipboard = qApp->clipboard();
    // clipboard->setText(document.toJson());
}

void GroupsManager::init()
{
    provider.loadGroups();
}

void GroupsManager::loadGroup(const GroupUuid& uuid)
{
    provider.loadGroup(uuid);
}

void GroupsManager::saveGroupInFolder(TermGroup::OptPtr group)
{
    Q_UNIMPLEMENTED();
    if (group) {
        // QString fileName = group->getName() + " " + group->getUuid().toString() + ".grp";
        // Save file
    }
}

bool GroupsManager::groupExist(const GroupUuid& uuid) { return provider.group(uuid).has_value(); }

QJsonDocument GroupsManager::getGroupForExport(const QUuid& groupUuid) const
{
    Q_UNIMPLEMENTED();
//    assert(!groupUuid.isNull());
//    auto uuid = GroupUuid::create(groupUuid).value();

//    QJsonObject groupJson = dataSource->getGroup(uuid).get().value();

//    QJsonArray termArray;

//    auto terms = dataSource->getTerms(uuid).get().value();

//    for (const auto& term : terms)
//        termArray.append(static_cast<QJsonObject>(term));

//    groupJson.insert("nodesList", termArray);

//    return QJsonDocument(groupJson);
}
