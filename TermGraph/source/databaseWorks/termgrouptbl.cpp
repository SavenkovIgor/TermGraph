#include "termgrouptbl.h"

#include "source/databaseWorks/columns/termgroupcolumn.h"

bool TermGroupTable::addGroup(const GroupInfoContainer &info)
{
    QUuid groupUuid = info.uuid;

    if (groupUuid.isNull())
        groupUuid = generateNewUuid();

    if (info.name.simplified().isEmpty())
        return false;

    if (hasGroupWithName(info.name))
        return false;

    InsertContainer::List values;

    values.push_back(InsertContainer(TermGroupColumn::uuid, groupUuid.toString()));
    values.push_back(InsertContainer(TermGroupColumn::name,    info.name));
    values.push_back(InsertContainer(TermGroupColumn::comment, info.comment));

    return insertInto(values);
}

bool TermGroupTable::updateGroup(const GroupInfoContainer &info)
{
    if (info.uuid.isNull())
        return false;

    if (!hasGroupWithUuid(info.uuid))
        return false;

    SetExpression set;

    set.set(TermGroupColumn::name,    info.name);
    set.set(TermGroupColumn::comment, info.comment);

    updateWhere(set, whereUuidEqual(info.uuid));

    return true;
}

UuidList TermGroupTable::getAllUuids()
{
    UuidList ret;
    auto records = toRecVector(select(TermGroupColumn::uuid));

    for (const auto& record : records) {
        QUuid uuid(record.value(TermGroupColumn::uuid).toString());
        if (!uuid.isNull()) {
            ret.push_back(uuid);
        }
    }

    return ret;
}

void TermGroupTable::deleteGroup(const QUuid& uuid)
{
    deleteWhere(whereUuidEqual(uuid));
}

bool TermGroupTable::hasGroupWithUuid(const QUuid& uuid)
{
    return hasAnyRecord(whereUuidEqual(uuid));
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
    QSqlQuery q = select(TermGroupColumn::uuid, where);
    if (!q.next()) {
        return QUuid();
    }

    return QUuid(q.record().value(TermGroupColumn::uuid).toString());
}

RecVector TermGroupTable::getAllUuidsAndNames()
{
    TColumn::List columns;
    columns << TermGroupColumn::uuid;
    columns << TermGroupColumn::name;
    auto sel = select(columns);
    return toRecVector(std::move(sel));
}

bool TermGroupTable::hasGroupWithName(const QString &groupName)
{
    return !getUuid(groupName).isNull();
}

const char* TermGroupTable::tableName() const
{
    return TableName::GROUPS;
}

TColumn TermGroupTable::primaryKey() const
{
    return TermGroupColumn::uuid;
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

GroupInfoContainer TermGroupTable::getGroup(const QUuid& uuid)
{
    GroupInfoContainer info;

    QSqlQuery sel = select(getAllColumns(), whereUuidEqual(uuid));

    if (!sel.next())
        return info;

    auto rec = sel.record();

    info.uuid = QUuid(rec.value(TermGroupColumn::uuid).toString());
    info.name = rec.value(TermGroupColumn::name).toString();
    info.comment = rec.value(TermGroupColumn::comment).toString();

    return info;
}

WhereCondition TermGroupTable::whereUuidEqual(const QUuid& uuid)
{
    return primaryKeyEqual(uuid.toString());
}
