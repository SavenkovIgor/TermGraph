#pragma once

#include <QString>
#include <QStringList>
#include <QUuid>

#include "source/databaseWorks/columns/tcolumn.h"
#include "source/databaseWorks/tools/querytools.h"

enum class ConditionType { And, Or };

// TODO: Make chain calls!
class WhereCondition
{
public:
    WhereCondition(const ConditionType& type = ConditionType::And);

    // Build methods
    static WhereCondition columnEqual(const TColumn& column, const QString& value);
    static WhereCondition columnEqual(const TColumn& column, const int& value);

    void notEqual(const QString& column, const int& value);
    void notEqual(const QString& column, const QString& value);

    void equal(const TColumn& column, const int& value);
    void equal(const TColumn& column, const QString& value);

    void isNull(const QString& column);
    void isNotNull(const QString& column);

    QString getJoinedConditions() const;

private:
    QStringList conditions;
    ConditionType type;

    inline QString concat(const QString& str1, const QString& str2, const QString& str3)
    {
        return str1 + " " + str2 + " " + str3;
    }
};
