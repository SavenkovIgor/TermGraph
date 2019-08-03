#include "source/databaseWorks/ndtbl.h"

#include "source/databaseWorks/columns/nodecolumn.h"

QUuid NodeTable::nodeUuidForNameAndGroup(const QString &name, const QUuid &groupUuid) const
{
    auto where = WhereCondition();
    where.equal(NodeColumn::term, name);
    where.equal(NodeColumn::groupUuid, groupUuid.toString());
    auto nodesRecords = toRecVector(select(NodeColumn::uuid, where));

    if (!nodesRecords.isEmpty()) {
        return QUuid(nodesRecords.first().value(NodeColumn::uuid).toString());
    }

    return QUuid();
}

QUuid NodeTable::addNode(const QString& name, const QUuid& groupUuid)
{
    QUuid uuid = generateNewUuid();
    return addNode(uuid, name, groupUuid);
}

QUuid NodeTable::addNode(const QUuid& uuid, const QString& name, const QUuid& groupUuid)
{
    Q_ASSERT(!name.simplified().isEmpty());
    Q_ASSERT(!hasNodeWithUuid(uuid));

    // Don't create node with empty name
    if (name.simplified() == "")
        return QUuid();

    if (hasNodeWithUuid(uuid))
        return QUuid();

    InsertContainer::List values;

    values.push_back(InsertContainer(NodeColumn::uuid, uuid.toString()));
    values.push_back(InsertContainer(NodeColumn::term, name));
    values.push_back(InsertContainer(NodeColumn::groupUuid, groupUuid.toString()));

    insertInto(values);

    updateLastEdit(uuid);
    return uuid;
}

QUuid NodeTable::addNode(const NodeInfoContainer& info)
{
    Q_ASSERT(!info.term.simplified().isEmpty());
    Q_ASSERT(!hasNodeWithUuid(info.uuid));

    // Создать вершину не удалось
    if (info.term.simplified().isEmpty())
        return QUuid();

    // This node already exist
    if (hasNodeWithUuid(info.uuid))
        return QUuid();

    InsertContainer::List values;

    QUuid nodeUuid = info.uuid;

    // Generate new uuid if current is empty
    if (nodeUuid.isNull()) {
        nodeUuid = generateNewUuid();
    }

    values.push_back(InsertContainer(NodeColumn::uuid, nodeUuid.toString()));
    values.push_back(InsertContainer(NodeColumn::term, info.term));
    values.push_back(InsertContainer(NodeColumn::termForms, info.termForms));
    values.push_back(InsertContainer(NodeColumn::definition, info.definition));
    values.push_back(InsertContainer(NodeColumn::description, info.description));
    values.push_back(InsertContainer(NodeColumn::examples, info.examples));
    values.push_back(InsertContainer(NodeColumn::wikiUrl, info.wikiUrl));
    values.push_back(InsertContainer(NodeColumn::wikiImage, info.wikiImage));
    values.push_back(InsertContainer(NodeColumn::groupUuid, info.groupUuid.toString()));

    QDateTime lastEdit = info.lastEdit;

    if (lastEdit.isNull()) {
        lastEdit = getLastEditNow();
    }

    values.push_back(InsertContainer(NodeColumn::lastEdit, lastEdit.toString(Qt::ISODate)));

    insertInto(values);

    return info.uuid;
}

int NodeTable::getRemindNum(const QUuid &uuid)
{
    QSqlRecord rec = getNodeSqlRecord(uuid);

    if (rec.isEmpty()) {
        return -1;
    }
    return rec.value(NodeColumn::remindNum).toInt();
}

void NodeTable::setName(const QUuid& uuid, const QString& name)
{
    setFieldUpdateLastEdit(NodeColumn::term, uuid, name);
}

void NodeTable::setWordForms(const QUuid& uuid, const QString& forms)
{
    setFieldUpdateLastEdit(NodeColumn::termForms, uuid, forms);
}

void NodeTable::setDefinition(const QUuid& uuid, const QString& definition)
{
    setFieldUpdateLastEdit(NodeColumn::definition, uuid, definition);
}

void NodeTable::setDescription(const QUuid& uuid, const QString& description)
{
    setFieldUpdateLastEdit(NodeColumn::description, uuid, description);
}

void NodeTable::setExamples(const QUuid& uuid, const QString& example)
{
    setFieldUpdateLastEdit(NodeColumn::examples, uuid, example);
}

void NodeTable::setWikiRef(const QUuid& uuid, const QString& wikiRef)
{
    setFieldUpdateLastEdit(NodeColumn::wikiUrl, uuid, wikiRef);
}

void NodeTable::setWikiImg(const QUuid& uuid, const QString& wikiImage)
{
    setFieldUpdateLastEdit(NodeColumn::wikiImage, uuid, wikiImage);
}

void NodeTable::setGroup(const QUuid& nodeUuid, const QUuid& groupUuid)
{
    setFieldUpdateLastEdit(NodeColumn::groupUuid, nodeUuid, groupUuid.toString());
}

void NodeTable::setAtLearn(const QUuid& uuid, const bool& learn)
{
    if (learn) {
        setField(NodeColumn::atLearn, uuid, "1");
    } else {
        setField(NodeColumn::atLearn, uuid, "0");
    }
}

// void NdTbl::setRemindToday(int uid)
// {
//     setRemindNum(uid, getRemindNum(uid) + 1, QDateTime::currentDateTime() );
// }

void NodeTable::setRemindNum(const QUuid& uuid, const int& num, const QDate& date)
{
    SetExpression set;
    set.set(NodeColumn::remindNum, num);
    set.set(NodeColumn::lastRemind, date.toString());

    updateWhere(set, WhereCondition::uuidEqual(uuid));
}

void NodeTable::deleteNode(const QUuid& uuid)
{
    deleteRecord(uuid);
}

const char* NodeTable::tableName() const
{
    return TableName::NODES;
}

void NodeTable::initTable()
{
    createTable();
}

TColumn::List NodeTable::getAllColumns() const
{
    TColumn::List lst;
    for (auto col : NodeColumn::columns) {
        lst << col;
    }
    return lst;
}

void NodeTable::setFieldUpdateLastEdit(const TColumn &column, const QUuid &uuid, const QString &val)
{
    setField(column, uuid, val);
    updateLastEdit(uuid);
}

void NodeTable::updateLastEdit(const QUuid& uuid)
{
    setField(NodeColumn::lastEdit, uuid, getLastEditNowString());
}

bool NodeTable::isUuidExist(const QUuid &uuid)
{
    return hasAnyRecord(WhereCondition::uuidEqual(uuid));
}

QUuid NodeTable::generateNewUuid()
{
    QUuid uuid;
    for (int i = 0; i < 1000; i++) {
        uuid = QUuid::createUuid();
        if (!isUuidExist(uuid)) {
            break;
        }
    }
    return uuid;
}

QDateTime NodeTable::getLastEditNow()
{
    return QDateTime::currentDateTimeUtc();
}

QString NodeTable::getLastEditNowString()
{
    return getLastEditNow().toString(Qt::ISODate);
}

UuidList NodeTable::getAllNodesUuids(const QUuid& groupUuid)
{
    UuidList ret;

    WhereCondition where;  // Take all uuids

    if (!groupUuid.isNull())  // Take only uuids in specifig group
        where = WhereCondition::columnEqual(NodeColumn::groupUuid, groupUuid.toString());

    auto sqlRecords = toRecVector(select(NodeColumn::uuid, where));

    for (auto& record : sqlRecords) {
        if (!record.contains(NodeColumn::uuid))
            continue;

        ret.push_back(QUuid(record.value(NodeColumn::uuid).toString()));
    }

    return filterEmptyUuids(ret);
}

NodeInfoContainer NodeTable::getNode(const QUuid& uuid)
{
    Q_ASSERT(!uuid.isNull());

    NodeInfoContainer info;

    auto where = WhereCondition::uuidEqual(uuid);
    auto records = toRecVector(select(getAllColumns(), where));

    if (records.isEmpty())
        return info;

    auto record = records.first();
    return recordToNodeInfo(record);
}

NodeInfoContainer::List NodeTable::getAllNodesInfo(const QUuid& groupUuid)
{
    Q_ASSERT(!groupUuid.isNull());

    NodeInfoContainer::List ret;
    auto where = WhereCondition::columnEqual(NodeColumn::groupUuid, groupUuid.toString());
    auto records = toRecVector(select(getAllColumns(), where));

    for (auto& record : records) {
        NodeInfoContainer info = recordToNodeInfo(record);
        ret.push_back(std::move(info));
    }

    return ret;
}

QDateTime NodeTable::getLastEdit(const QUuid &uuid)
{
    auto field = getStringField(NodeColumn::lastEdit, uuid);

    if (field.isEmpty())
        return QDateTime();

    return QDateTime::fromString(field, Qt::ISODate);
}

RecVector NodeTable::getAllLastEditRecords()
{
    auto columns = TColumn::List();
    columns << NodeColumn::groupUuid;
    columns << NodeColumn::lastEdit;
    return toRecVector(select(columns));
}

bool NodeTable::updateNode(const NodeInfoContainer& info,
                           NodeTable::LastEditSource lastEditSource,
                           bool checkLastEdit)
{
    if (info.uuid.isNull())
        return false;

    if (!hasNodeWithUuid(info.uuid))
        return false;

    if (checkLastEdit) {
        auto currentLastEdit = getLastEdit(info.uuid);
        auto newLastEdit = info.lastEdit;
        if (currentLastEdit > newLastEdit)  // If db version is fresher, do nothing
            return false;
    }

    SetExpression set;

    set.set(NodeColumn::term, info.term);
    set.set(NodeColumn::termForms, info.termForms);
    set.set(NodeColumn::definition, info.definition);
    set.set(NodeColumn::description, info.description);
    set.set(NodeColumn::examples, info.examples);
    set.set(NodeColumn::wikiUrl, info.wikiUrl);
    set.set(NodeColumn::wikiImage, info.wikiImage);
    set.set(NodeColumn::groupUuid, info.groupUuid.toString());

    switch (lastEditSource) {
    case NodeTable::TakeFromNodeInfo:
        set.set(NodeColumn::lastEdit, info.lastEdit.toString(Qt::ISODate));
        break;
    case NodeTable::AutoGenerate:
        set.set(NodeColumn::lastEdit, getLastEditNowString());
        break;
    }

    updateWhere(set, WhereCondition::uuidEqual(info.uuid));

    return true;
}

QSqlRecord NodeTable::getNodeSqlRecord(const QUuid &uuid)
{
    QSqlQuery sel = select(getAllColumns(), WhereCondition::uuidEqual(uuid));

    if (!sel.next()) {
        return QSqlRecord();
    }

    return sel.record();
}

NodeInfoContainer NodeTable::recordToNodeInfo(QSqlRecord& record)
{
    NodeInfoContainer info;

    info.uuid        = QUuid(record.value(NodeColumn::uuid).toString());
    info.term        = record.value(NodeColumn::term).toString();
    info.termForms   = record.value(NodeColumn::termForms).toString();
    info.definition  = record.value(NodeColumn::definition).toString();
    info.description = record.value(NodeColumn::description).toString();
    info.examples    = record.value(NodeColumn::examples).toString();
    info.wikiUrl     = record.value(NodeColumn::wikiUrl).toString();
    info.wikiImage   = record.value(NodeColumn::wikiImage).toString();
    info.groupUuid   = QUuid(record.value(NodeColumn::groupUuid).toString());
    info.lastEdit    = QDateTime::fromString(record.value(NodeColumn::lastEdit).toString(), Qt::ISODate);

    return info;
}

bool NodeTable::hasNodeWithUuid(const QUuid &uuid)
{
    return isUuidExist(uuid);
}
