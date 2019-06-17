#ifndef TCOLUMN_H
#define TCOLUMN_H

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
        return name == otherCol.name && type == otherCol.type;
    }
};

#endif // TCOLUMN_H
