#include "nodesmanager.h"

#include "../databaseWorks/columns/nodecolumn.h"

NodesManager::NodesManager(QObject* parent)
    : QObject(parent)
{}

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

    QUuid nodeUuid = Database::instance().nodeTable->addNode(name, groupUuid);
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
            emit showWarning("Термин с таким названием уже существует в этой группе");
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
    auto& db = Database::instance();

    db.nodeTable->setName(nodeUuid, name);
    db.nodeTable->setWordForms(nodeUuid, forms);
    db.nodeTable->setDefinition(nodeUuid, definition);
    db.nodeTable->setDescription(nodeUuid, description);
    db.nodeTable->setExamples(nodeUuid, example);
    db.nodeTable->setGroup(nodeUuid, groupUuid);

    if (sendChangeSignal) {
        emit nodeChanged();
    }

    return true;
}

void NodesManager::deleteNode(QUuid uuid)
{
    Database::instance().nodeTable->deleteNode(uuid);
    emit nodeChanged();
}

PaintedTerm::List NodesManager::getAllNodesForGroup(const QUuid& groupUuid)
{
    PaintedTerm::List ret;
    auto nodesInfo = Database::instance().nodeTable->getAllNodesInfo(groupUuid);

    for (const auto& info : nodesInfo)
        ret << new PaintedTerm(info);

    return ret;
}

UuidList NodesManager::getAllNodesUuidsInGroup(const QUuid& groupUuid)
{
    return Database::instance().nodeTable->getAllNodesUuids(groupUuid);
}

QSqlRecord NodesManager::getNodeSqlRecord(QUuid nodeUuid)
{
    return Database::instance().nodeTable->getNodeSqlRecord(nodeUuid);
}

QDateTime NodesManager::getLastEdit(QUuid nodeUuid)
{
    return Database::instance().nodeTable->getLastEdit(nodeUuid);
}

void NodesManager::importNodeFromJson(QJsonObject nodeObject)
{
    auto& db = Database::instance();
    QUuid nodeUuid = QUuid(nodeObject.value(NodeColumn::uuid).toString());

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
    QUuid groupUuid = QUuid(nodeObject.value(NodeColumn::groupUuid).toString());
    QString lastEditString = nodeObject.value(NodeColumn::lastEdit).toString();
    QDateTime lastEdit = QDateTime::fromString(lastEditString, Qt::ISODate);

    if (groupUuid.isNull()) {
        return;
    }

    // Create
    if (!db.nodeTable->isNodeWithUuidExist(nodeUuid)) {
        // TODO: Отрефакторить. отдавать всю работу nodesManager,
        // это его ответственность
        db.nodeTable->addNode(nodeUuid, name, groupUuid);
        changeNode(nodeUuid, name, forms, definition, description, examples, groupUuid);
    } else {
        // TODO: Continue from here!!!
        // Update
        if (name.simplified() != "")
            db.nodeTable->setName(nodeUuid, name);
        if (forms.simplified() != "")
            db.nodeTable->setWordForms(nodeUuid, forms);
        if (definition.simplified() != "")
            db.nodeTable->setDefinition(nodeUuid, definition);
        if (description.simplified() != "")
            db.nodeTable->setDescription(nodeUuid, description);
        if (examples.simplified() != "")
            db.nodeTable->setExamples(nodeUuid, examples);

        db.nodeTable->setGroup(nodeUuid, groupUuid);
    }
}

bool NodesManager::correctGroupUuid(const QUuid &groupUuid, bool sendWarnings)
{
    if (groupUuid.isNull()) {
        if (sendWarnings) {
            emit showWarning("Пустой или некорректный Uuid группы");
        }
        return false;
    }

    if (!Database::instance().groupTable->hasGroupWithUuid(groupUuid)) {
        if (sendWarnings) {
            emit showWarning("Группа " + groupUuid.toString() + " не найдена");
        }
        return false;
    }

    return true;
}

bool NodesManager::correctNewNodeName(const QString &name, QUuid &groupUuid, bool showWarnings)
{
    if (hasNodeWithNameInGroup(name, groupUuid)) {
        if (showWarnings) {
            emit showWarning("Термин с таким названием уже существует в этой группе");
        }
        return false;
    }

    return true;
}

QUuid NodesManager::getNodeUuidByNameAndGroup(const QString &name, QUuid &groupUuid) const
{
    return Database::instance().nodeTable->nodeUuidForNameAndGroup(name, groupUuid);
}

bool NodesManager::hasNodeWithNameInGroup(const QString &name, QUuid &groupUuid) const
{
    return !getNodeUuidByNameAndGroup(name, groupUuid).isNull();
}
