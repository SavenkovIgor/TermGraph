#include "./ndtbl.h"

//const QString NodeColumn::uid = "uid";

QUuid NodeTable::addNode(QString name)
{
    QUuid uuid = generateNewUuid();
    return addNode(uuid, name);
}

QUuid NodeTable::addNode(QUuid uuid, QString name)
{
    if (name.simplified() == "") {
        return "";  // Создать вершину не удалось
    }

    QList<InsertContainer> values;
    values << InsertContainer(this->term, name);
    values << InsertContainer(this->longUID, uuid.toString());

    insertInto(values);

    updateLastEdit(uuid);
    return uuid;
}

int NodeTable::getRemindNum(QUuid uuid)
{
    QSqlRecord rec = getNodeSqlRecord(uuid);

    if (rec.isEmpty()) {
        return -1;
    }
    return rec.value(this->remindNum).toInt();
}

void NodeTable::setName(QUuid uuid, QString name)
{
    setFieldUpdateLastEdit(this->term, uuid, name);
}

void NodeTable::setWordForms(QUuid uuid, QString forms)
{
    setFieldUpdateLastEdit(this->termForms, uuid, forms);
}

void NodeTable::setDefinition(QUuid uuid, QString definition)
{
    setFieldUpdateLastEdit(this->definition, uuid, definition);
}

void NodeTable::setDescription(QUuid uuid, QString description)
{
    setFieldUpdateLastEdit(this->description, uuid, description);
}

void NodeTable::setExamples(QUuid uuid, QString example)
{
    setFieldUpdateLastEdit(this->examples, uuid, example);
}

void NodeTable::setWikiRef(QUuid uuid, QString wikiRef)
{
    setFieldUpdateLastEdit(this->wikiRef, uuid, wikiRef);
}

void NodeTable::setWikiImg(QUuid uuid, QString wikiImage)
{
    setFieldUpdateLastEdit(this->wikiImg, uuid, wikiImage);
}

void NodeTable::setGroup(QUuid nodeUuid, QUuid groupUuid)
{
    setFieldUpdateLastEdit(this->termGroup, nodeUuid, groupUuid.toString());
}

void NodeTable::setAtLearn(QUuid uuid, bool learn)
{
    if (learn) {
        setField(this->atLearn, uuid, "1");
    } else {
        setField(this->atLearn, uuid, "0");
    }
}

// void NdTbl::setRemindToday(int uid)
// {
//     setRemindNum(uid, getRemindNum(uid) + 1, QDateTime::currentDateTime() );
// }

void NodeTable::setRemindNum(QUuid uuid, int num, QDate date)
{
    SetExpression set;
    set.set(this->remindNum, num);
    set.set(this->lastRemind, date.toString());

    updateWhere(set, WhereCondition::uuidEqual(uuid));
}

void NodeTable::deleteNode(QUuid uuid)
{
    deleteRecord(uuid);
}

void NodeTable::setFieldUpdateLastEdit(QString columnName, QUuid uuid, QString val)
{
    setField(columnName, uuid, val);
    updateLastEdit(uuid);
}

void NodeTable::updateLastEdit(QUuid uuid)
{
    setField(this->lastEdit, uuid, QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
}

bool NodeTable::isUuidExist(QUuid uuid)
{
    RecList recs = toRecList(select(QStringList() << this->longUID,
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

    RecList idS = toRecList(select(QStringList() << this->longUID));
    for (QSqlRecord& r : idS) {
        QUuid tmpUuid(r.value(this->longUID).toString());
        if (!tmpUuid.isNull()) {
            ret << tmpUuid;
        }
    }

    return ret;
}

QList<QUuid> NodeTable::getAllNodesUuidsInGroup(QUuid groupUuid)
{
    QList<QUuid> ret;

    WhereCondition where;
    where.equal(this->termGroup, groupUuid.toString());

    RecList nodesRecords = toRecList(select(QStringList() << this->longUID, where));

    for (QSqlRecord& record : nodesRecords) {
        if (!record.contains(this->longUID)) {
            continue;
        }

        QUuid tmpUuid(record.value(this->longUID).toString());
        if (!tmpUuid.isNull()) {
            ret << tmpUuid;
        }
    }

    return ret;
}

QDateTime NodeTable::getLastEdit(QUuid uuid)
{
    QString field = getStringField(this->lastEdit, uuid);
    if (field.isEmpty()) {
        return QDateTime();
    }
    return QDateTime::fromString(field, Qt::ISODate);
}

QSqlRecord NodeTable::getNodeSqlRecord(QUuid uuid)
{
    QSqlQuery sel = select(getAllCols(), WhereCondition::uuidEqual(uuid));

    if (!sel.next()) {
        return QSqlRecord();
    }

    return sel.record();
}

bool NodeTable::isNodeWithUuidExist(QUuid uuid)
{
    return isUuidExist(uuid);
}
