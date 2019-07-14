#pragma once

#include <QList>
#include <QString>

struct TColumn
{
    using List = QList<TColumn>;  // TODO: replace with vector

    const char* name;  // Name of column
    const char* type;  // Type description of column

    constexpr TColumn(const char* name, const char* type) : name(name), type(type) { }
    constexpr TColumn(const TColumn& col) : name(col.name), type(col.type) { }

    operator QString() const {
        return QString(name);
    }

    bool operator ==(const TColumn& otherCol) const {
        QString nameSelf(name);
        QString nameOther(otherCol.name);

        QString typeSelf(type);
        QString typeOther(otherCol.type);
        return nameSelf == nameOther && typeSelf == typeOther;
    }
};
