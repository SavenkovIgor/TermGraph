#ifndef PAINTEDNODE_H
#define PAINTEDNODE_H

#include "graphterm.h"
#include <QSqlRecord>

class PaintedNode : public GraphTerm
{
public:
    PaintedNode(QSqlRecord rec);
};

#endif // PAINTEDNODE_H
