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
    InsertContainer(QString columnName, QString value) {
        this->columnName = columnName;
        this->value = value;
    }

    InsertContainer(QString columnName, int value) {
        this->columnName = columnName;
        this->value = QString::number(value);
    }

    QString getColumnName() { return columnName; }
    QString getValue() { return value; }
};

class SetExpression {
private:
    QStringList expression;
public:
    void set(QString column, int value) {
        set(column, QString::number(value));
    }

    void set(QString column,QString value) {
        expression << column + " = " + CommonQueryFunctions::vv(value);
    }

    QString getExpression() {
        return expression.join(CommonQueryFunctions::joinParam());
    }
};

enum ConditionType{
    And,
    Or
};


class WhereConditions {
private:
    QString concat(QString str1, QString str2, QString str3) {
        return str1 + " " + str2 + " " + str3;
    }

    QStringList conditions;
    ConditionType type;

public:
    WhereConditions(ConditionType type = And) {
        this->type = type;
    }
    void notEqual(QString column, int value) {
        notEqual(column,QString::number(value));
    }

    void notEqual(QString column, QString value) {
        conditions << concat(column,"!=",CommonQueryFunctions::vv(value));
    }

    void equal(QString column, int value) {
        equal(column,QString::number(value));
    }

    void equal(QString column, QString value) {
        conditions << concat(column,"=",CommonQueryFunctions::vv(value));
    }

    void isNull(QString column) {
        conditions << column + " IS NULL";
    }

    void isNotNull(QString column) {
        conditions << column + " IS NOT NULL";
    }

    QString getJoinedConditions() {
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

    QString selectQuery(QStringList columns, WhereConditions where, QString orderBy = "");
    QString insertQuery(QList<InsertContainer> values);
    QString updateQuery(SetExpression set, WhereConditions where);

    QString deleteWhereQuery(WhereConditions where);
    QString deleteByUuidQuery(QUuid uuid, QString primaryKeyName = "longUID");

    static QStringList vv(QStringList lst);
};

#endif // SQLQUERYCONSTRUCTOR_H
