#ifndef SQLQUERYCONSTRUCTOR_H
#define SQLQUERYCONSTRUCTOR_H

#include <QtCore>

#include "commonqueryfunctions.h"

//typedef QString ColumnName;

struct TableColumnDescription {
    QString name;
    QString type;
};

class InsertContainer {
private:
    QString columnName;
    QString value;

public:
    InsertContainer(const QString& columnName, const QString& value) {
        this->columnName = columnName;
        this->value = value;
    }

    InsertContainer(const QString& columnName, const int& value) {
        this->columnName = columnName;
        this->value = QString::number(value);
    }

    QString getColumnName() const
    {
        return columnName;
    }

    QString getValue() const
    {
        return value;
    }
};

class SetExpression {
private:
    QStringList expression;

public:
    void set(const QString& column, const int& value) {
        set(column, QString::number(value));
    }

    void set(const QString& column, const QString& value) {
        expression << column + " = " + CommonQueryFunctions::vv(value);
    }

    QString getExpression() const {
        return expression.join(CommonQueryFunctions::joinParam);
    }
};

enum ConditionType{
    And,
    Or
};


class WhereCondition {
private:
    inline QString concat(QString str1, QString str2, QString str3) {
        return str1 + " " + str2 + " " + str3;
    }

    QStringList conditions;
    ConditionType type;

public:
    WhereCondition(const ConditionType& type = And) {
        this->type = type;
    }

    static WhereCondition uuidEqual(const QUuid& uuid) {
        WhereCondition where;
        where.equal("longUID", uuid.toString());
        return  where;
    }

    static WhereCondition columnEqual(const QString& column, const QString& value) {
        WhereCondition where;
        where.equal(column, value);
        return where;
    }

    static WhereCondition columnEqual(const QString& column, const int& value) {
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

    void equal(const QString& column, const int& value) {
        equal(column,QString::number(value));
    }

    void equal(const QString& column, const QString& value) {
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
        case And: return conditions.join(" AND ");
        case Or:  return conditions.join(" OR ");
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

    QString createTable(QList<TableColumnDescription> columns);
    QString addColumn(TableColumnDescription column);

    QString selectQuery(const QStringList &columns, const WhereCondition &where, const QString &orderBy = "");
    QString insertQuery(const QList<InsertContainer> &values);
    QString updateQuery(const SetExpression &set, const WhereCondition &where);

    QString deleteWhereQuery(const WhereCondition &where);
    QString deleteByUuidQuery(const QUuid &uuid, const QString &primaryKeyName = "longUID");

    static QStringList vv(QStringList lst);
};

#endif // SQLQUERYCONSTRUCTOR_H
