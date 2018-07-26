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
        QString def,
        QString descr,
        QString exam,
        QString groupName)
{
    DBAbstract* db = Glb::db;
    // TODO: Тоже фигня. Нельзя искать в базе по имени группы!
    if (!db->groupTbl->hasGroupWithName(groupName)) {
        qDebug() << "Группа не найдена";
        return;
    }

    QUuid groupUuid = db->groupTbl->getUuid(groupName);

    db->nodeTbl->setName(nodeUuid, name);
    db->nodeTbl->setWordForms(nodeUuid, forms);
    db->nodeTbl->setDefinition(nodeUuid, def);
    db->nodeTbl->setDescription(nodeUuid, descr);
    db->nodeTbl->setExamples(nodeUuid, exam);
    db->nodeTbl->setGroup(nodeUuid, groupUuid);

    nodeChanged();
}

void NodesManager::deleteNode(QUuid uuid)
{
    DBAbstract* db = Glb::db;
    db->nodeTbl->deleteNode(uuid);
    nodeChanged();
}

QDateTime NodesManager::getLastEdit(QUuid nodeUuid)
{
    DBAbstract* db = Glb::db;
    return db->nodeTbl->getLastEdit(nodeUuid);
}
