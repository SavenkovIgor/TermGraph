#include "./ndtbl.h"

#include "./columns/nodecolumn.h"

QUuid NodeTable::nodeUuidForNameAndGroup(const QString &name, const QUuid &groupUuid) const
{
    auto where = WhereCondition();
    where.equal(NodeColumn::term, name);
    where.equal(NodeColumn::termGroup, groupUuid.toString());
    auto nodesRecords = toRecVector(select(NodeColumn::longUID, where));

    if (!nodesRecords.isEmpty()) {
        return QUuid(nodesRecords.first().value(NodeColumn::longUID).toString());
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
    if (name.simplified() == "") {
        return "";  // Создать вершину не удалось
    }

    QList<InsertContainer> values;
    values << InsertContainer(NodeColumn::term, name);
    values << InsertContainer(NodeColumn::longUID, uuid.toString());
    values << InsertContainer(NodeColumn::termGroup, groupUuid.toString());

    insertInto(values);

    updateLastEdit(uuid);
    return uuid;
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
    setFieldUpdateLastEdit(NodeColumn::wikiRef, uuid, wikiRef);
}

void NodeTable::setWikiImg(const QUuid& uuid, const QString& wikiImage)
{
    setFieldUpdateLastEdit(NodeColumn::wikiImg, uuid, wikiImage);
}

void NodeTable::setGroup(const QUuid& nodeUuid, const QUuid& groupUuid)
{
    setFieldUpdateLastEdit(NodeColumn::termGroup, nodeUuid, groupUuid.toString());
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
    setField(NodeColumn::lastEdit, uuid, QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
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

UuidList NodeTable::getAllNodesUuids(const QUuid& groupUuid)
{
    UuidList ret;
    QVector<QSqlRecord> sqlRecords;

    if (groupUuid.isNull()) { // Taking all uuids
        sqlRecords = toRecVector(select(NodeColumn::longUID));
    } else {
        auto where = WhereCondition::columnEqual(NodeColumn::termGroup, groupUuid.toString());
        sqlRecords = toRecVector(select(NodeColumn::longUID, where));
    }

    for (auto& record : sqlRecords) {
        if (!record.contains(NodeColumn::longUID))
            continue;

        ret.push_back(QUuid(record.value(NodeColumn::longUID).toString()));
    }

    return filterEmptyUuids(ret);
}

NodeInfoContainer::List NodeTable::getAllNodesInfo(const QUuid& groupUuid)
{
    NodeInfoContainer::List ret;
    auto where = WhereCondition::columnEqual(NodeColumn::termGroup, groupUuid.toString());
    auto records = toRecVector(select(getAllColumns(), where));

    for (auto& record : records) {
        NodeInfoContainer container;

        container.uuid    = QUuid(record.value(NodeColumn::longUID).toString());
        container.name        = record.value(NodeColumn::term).toString();
        container.wordForms   = record.value(NodeColumn::termForms).toString();;
        container.definition  = record.value(NodeColumn::definition).toString();
        container.description = record.value(NodeColumn::description).toString();
        container.examples     = record.value(NodeColumn::examples).toString();
        container.wikiRef     = record.value(NodeColumn::wikiRef).toString();
        container.wikiImage   = record.value(NodeColumn::wikiImg).toString();
        container.groupUuid   = QUuid(record.value(NodeColumn::termGroup).toString());
        container.lastEdit    = QDateTime::fromString(record.value(NodeColumn::lastEdit).toString(), Qt::ISODate);

        ret.push_back(std::move(container));
    }

    return ret;
}

QDateTime NodeTable::getLastEdit(const QUuid &uuid)
{
    QString field = getStringField(NodeColumn::lastEdit, uuid);
    if (field.isEmpty()) {
        return QDateTime();
    }
    return QDateTime::fromString(field, Qt::ISODate);
}

RecVector NodeTable::getAllLastEditRecords()
{
    auto columns = TColumn::List();
    columns << NodeColumn::termGroup;
    columns << NodeColumn::lastEdit;
    return toRecVector(select(columns));
}

QSqlRecord NodeTable::getNodeSqlRecord(const QUuid &uuid)
{
    QSqlQuery sel = select(getAllColumns(), WhereCondition::uuidEqual(uuid));

    if (!sel.next()) {
        return QSqlRecord();
    }

    return sel.record();
}

bool NodeTable::isNodeWithUuidExist(const QUuid &uuid)
{
    return isUuidExist(uuid);
}
