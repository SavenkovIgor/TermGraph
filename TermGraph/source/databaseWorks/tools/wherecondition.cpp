#include "wherecondition.h"

WhereCondition::WhereCondition(const ConditionType& type)
{
    this->type = type;
}

WhereCondition WhereCondition::columnEqual(const TColumn& column, const QString& value)
{
    WhereCondition where;
    where.equal(column, value);
    return where;
}

WhereCondition WhereCondition::columnEqual(const TColumn& column, const int& value)
{
    WhereCondition where;
    where.equal(column, value);
    return where;
}

void WhereCondition::notEqual(const QString& column, const int& value)
{
    notEqual(column, QString::number(value));
}

void WhereCondition::notEqual(const QString& column, const QString& value)
{
    conditions << concat(column, "!=", QueryTools::vv(value));
}

void WhereCondition::equal(const TColumn& column, const int& value)
{
    equal(column, QString::number(value));
}

void WhereCondition::equal(const TColumn& column, const QString& value)
{
    conditions << concat(column, "=", QueryTools::vv(value));
}

void WhereCondition::isNull(const QString& column)
{
    conditions << column + " IS NULL";
}

void WhereCondition::isNotNull(const QString& column)
{
    conditions << column + " IS NOT NULL";
}

QString WhereCondition::getJoinedConditions() const
{
    switch (type) {
    case ConditionType::And:
        return conditions.join(" AND ");
    case ConditionType::Or:
        return conditions.join(" OR ");
    }

    return conditions.join("");
}
