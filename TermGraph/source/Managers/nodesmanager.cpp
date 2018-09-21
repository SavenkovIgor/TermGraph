#include "nodesmanager.h"

NodesManager::NodesManager(QObject *parent) : QObject(parent)
{ }

void NodesManager::addNewNode(
        QString name,
        const QString forms,
        const QString def,
        const QString descr,
        const QString exam,
        const QString groupName)
{
    DBAbstract* db = Glb::db;
    // TODO: Тоже фигня. Нельзя искать в базе по имени группы!
    if (!db->groupTbl->hasGroupWithName(groupName)) {
        qDebug() << "Группа не найдена";
        return;
    }

    QUuid nodeUuid = db->nodeTbl->addNode(name);
    changeNode(nodeUuid, name, forms, def, descr, exam, groupName);
}

void NodesManager::changeNode(
        QUuid nodeUuid,
        QString name,
        QString forms,
        QString definition,
        QString description,
        QString example,
        QString groupName)
{
    DBAbstract* db = Glb::db;
    // TODO: Тоже фигня. Нельзя искать в базе по имени группы!
    if (!db->groupTbl->hasGroupWithName(groupName)) {
        qDebug() << "Группа не найдена";
        return;
    }

    QUuid groupUuid = db->groupTbl->getUuid(groupName);
    changeNode(nodeUuid, name, forms, definition, description, example, groupUuid);
}

void NodesManager::changeNode(
        QUuid nodeUuid,
        QString name,
        QString forms,
        QString definition,
        QString description,
        QString example,
        QUuid groupUuid)
{
    DBAbstract* db = Glb::db;

    db->nodeTbl->setName(nodeUuid, name);
    db->nodeTbl->setWordForms(nodeUuid, forms);
    db->nodeTbl->setDefinition(nodeUuid, definition);
    db->nodeTbl->setDescription(nodeUuid, description);
    db->nodeTbl->setExamples(nodeUuid, example);
    db->nodeTbl->setGroup(nodeUuid, groupUuid);

    nodeChanged();
}

void NodesManager::deleteNode(QUuid uuid)
{
    DBAbstract* db = Glb::db;
    db->nodeTbl->deleteNode(uuid);
    nodeChanged();
}

TermNode::List NodesManager::getAllNodesForGroup(QUuid groupUuid)
{
    TermNode::List ret;
    for (QUuid uuid : getAllNodesUuidsInGroup(groupUuid)) {
        QSqlRecord rec = getNodeSqlRecord(uuid);
        if (rec.count() == 0)
            continue;

        ret << new TermNode(rec);
    }
    return ret;
}

QList<QUuid> NodesManager::getAllNodesUuidsInGroup(QUuid groupUuid)
{
    DBAbstract* db = Glb::db;
    return db->nodeTbl->getAllNodesUuidsInGroup(groupUuid);
}

QSqlRecord NodesManager::getNodeSqlRecord(QUuid nodeUuid)
{
    DBAbstract* db = Glb::db;
    return db->nodeTbl->getNodeSqlRecord(nodeUuid);
}

QDateTime NodesManager::getLastEdit(QUuid nodeUuid)
{
    DBAbstract* db = Glb::db;
    return db->nodeTbl->getLastEdit(nodeUuid);
}

void NodesManager::importNodeFromJson(QJsonObject nodeObject)
{
    DBAbstract* db = Glb::db;
    QUuid nodeUuid = QUuid(nodeObject.value(NodeColumn::longUID).toString());

    if (nodeUuid.isNull()) {
        nodeUuid = QUuid(nodeObject.value("longUid").toString());
    }

    if (nodeUuid.isNull()) {
        return;
    }

    QString name = nodeObject.value(NodeColumn::term).toString();
    QString forms = nodeObject.value(NodeColumn::termForms).toString();
    QString definition = nodeObject.value(NodeColumn::definition).toString();
    QString description = nodeObject.value(NodeColumn::description).toString();
    QString examples = nodeObject.value(NodeColumn::examples).toString();
    QUuid groupUuid = QUuid(nodeObject.value(NodeColumn::termGroup).toString());
    QString lastEditString = nodeObject.value(NodeColumn::lastEdit).toString();
    QDateTime lastEdit = QDateTime::fromString(lastEditString, Qt::ISODate);

    if (groupUuid.isNull()) {
        return;
    }

    // Create
    if (!db->nodeTbl->isNodeWithUuidExist(nodeUuid)) {
        // TODO: Отрефакторить. отдавать всю работу nodesManager,
        // это его ответственность
        db->nodeTbl->addNode(nodeUuid, name);
        changeNode(nodeUuid, name, forms, definition, description, examples, groupUuid);
    } else {
        // TODO: Continue from here!!!
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
