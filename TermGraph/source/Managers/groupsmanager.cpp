#include "groupsmanager.h"

#include "source/databaseWorks/columns/termgroupcolumn.h"
#include "source/databaseWorks/columns/nodecolumn.h"
#include "source/Managers/jsongroupinfocontainerparser.h"

GroupsManager::GroupsManager(NodesManager *nodesMgr, QObject *parent)
    : QObject(parent)
{
    this->nodesMgr = nodesMgr;
    connect(nodesMgr, &NodesManager::nodeChanged, this, &GroupsManager::groupsListChanged);
}

QStringList GroupsManager::getAllGroupsNames(bool withAllVeiw)
{
    QList<QUuid> allUuids = getAllUuidsSortedByLastEdit();
    QStringList groupNames = getGroupNames(allUuids);

    if (withAllVeiw) {
        groupNames.push_back("Все группы");
    }

    return groupNames;
}

QList<TermGroup*> GroupsManager::getAllGroups()
{
    QList<TermGroup*> ret;
    for (QUuid groupUuid : getAllUuidsSortedByLastEdit()) {
        TermGroup* group = createGroup(groupUuid);

        if (group == nullptr) {
            qDebug() << "Cant create group with Uuid: " << groupUuid;
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

    if (Database::instance().groupTable->addGroup(info)) {
        updateGroupUuidNameMaps();
        emit groupsListChanged();
    } else {
        NotificationManager::showError("Название группы не уникально");
    }
}

void GroupsManager::deleteGroup(QString uuidString)
{
    Database::instance().groupTable->deleteGroup(QUuid(uuidString));
    updateGroupUuidNameMaps();
    emit groupsListChanged();
}

void GroupsManager::importGroupFromJsonFile(const QString& filename)
{
    QUrl url(filename);
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

bool GroupsManager::isValidGroupJson(const QJsonDocument json) // TODO: Rework!
{
    QJsonObject jsonGroup = json.object();

    // Checking keys
    if (
            jsonGroup.contains("name") &&
            jsonGroup.value("name").isString() &&
            jsonGroup.contains("nodesList") &&
            jsonGroup.value("nodesList").isArray()
            ) {
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

    auto info = Database::instance().groupTable->getGroup(groupUuid);
    TermGroup* group = new TermGroup(info);
    group->loadNodes(nodesMgr->getAllNodesForGroup(groupUuid));
    return group;
}

bool GroupsManager::getHasAnyGroup() const
{
    return !Database::instance().groupTable->getAllUuids().empty();
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
    // Load info from groups table - need if any group is empty and has no lastEdit value
    QMap<QUuid, QDateTime> groupsLastEdit;

    for (const auto& uuid : Database::instance().groupTable->getAllUuids()) {
        groupsLastEdit.insert(uuid, QDateTime());
    }

    // Try to fill lastEdit dateTimes
    // First - groupUuid, third - lastEdit
    QList<std::tuple<QUuid, QDateTime>> allNodesLastEdits;

    for (auto record : Database::instance().nodeTable->getAllLastEditRecords()) {

        QUuid groupUuid = QUuid(record.value(NodeColumn::groupUuid).toString());
        QDateTime lastEdit = QDateTime::fromString(record.value(NodeColumn::lastEdit).toString(), Qt::ISODate);

        if (groupsLastEdit.contains(groupUuid)) {
            if (groupsLastEdit[groupUuid].isNull()) {
                groupsLastEdit[groupUuid] = lastEdit;
            } else {
                groupsLastEdit[groupUuid] = std::max(groupsLastEdit[groupUuid], lastEdit);
            }
        }
    }

    // Casting to pairs
    QList<QPair<QUuid, QDateTime>> groupSorting;

    // Forming structure with group uuids and last edit times
    for (auto& [groupUuid, lastEdit] : groupsLastEdit.toStdMap()) {
        groupSorting.append(QPair(groupUuid, lastEdit));
    }

    // Sorting this structure
    int nMaxDate;
    for (int i = 0; i < groupSorting.size(); i++) {
        nMaxDate = i;
        for (int j = i + 1; j < groupSorting.size(); j++) {
            if (groupSorting[nMaxDate].second < groupSorting[j].second) {
                nMaxDate = j;
            }
        }
        groupSorting.swap(nMaxDate, i);
    }

    // Casting back to uuids only
    QList<QUuid> ret;
    for (int i = 0; i < groupSorting.size(); i++) {
        ret << groupSorting[i].first;
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

void GroupsManager::importGroupFromJson(const QJsonDocument& json)
{
    auto& db = Database::instance();
    if (!isValidGroupJson(json))
        return;

    qDebug() << json;

    QJsonObject jsonGroup = json.object();

    auto info = JsonGroupInfoContainerParser::fromJson(jsonGroup);

    if (info.uuid.isNull())
        return;

    QJsonArray nodes = jsonGroup.value("nodesList").toArray();

    // Searching for existed group
    if (!db.groupTable->hasGroupWithUuid(info.uuid)) { // Group found
        db.groupTable->addGroup(info);
    } else {
        db.groupTable->updateGroup(info);
    }

    // Importing nodes
    for (const auto nodeValue : nodes) {
        nodesMgr->importNodeFromJson(nodeValue.toObject());
    }

    updateGroupUuidNameMaps();
    emit groupsListChanged();
}

QString GroupsManager::getExportPath() const
{
    return AppSettings::StdPaths::groupsJsonFolder();
}

void GroupsManager::exportGrpToJson(QString groupUuid)
{
    if (auto group = createGroup(QUuid(groupUuid))) {

        saveGroupInFolder(group);

        //            QJsonDocument document = group->getJsonDoc();
        //            QClipboard* clipboard = qApp->clipboard();
        //            clipboard->setText(document.toJson());

        delete group;
    }
}

void GroupsManager::saveGroupInFolder(TermGroup *group)
{
    if (group != nullptr) {
//        QString fileName = group->getName() + " " + group->getUuid().toString() + ".grp";
//        FSWorks::saveFile(AppConfig::StdFolderPaths::groupsJsonFolder(), fileName, group->getJsonDoc().toJson());
    }
}

QJsonDocument GroupsManager::getGroupForExport(const QUuid& groupUuid)
{
    auto& db = Database::instance();

    auto info = db.groupTable->getGroup(groupUuid);
    auto groupJson = JsonGroupInfoContainerParser::toJson(info);

    auto nodesUuids = db.nodeTable->getAllNodesUuids(groupUuid);

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
    qDebug() << "List updated";

    uuidToNames.clear();
    namesToUuid.clear();

    for (auto sqlRecord : Database::instance().groupTable->getAllUuidsAndNames()) {
        QUuid uuid(sqlRecord.value(TermGroupColumn::uuid).toString());
        QString name = sqlRecord.value(TermGroupColumn::name).toString();

        uuidToNames.insert(uuid, name);
        namesToUuid.insert(name, uuid);
    }
}
