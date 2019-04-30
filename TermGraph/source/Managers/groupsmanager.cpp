#include "groupsmanager.h"

GroupsManager::GroupsManager(
        NodesManager *nodesMgr,
        NetworkManager* network,
        QObject *parent):
    QObject(parent)
{
    this->nodesMgr = nodesMgr;
    this->network = network;
    connect(
                this->network,
                SIGNAL(newSyncGroup(QString)),
                SLOT(importGroupFromJson(QString)));
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
        showError("Название группы не может быть пустым");
        return;
    }

    int type = 0;  // GroupType::terms
    if (Glb::dbPtr->groupTbl->addGroup(name, comment, type)) {
        updateGroupUuidNameMaps();
        groupsListChanged();
    } else {
        showError("Название группы не уникально");
    }
}

void GroupsManager::deleteGroup(QString uuidString)
{
    Glb::dbPtr->groupTbl->deleteGroup(QUuid(uuidString));
    updateGroupUuidNameMaps();
    groupsListChanged();
}

void GroupsManager::importGroupFromJsonFile(const QString& filename)
{
    QUrl url(filename);
    QFile file(url.toLocalFile());
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray arr = file.readAll();
        importGroupFromJson(QString(arr));
    }
}

void GroupsManager::importGroupFromJson(const QString& rawJson)
{
    QByteArray byteArr;
    byteArr.append(rawJson);
    QJsonDocument doc = QJsonDocument::fromJson(byteArr);
    importGroupFromJson(doc);
}

bool GroupsManager::isValidGroupJson(const QJsonDocument json)
{
    QJsonObject jsonGroup = json.object();

    // Checking keys
    if (
            jsonGroup.contains("name") &&
            jsonGroup.value("name").isString() &&
            jsonGroup.contains("type") &&
            jsonGroup.value("type").isDouble() &&
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

    QSqlRecord groupRecord = Glb::dbPtr->groupTbl->getGroup(groupUuid);
    TermGroup* group = new TermGroup(groupRecord);
    group->loadNodes(nodesMgr->getAllNodesForGroup(groupUuid));
    return group;
}

bool GroupsManager::hasAnyGroup() const
{
    return !Glb::dbPtr->groupTbl->getAllGroupsUuid().isEmpty();
}

QDateTime GroupsManager::getLastEdit(QUuid groupUuid)
{
    QDateTime lastEdit;
    for (QUuid nodeUuid : nodesMgr->getAllNodesUuidsInGroup(groupUuid)) {
        QDateTime currNodeLastEdit = nodesMgr->getLastEdit(nodeUuid);
        if (lastEdit.isNull()) {
            lastEdit = currNodeLastEdit;
        } else {
            lastEdit = qMax(lastEdit, currNodeLastEdit);
        }
    }
    return lastEdit;
}

QList<QUuid> GroupsManager::getAllUuidsSortedByLastEdit()
{
    using namespace std;

    // First - nodeUuid, second - groupUuid, third - lastEdit
    QList<tuple<QUuid, QUuid, QDateTime>> allLastEdits;

    for (auto record : Glb::dbPtr->nodeTbl->getAllLastEditRecords()) {
        QUuid nodeUuid  = QUuid(record.value(NodeColumn::longUID).toString());
        QUuid groupUuid = QUuid(record.value(NodeColumn::termGroup).toString());
        QDateTime lastEdit = QDateTime::fromString(record.value(NodeColumn::lastEdit).toString(), Qt::ISODate);

        allLastEdits << make_tuple(nodeUuid, groupUuid, lastEdit);
    }

    // Taking group last edit
    QMap<QUuid, QDateTime> groupsLastEdit;

    for (const auto& [nodeUuid, groupUuid, lastEdit] : allLastEdits) {
        if (groupsLastEdit.contains(groupUuid)) {
            if (groupsLastEdit[groupUuid] < lastEdit) {
                groupsLastEdit[groupUuid] = lastEdit;
            }
        } else {
            groupsLastEdit.insert(groupUuid, lastEdit);
        }
    }

    // Casting to pairs
    QList<QPair<QUuid, QDateTime>> groupSorting;

    // Forming structure with group uuids and last edit times
    for (auto& [groupUuid, lastEdit] : groupsLastEdit.toStdMap()) {
        QPair<QUuid, QDateTime> pair;
        pair.first = groupUuid;
        pair.second = lastEdit;
        groupSorting.append(pair);
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
    DBAbstract* db = Glb::dbPtr.get();
    if (!isValidGroupJson(json))
        return;

    qDebug() << json;

    QJsonObject jsonGroup = json.object();
    QUuid groupUuid = QUuid(jsonGroup.value("longUID").toString());

    if (groupUuid.isNull())
        return;

    QString groupName = jsonGroup.value("name").toString();
    QString comment;
    int type = jsonGroup.value("type").toInt();

    QJsonArray nodes = jsonGroup.value("nodesList").toArray();

    // Searching for existed group
    if (db->groupTbl->hasGroupWithUuid(groupUuid)) {  // Group found
        db->groupTbl->setName(groupUuid, groupName);
        db->groupTbl->setComment(groupUuid, comment);
        db->groupTbl->setType(groupUuid, type);
    } else {
        db->groupTbl->addGroup(groupUuid, groupName, comment, type);
    }

    // Importing nodes
    for (QJsonValue nodeValue : nodes) {
        QJsonObject nodeObj = nodeValue.toObject();

        QUuid nodeUuid = QUuid(nodeObj.value("longUID").toString());

        if (nodeUuid.isNull())
            nodeUuid = QUuid(nodeObj.value("longUid").toString());

        if (nodeUuid.isNull())
            continue;

        QString name = nodeObj.value("name").toString();
        QString forms = nodeObj.value("nameForms").toString();
        QString definition = nodeObj.value("definition").toString();
        QString description = nodeObj.value("description").toString();
        QString examples = nodeObj.value("examples").toString();

        // Create
        if (!db->nodeTbl->isNodeWithUuidExist(nodeUuid)) {
            // TODO: Отрефакторить. отдавать всю работу nodesManager,
            // это его ответственность
            db->nodeTbl->addNode(nodeUuid, name, groupUuid);
            nodesMgr->changeNode(
                        nodeUuid,
                        name,
                        forms,
                        definition,
                        description,
                        examples,
                        groupUuid.toString(),
                        false);
        } else {
            // Update
            if (name.simplified() != "")
                db->nodeTbl->setName(nodeUuid, name);
            if (forms.simplified() != "")
                db->nodeTbl->setWordForms(nodeUuid, forms);
            if (definition.simplified() != "")
                db->nodeTbl->setDefinition(nodeUuid, definition);
            if (description.simplified() != "")
                db->nodeTbl->setDescription(nodeUuid, description);
            if (examples.simplified() != "")
                db->nodeTbl->setExamples(nodeUuid, examples);

            db->nodeTbl->setGroup(nodeUuid, groupUuid);
        }
    }

    updateGroupUuidNameMaps();
    groupsListChanged();
}

QString GroupsManager::getExportPath() const
{
    return AppConfig::StdFolderPaths::groupsJsonFolder();
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
        QString fileName = group->getName() + " " + group->getUuid().toString() + ".grp";
        FSWorks::saveFile(AppConfig::StdFolderPaths::groupsJsonFolder(), fileName, group->getJsonDoc().toJson());
    }
}

void GroupsManager::sendGroupByNetwork(const QString groupUuid)
{
    if (auto group = createGroup(QUuid(groupUuid))) {
        network->sendGroup(group->getJsonDoc());
        //    delete group;  // TODO: Проверить, почему удаление вызывает ошибку
    }
}

void GroupsManager::updateGroupUuidNameMaps()
{
    qDebug() << "List updated";

    uuidToNames.clear();
    namesToUuid.clear();

    for (auto sqlRecord : Glb::dbPtr->groupTbl->getAllUuidsAndNames()) {
        QUuid uuid(sqlRecord.value(TermGroupColumn::longUID).toString());
        QString name = sqlRecord.value(TermGroupColumn::name).toString();

        uuidToNames.insert(uuid, name);
        namesToUuid.insert(name, uuid);
    }
}
