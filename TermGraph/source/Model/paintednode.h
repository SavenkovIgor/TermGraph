#ifndef PAINTEDNODE_H
#define PAINTEDNODE_H

#include "graphterm.h"
#include <QSqlRecord>
#include <QSizeF>

class PaintedNode : public GraphTerm
{
public:
    static bool someoneHover;
    static bool someoneSelect;

    PaintedNode(QSqlRecord rec);

protected:
    // Hovers
    bool thisHovered = false;
    bool thisSelected = false;

    QSizeF nodeSize = QSizeF(40.0, 10.0);
};

#endif // PAINTEDNODE_H
