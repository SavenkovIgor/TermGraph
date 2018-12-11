#include "termgrouptbl.h"







bool TermGroupTable::addGroup(QString name, QString comment, int type)
{
    if (name.simplified() == "") {
        return false;
    }

    QUuid uuid = generateNewUuid();
    return addGroup(uuid, name, comment, type);
}

bool TermGroupTable::addGroup(QUuid uuid, QString name, QString comment, int type)
{
    if (uuid.isNull()) {
        return false;
    }

    if (name.simplified() == "") {
        return false;
    }

    if (hasGroupWithName(name)) {
        return false;
    }

    QList<InsertContainer> values;
    values << InsertContainer(TermGroupColumn::longUID, uuid.toString());
    values << InsertContainer(TermGroupColumn::name, name);
    values << InsertContainer(TermGroupColumn::comment, comment);
    values << InsertContainer(TermGroupColumn::type, type);

    return insertInto(values);
}

QList<QUuid> TermGroupTable::getAllGroupsUuid()
{
    QList<QUuid> ret;
    RecList records = toRecList(select(QStringList() << TermGroupColumn::longUID));

    for (QSqlRecord record : records) {
        QUuid uuid(record.value(TermGroupColumn::longUID).toString());
        if (!uuid.isNull()) {
            ret << uuid;
        }
    }

    return ret;
}

void TermGroupTable::deleteGroup(QString name)
{
    WhereCondition where;
    where.equal(TermGroupColumn::name, name);
    deleteWhere(where);
}

bool TermGroupTable::hasGroupWithUuid(const QUuid& uuid)
{
    RecList recs = toRecList(select(QStringList() << TermGroupColumn::longUID,
                                    WhereCondition::uuidEqual(uuid)));
    return !recs.isEmpty();
}

QUuid TermGroupTable::generateNewUuid()
{
    QUuid uuid;
    for (int i = 0; i < 1000; i++) {
        uuid = QUuid::createUuid();
        if (!hasGroupWithUuid(uuid)) {
            break;
        }
    }
    return uuid;
}

QUuid TermGroupTable::getUuid(const QString& groupName)
{
    WhereCondition where;
    where.equal(TermGroupColumn::name, groupName);

    QSqlQuery q = select(QStringList() << TermGroupColumn::longUID, where);
    if (!q.next()) {
        return QUuid();
    }

    return QUuid(q.record().value(TermGroupColumn::longUID).toString());
}

bool TermGroupTable::hasGroupWithName(QString groupName)
{
    return !getUuid(groupName).isNull();
}

int TermGroupTable::getType(QUuid groupUuid)
{
    return getIntField(TermGroupColumn::type, groupUuid);
}

void TermGroupTable::setName(QUuid uuid, QString name)
{
    setField(TermGroupColumn::name, uuid, name);
}

void TermGroupTable::setComment(QUuid uuid, QString comment)
{
    setField(TermGroupColumn::comment, uuid, comment);
}

void TermGroupTable::setType(QUuid uuid, int type)
{
    setField(TermGroupColumn::type, uuid, QString::number(type));
}

QString TermGroupTable::getName(const QUuid& uuid)
{
    return getStringField(TermGroupColumn::name, uuid);
}

QSqlRecord TermGroupTable::getGroup(const QUuid& uuid)
{
    QSqlQuery sel = select(getAllCols(), WhereCondition::uuidEqual(uuid));

    if (!sel.next()) {
        return QSqlRecord();
    }

    return sel.record();
}
