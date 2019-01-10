#include "nodesmanager.h"

NodesManager::NodesManager(QObject *parent) : QObject(parent) { }

void NodesManager::addNewNode(const QString &name,
        const QString &forms,
        const QString &def,
        const QString &descr,
        const QString &exam,
        const QString &groupUuidString,
        const bool &sendChangeSignal)
{
    QUuid groupUuid = QUuid(groupUuidString);

    if (!correctGroupUuid(groupUuid))
        return;

    if (!correctNodeName(name, groupUuid))
        return;

    QUuid nodeUuid = Glb::db->nodeTbl->addNode(name, groupUuid);
    changeNode(nodeUuid, name, forms, def, descr, exam, groupUuid, sendChangeSignal);
}

void NodesManager::changeNode(const QUuid &nodeUuid,
        const QString &name,
        const QString &forms,
        const QString &definition,
        const QString &description,
        const QString &example,
        const QString &groupUuidString,
        const bool &sendChangeSignal)
{
    QUuid groupUuid = QUuid(groupUuidString);

    if (!correctGroupUuid(groupUuid))
        return;

    if (!correctNodeName(name, groupUuid))
        return;

    changeNode(nodeUuid, name, forms, definition, description, example, groupUuid, sendChangeSignal);
}

void NodesManager::changeNode(
        const QUuid& nodeUuid,
        const QString& name,
        const QString& forms,
        const QString& definition,
        const QString& description,
        const QString& example,
        const QUuid& groupUuid,
        const bool& sendChangeSignal)
{
    DBAbstract* db = Glb::db;

    db->nodeTbl->setName(nodeUuid, name);
    db->nodeTbl->setWordForms(nodeUuid, forms);
    db->nodeTbl->setDefinition(nodeUuid, definition);
    db->nodeTbl->setDescription(nodeUuid, description);
    db->nodeTbl->setExamples(nodeUuid, example);
    db->nodeTbl->setGroup(nodeUuid, groupUuid);

    if (sendChangeSignal) {
        nodeChanged();
    }
}

void NodesManager::deleteNode(QUuid uuid)
{
    DBAbstract* db = Glb::db;
    db->nodeTbl->deleteNode(uuid);
    nodeChanged();
}

PaintedTerm::List NodesManager::getAllNodesForGroup(QUuid groupUuid)
{
    PaintedTerm::List ret;
    for (QUuid uuid : getAllNodesUuidsInGroup(groupUuid)) {
        QSqlRecord rec = getNodeSqlRecord(uuid);
        if (rec.count() == 0)
            continue;

        ret << new PaintedTerm(rec);
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
        db->nodeTbl->addNode(nodeUuid, name, groupUuid);
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

bool NodesManager::correctGroupUuid(const QUuid &groupUuid, bool sendWarnings)
{
    if (groupUuid.isNull()) {
        if (sendWarnings) {
            showWarning("Пустой или некорректный Uuid группы");
        }
        return false;
    }

    if (!Glb::db->groupTbl->hasGroupWithUuid(groupUuid)) {
        if (sendWarnings) {
            showWarning("Группа " + groupUuid.toString() + " не найдена");
        }
        return false;
    }

    return true;
}

bool NodesManager::correctNodeName(const QString &name, QUuid &groupUuid, bool showWarnings)
{
    if (Glb::db->nodeTbl->hasNodeWithNameInGroup(name, groupUuid)) {
        if (showWarnings) {
            showWarning("Термин с таким названием уже существует в этой группе");
        }
        return false;
    }

    return true;
}
