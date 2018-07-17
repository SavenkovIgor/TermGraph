#include "groupsmanager.h"

GroupsManager::GroupsManager(NodesManager *nodesMgr, NetworkManager* network, QObject *parent) : QObject(parent)
{
    this->nodesMgr = nodesMgr;
    this->network = network;
    connect(this->network, SIGNAL(newSyncGroup(QString)), SLOT(importGroupFromJson(QString)));
}

QStringList GroupsManager::getAllGroupsNames(bool withAllVeiw)
{
    DBAbstract* db = Glb::db;
    QStringList ret = db->groupTbl->getAllGroupsNames();
    if (withAllVeiw)
        ret.push_front("Все группы");
    return ret;
}

QList<QUuid> GroupsManager::getAllGroupsUuids()
{
    DBAbstract* db = Glb::db;
    return db->groupTbl->getAllGroupsUuid();
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
    if(!rec.contains(db->groupTbl->name))
        return "";
    return rec.value(db->groupTbl->name).toString();
}

void GroupsManager::addNewGroup(QString name, QString comment, int type)
{
    DBAbstract* db = Glb::db;
    if ( db->groupTbl->addGroup( name, comment, type ) )
        groupsListChanged();
    else
        qDebug()<<"Название группы не уникально"; //TODO: Сделать уведомлением!
}

void GroupsManager::deleteGroup(QString name)
{
    DBAbstract* db = Glb::db;
    db->groupTbl->deleteGroup( name );
    groupsListChanged();
}

void GroupsManager::importGroupFromJsonFile(QString filename)
{
    QUrl url(filename);
    QFile file(url.toLocalFile());
    if(file.open(QIODevice::ReadOnly)) {
        QByteArray arr = file.readAll();
        importGroupFromJson(QString(arr));
    }
}

void GroupsManager::importGroupFromJson(QString rawJson)
{
    QByteArray byteArr;
    byteArr.append(rawJson);
    QJsonDocument doc = QJsonDocument::fromJson(byteArr);
    importGroupFromJson(doc);
}

bool GroupsManager::isValidGroupJson(QJsonDocument json)
{
    QJsonObject jsonGroup = json.object();

    //Checking keys
    if(
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

TermGroup *GroupsManager::getGroupByNameForInnerUse(QString name)
{
    DBAbstract* db = Glb::db;
    QUuid groupUuid = db->groupTbl->getUuid(name);
    if(groupUuid.isNull())
        return nullptr;

    QSqlRecord groupRecord = getGroupSqlRecord(groupUuid);
    if(groupRecord.count() != 1)
        return nullptr;

    return new TermGroup(groupRecord);
}

void GroupsManager::importGroupFromJson(QJsonDocument json)
{
    DBAbstract* db = Glb::db;
    if(!isValidGroupJson(json))
        return;

    QJsonObject jsonGroup = json.object();
    QUuid groupUuid = QUuid(jsonGroup.value("longUID").toString());

    if(groupUuid.isNull())
        return;

    QString groupName = jsonGroup.value("name").toString();
    QString comment;
    int type = jsonGroup.value("type").toInt();

    QJsonArray nodes = jsonGroup.value("nodesList").toArray();

    // Searching for existed group
    if( db->groupTbl->hasGroupWithUuid(groupUuid) ) { //Group found
        db->groupTbl->setName(groupUuid, groupName);
        db->groupTbl->setComment(groupUuid, comment);
        db->groupTbl->setType(groupUuid, type);
    } else {
        db->groupTbl->addGroup(groupUuid, groupName, comment, type);
    }

    // Importing nodes
    for(QJsonValue nodeValue: nodes) {
        QJsonObject nodeObj = nodeValue.toObject();

        QUuid nodeUuid = QUuid(nodeObj.value("longUID").toString());

        if(nodeUuid.isNull())
            nodeUuid = QUuid(nodeObj.value("longUid").toString());

        if(nodeUuid.isNull())
            continue;

        QString name = nodeObj.value("name").toString();
        QString forms = nodeObj.value("nameForms").toString();
        QString definition = nodeObj.value("definition").toString();
        QString description = nodeObj.value("description").toString();
        QString examples = nodeObj.value("examples").toString();

        // Create
        if( !db->nodeTbl->isNodeWithUuidExist( nodeUuid ) ) {
            //TODO: Отрефакторить. отдавать всю работу nodesManager это его ответственность
            db->nodeTbl->addNode(nodeUuid, name);
            nodesMgr->changeNode(nodeUuid, name, forms, definition, description, examples, groupName);
        } else {
            // Update
            if(name.simplified() != "")
                db->nodeTbl->setName(nodeUuid, name);
            if(forms.simplified() != "")
                db->nodeTbl->setWordForms(nodeUuid, forms);
            if(definition.simplified() != "")
                db->nodeTbl->setDefinition (nodeUuid, definition);
            if(description.simplified() != "")
                db->nodeTbl->setDescription(nodeUuid, description);
            if(examples.simplified() != "")
                db->nodeTbl->setExamples(nodeUuid, examples);

            db->nodeTbl->setGroup(nodeUuid, groupUuid);
        }
    }

    groupsListChanged();
}

void GroupsManager::sendGroupByNetwork(QString groupName)
{
    TermGroup* g = getGroupByNameForInnerUse(groupName);
    if ( g == nullptr )
        return;

    network->connectAndSendGroup(g->getJsonDoc());
}
