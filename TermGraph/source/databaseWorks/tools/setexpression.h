#pragma once

#include <QStringList>

#include "source/databaseWorks/columns/tcolumn.h"
#include "source/databaseWorks/tools/querytools.h"

class SetExpression
{
public:
    void set(const TColumn& column, const int& value);
    void set(const TColumn& column, const QString& value);

    QString getExpression() const;

private:
    QStringList expression;
};
