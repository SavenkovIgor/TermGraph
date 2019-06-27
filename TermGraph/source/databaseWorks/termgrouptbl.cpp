#include "termgrouptbl.h"

#include "./columns/termgroupcolumn.h"

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
    GroupInfoContainer info;

    info.uuid = uuid;
    info.name = name;
    info.comment = comment;
    info.type = static_cast<GroupType>(type);

    return addGroup(info);
}

bool TermGroupTable::addGroup(const GroupInfoContainer &info)
{
    if (info.uuid.isNull())
        return false;

    if (info.name.simplified() == "")
        return false;

    if (hasGroupWithName(info.name))
        return false;

    QList<InsertContainer> values;

    values << InsertContainer(TermGroupColumn::longUID, info.uuid.toString());
    values << InsertContainer(TermGroupColumn::name,    info.name);
    values << InsertContainer(TermGroupColumn::comment, info.comment);
    values << InsertContainer(TermGroupColumn::type,    static_cast<int>(info.type));

    return insertInto(values);
}

UuidList TermGroupTable::getAllUuids()
{
    UuidList ret;
    auto records = toRecVector(select(TermGroupColumn::longUID));

    for (const auto& record : records) {
        QUuid uuid(record.value(TermGroupColumn::longUID).toString());
        if (!uuid.isNull()) {
            ret.push_back(uuid);
        }
    }

    return ret;
}

void TermGroupTable::deleteGroup(const QUuid &uuid)
{
    deleteWhere(WhereCondition::uuidEqual(uuid));
}

bool TermGroupTable::hasGroupWithUuid(const QUuid& uuid)
{
    return hasAnyRecord(WhereCondition::uuidEqual(uuid));
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

QUuid TermGroupTable::getUuid(const QString& groupName) const
{
    auto where = WhereCondition::columnEqual(TermGroupColumn::name, groupName);
    QSqlQuery q = select(TermGroupColumn::longUID, where);
    if (!q.next()) {
        return QUuid();
    }

    return QUuid(q.record().value(TermGroupColumn::longUID).toString());
}

RecVector TermGroupTable::getAllUuidsAndNames()
{
    TColumn::List columns;
    columns << TermGroupColumn::longUID;
    columns << TermGroupColumn::name;
    auto sel = select(columns);
    return toRecVector(std::move(sel));
}

bool TermGroupTable::hasGroupWithName(QString groupName)
{
    return !getUuid(groupName).isNull();
}

void TermGroupTable::initTable()
{
    createTable();
}

TColumn::List TermGroupTable::getAllColumns() const
{
    TColumn::List lst;
    for (auto col : TermGroupColumn::columns) {
        lst << col;
    }
    return lst;
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

QSqlRecord TermGroupTable::getGroup(const QUuid& uuid)
{
    QSqlQuery sel = select(getAllColumns(), WhereCondition::uuidEqual(uuid));

    if (!sel.next()) {
        return QSqlRecord();
    }

    return sel.record();
}

GroupInfoContainer TermGroupTable::getGroupInfoContainer(const QUuid& uuid)
{
    GroupInfoContainer info;

    QSqlQuery sel = select(getAllColumns(), WhereCondition::uuidEqual(uuid));

    if (!sel.next())
        return info;

    auto rec = sel.record();

    info.uuid = QUuid(rec.value(TermGroupColumn::longUID).toString());
    info.name = rec.value(TermGroupColumn::name).toString();
    info.comment = rec.value(TermGroupColumn::comment).toString();
    info.type = static_cast<GroupType>(rec.value(TermGroupColumn::type).toInt());

    return info;
}
