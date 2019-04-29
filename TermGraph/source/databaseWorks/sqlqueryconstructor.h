#ifndef SQLQUERYCONSTRUCTOR_H
#define SQLQUERYCONSTRUCTOR_H

#include <QtCore>

#include "commonqueryfunctions.h"

struct TColumn
{
    typedef QList<TColumn> List;

    const char* name;  // Name of column
    const char* type;  // Type description of column

    constexpr TColumn(const char* name, const char* type) : name(name), type(type) { }
    constexpr TColumn(const TColumn& col) : name(col.name), type(col.type) { }

    operator QString() const {
        return QString(name);
    }

    bool operator ==(const TColumn& otherCol) const {
        return name == otherCol.name && type == otherCol.type;
    }
};

class InsertContainer
{
public:
    InsertContainer(const TColumn& column, const QString& value) :
        columnName(column.name),
        value(value)
    { }

    InsertContainer(const TColumn& column, const int& value) :
        columnName(column.name),
        value(QString::number(value))
    { }

    QString getColumnName() const { return columnName; }
    QString getValue() const { return value; }

private:
    QString columnName;
    QString value;
};

class SetExpression
{
private:
    QStringList expression;

public:
    void set(const TColumn& column, const int& value) {
        set(column, QString::number(value));
    }

    void set(const TColumn& column, const QString& value) {
        expression << QString(column) + " = " + CommonQueryFunctions::vv(value);
    }

    QString getExpression() const {
        return expression.join(CommonQueryFunctions::joinParam);
    }
};

enum class ConditionType{
    And,
    Or
};


class WhereCondition {  // TODO: Make chain calls!
private:
    inline QString concat(QString str1, QString str2, QString str3) {
        return str1 + " " + str2 + " " + str3;
    }

    QStringList conditions;
    ConditionType type;

public:
    WhereCondition(const ConditionType& type = ConditionType::And) {
        this->type = type;
    }

    static WhereCondition uuidEqual(const QUuid& uuid) {
        WhereCondition where;
        where.equal(TColumn("longUID", ""), uuid.toString());
        return  where;
    }

    static WhereCondition columnEqual(const TColumn& column, const QString& value) {
        WhereCondition where;
        where.equal(column, value);
        return where;
    }

    static WhereCondition columnEqual(const TColumn& column, const int& value) {
        WhereCondition where;
        where.equal(column, value);
        return where;
    }

    void notEqual(const QString& column, const int& value) {
        notEqual(column, QString::number(value));
    }

    void notEqual(const QString& column, const QString& value) {
        conditions << concat(column, "!=", CommonQueryFunctions::vv(value));
    }

    void equal(const TColumn& column, const int& value) {
        equal(column, QString::number(value));
    }

    void equal(const TColumn& column, const QString& value) {
        conditions << concat(column, "=", CommonQueryFunctions::vv(value));
    }

    void isNull(const QString& column) {
        conditions << column + " IS NULL";
    }

    void isNotNull(const QString& column) {
        conditions << column + " IS NOT NULL";
    }

    QString getJoinedConditions() const {
        switch (type) {
        case ConditionType::And: return conditions.join(" AND ");
        case ConditionType::Or:  return conditions.join(" OR ");
        }

        return conditions.join("");
    }

};

/// Задача этого класса - формировать запросы
class SqlQueryConstructor
{
private:
    QString tableName;
public:
    SqlQueryConstructor(QString tableName);

    QString createTable(TColumn::List columns);
    QString addColumn(TColumn column);

    QString selectQuery(const QStringList& columns, const WhereCondition& where, const QString &orderBy = "");
    QString selectOneQuery(const WhereCondition& where);
    QString insertQuery(const QList<InsertContainer> &values);
    QString updateQuery(const SetExpression &set, const WhereCondition &where);

    QString deleteWhereQuery(const WhereCondition &where);
    QString deleteByUuidQuery(const QUuid &uuid, const QString &primaryKeyName = "longUID");

    static QStringList vv(QStringList lst);
};

#endif // SQLQUERYCONSTRUCTOR_H
