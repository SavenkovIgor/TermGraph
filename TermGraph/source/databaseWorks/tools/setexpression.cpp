#include "setexpression.h"

void SetExpression::set(const TColumn& column, const int& value)
{
    set(column, QString::number(value));
}

void SetExpression::set(const TColumn& column, const QString& value)
{
    expression << QString(column) + " = " + QueryTools::vv(value);
}

QString SetExpression::getExpression() const
{
    return expression.join(QueryTools::joinParam);
}
