#include "termgrouptbl.h"

#include "source/databaseWorks/columns/termgroupcolumn.h"

bool TermGroupTable::addGroup(const GroupInfoContainer &info)
{
    QUuid groupUuid = info.uuid;

    if (groupUuid.isNull()) {
        groupUuid = generateNewUuid();
    }

    if (info.name.simplified() == "")
        return false;

    if (hasGroupWithName(info.name))
        return false;

    QList<InsertContainer> values;

    values << InsertContainer(TermGroupColumn::longUID, groupUuid.toString());
    values << InsertContainer(TermGroupColumn::name,    info.name);
    values << InsertContainer(TermGroupColumn::comment, info.comment);
    values << InsertContainer(TermGroupColumn::type,    static_cast<int>(info.type));

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
    set.set(TermGroupColumn::type,    static_cast<int>(info.type));

    updateWhere(set, WhereCondition::uuidEqual(info.uuid));

    return true;
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

bool TermGroupTable::hasGroupWithName(const QString &groupName)
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

GroupInfoContainer TermGroupTable::getGroup(const QUuid& uuid)
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
