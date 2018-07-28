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
    DBAbstract* db = Glb::db;
    QList<QPair<QUuid, QDateTime>> groupSorting;

    // Forming structure with group uuids and last edit times
    for (QUuid groupUuid : db->groupTbl->getAllGroupsUuid()) {
        QPair<QUuid, QDateTime> pair;
        pair.first = groupUuid;
        pair.second = getLastEdit(groupUuid);
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

    // Casting uuids to names
    QStringList ret;
    for (int i = 0; i < groupSorting.size(); i++) {
        ret << db->groupTbl->getName(groupSorting[i].first);
    }

    if (withAllVeiw)
        ret.push_front("Все группы");
    return ret;
}

QList<TermGroup*> GroupsManager::getAllGroups()
{
    DBAbstract* db = Glb::db;
    QList<TermGroup*> ret;
    for (QUuid groupUuid : db->groupTbl->getAllGroupsUuid()) {
        QSqlRecord rec = getGroupSqlRecord(groupUuid);

        if (rec.count() == 0)
            continue;

        TermGroup* group = new TermGroup(rec);
        group->loadNodes(nodesMgr->getAllNodesForGroup(groupUuid));
        ret << group;
    }
    return ret;
}

QSqlRecord GroupsManager::getGroupSqlRecord(QUuid groupUuid)
{
    DBAbstract* db = Glb::db;
    return db->groupTbl->getGroup( groupUuid );
}

QString GroupsManager::getGroupNameByUuid(QUuid groupUuid)
{
    DBAbstract* db = Glb::db;
    QSqlRecord rec = getGroupSqlRecord(groupUuid);
    if (!rec.contains(db->groupTbl->name))
        return "";
    return rec.value(db->groupTbl->name).toString();
}

void GroupsManager::addNewGroup(const QString& name, const QString& comment, const int& type)
{
    DBAbstract* db = Glb::db;
    if (db->groupTbl->addGroup(name, comment, type)) {
        groupsListChanged();
    } else {
        // TODO: Сделать уведомлением!
        qDebug() << "Название группы не уникально";
    }
}

void GroupsManager::deleteGroup(QString name)
{
    DBAbstract* db = Glb::db;
    db->groupTbl->deleteGroup(name);
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

TermGroup *GroupsManager::getGroupByNameForInnerUse(const QString name)
{
    DBAbstract* db = Glb::db;
    QUuid groupUuid = db->groupTbl->getUuid(name);
    if (groupUuid.isNull())
        return nullptr;

    QSqlRecord groupRecord = getGroupSqlRecord(groupUuid);
    return new TermGroup(groupRecord);
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

void GroupsManager::importGroupFromJson(QJsonDocument json)
{
    DBAbstract* db = Glb::db;
    if (!isValidGroupJson(json))
        return;

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
            db->nodeTbl->addNode(nodeUuid, name);
            nodesMgr->changeNode(
                        nodeUuid,
                        name,
                        forms,
                        definition,
                        description,
                        examples,
                        groupName);
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

    groupsListChanged();
}

void GroupsManager::sendGroupByNetwork(const QString groupName)
{
    TermGroup* g = getGroupByNameForInnerUse(groupName);
    if (g == nullptr)
        return;

    network->sendGroup(g->getJsonDoc());
}
