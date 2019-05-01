#include "nodesmanager.h"

NodesManager::NodesManager(QObject *parent) : QObject(parent) { }

bool NodesManager::addNewNode(
        const QString &name,
        const QString &forms,
        const QString &def,
        const QString &descr,
        const QString &exam,
        const QString &groupUuidString,
        const bool &sendChangeSignal)
{
    QUuid groupUuid = QUuid(groupUuidString);

    if (!correctGroupUuid(groupUuid))
        return false;

    if (!correctNewNodeName(name, groupUuid))
        return false;

    QUuid nodeUuid = Glb::dbPtr->nodeTbl->addNode(name, groupUuid);
    return changeNode(nodeUuid, name, forms, def, descr, exam, groupUuid, sendChangeSignal);
}

bool NodesManager::changeNode(
        const QUuid &nodeUuid,
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
        return false;

    // Check for already existing node with same name
    auto alterNodeUuid = getNodeUuidByNameAndGroup(name, groupUuid);
    if (!alterNodeUuid.isNull()) {
        if (alterNodeUuid != nodeUuid) {
            showWarning("Термин с таким названием уже существует в этой группе");
            return false;
        }
    }

    return changeNode(nodeUuid, name, forms, definition, description, example, groupUuid, sendChangeSignal);
}

bool NodesManager::changeNode(
        const QUuid& nodeUuid,
        const QString& name,
        const QString& forms,
        const QString& definition,
        const QString& description,
        const QString& example,
        const QUuid& groupUuid,
        const bool& sendChangeSignal)
{
    Database* db = Glb::dbPtr.get();

    db->nodeTbl->setName(nodeUuid, name);
    db->nodeTbl->setWordForms(nodeUuid, forms);
    db->nodeTbl->setDefinition(nodeUuid, definition);
    db->nodeTbl->setDescription(nodeUuid, description);
    db->nodeTbl->setExamples(nodeUuid, example);
    db->nodeTbl->setGroup(nodeUuid, groupUuid);

    if (sendChangeSignal) {
        nodeChanged();
    }

    return true;
}

void NodesManager::deleteNode(QUuid uuid)
{
    Glb::dbPtr->nodeTbl->deleteNode(uuid);
    nodeChanged();
}

PaintedTerm::List NodesManager::getAllNodesForGroup(QUuid groupUuid)
{
    PaintedTerm::List ret;

    auto records = Glb::dbPtr->nodeTbl->getAllNodesDBRecrods(groupUuid);

    for (auto record : records) {

        if (record.count() == 0)
            continue;

        ret << new PaintedTerm(record);
    }
    return ret;
}

QList<QUuid> NodesManager::getAllNodesUuidsInGroup(QUuid groupUuid)
{
    return Glb::dbPtr->nodeTbl->getAllNodesUuidsInGroup(groupUuid);
}

QSqlRecord NodesManager::getNodeSqlRecord(QUuid nodeUuid)
{
    return Glb::dbPtr->nodeTbl->getNodeSqlRecord(nodeUuid);
}

QDateTime NodesManager::getLastEdit(QUuid nodeUuid)
{
    return Glb::dbPtr->nodeTbl->getLastEdit(nodeUuid);
}

void NodesManager::importNodeFromJson(QJsonObject nodeObject)
{
    Database* db = Glb::dbPtr.get();
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

    if (!Glb::dbPtr->groupTbl->hasGroupWithUuid(groupUuid)) {
        if (sendWarnings) {
            showWarning("Группа " + groupUuid.toString() + " не найдена");
        }
        return false;
    }

    return true;
}

bool NodesManager::correctNewNodeName(const QString &name, QUuid &groupUuid, bool showWarnings)
{
    if (hasNodeWithNameInGroup(name, groupUuid)) {
        if (showWarnings) {
            showWarning("Термин с таким названием уже существует в этой группе");
        }
        return false;
    }

    return true;
}

QUuid NodesManager::getNodeUuidByNameAndGroup(const QString &name, QUuid &groupUuid) const
{
    return Glb::dbPtr->nodeTbl->nodeUuidForNameAndGroup(name, groupUuid);
}

bool NodesManager::hasNodeWithNameInGroup(const QString &name, QUuid &groupUuid) const
{
    return !getNodeUuidByNameAndGroup(name, groupUuid).isNull();
}
