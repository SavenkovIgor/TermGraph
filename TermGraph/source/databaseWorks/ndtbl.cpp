#include "./ndtbl.h"

bool NodeTable::hasNodeWithNameInGroup(const QString &name, const QUuid &groupUuid) const
{
    auto where = WhereCondition();
    where.equal(NodeColumn::term, name);
    where.equal(NodeColumn::termGroup, groupUuid.toString());
    RecList nodesRecords = toRecList(select(QStringList() << NodeColumn::longUID, where));
    return !nodesRecords.isEmpty();
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

void NodeTable::deleteNode(const QUuid &uuid)
{
    deleteRecord(uuid);
}

void NodeTable::setFieldUpdateLastEdit(const QString& columnName, const QUuid& uuid, const QString& val)
{
    setField(columnName, uuid, val);
    updateLastEdit(uuid);
}

void NodeTable::updateLastEdit(const QUuid& uuid)
{
    setField(NodeColumn::lastEdit, uuid, QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
}

bool NodeTable::isUuidExist(const QUuid &uuid)
{
    RecList recs = toRecList(select(QStringList() << NodeColumn::longUID,
                                      WhereCondition::uuidEqual(uuid)));
    return !recs.isEmpty();
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

QList<QUuid> NodeTable::getAllNodesUuids()
{
    QList<QUuid> ret;

    RecList idS = toRecList(select(QStringList() << NodeColumn::longUID));
    for (QSqlRecord& r : idS) {
        QUuid tmpUuid(r.value(NodeColumn::longUID).toString());
        if (!tmpUuid.isNull()) {
            ret << tmpUuid;
        }
    }

    return ret;
}

QList<QUuid> NodeTable::getAllNodesUuidsInGroup(const QUuid& groupUuid)
{
    auto where = WhereCondition::columnEqual(NodeColumn::termGroup, groupUuid.toString());
    RecList nodesRecords = toRecList(select(QStringList() << NodeColumn::longUID, where));

    QList<QUuid> ret;

    for (QSqlRecord& record : nodesRecords) {
        if (!record.contains(NodeColumn::longUID)) {
            continue;
        }

        QUuid tmpUuid(record.value(NodeColumn::longUID).toString());
        if (!tmpUuid.isNull()) {
            ret << tmpUuid;
        }
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

QSqlRecord NodeTable::getNodeSqlRecord(const QUuid &uuid)
{
    QSqlQuery sel = select(getAllCols(), WhereCondition::uuidEqual(uuid));

    if (!sel.next()) {
        return QSqlRecord();
    }

    return sel.record();
}

bool NodeTable::isNodeWithUuidExist(const QUuid &uuid)
{
    return isUuidExist(uuid);
}
