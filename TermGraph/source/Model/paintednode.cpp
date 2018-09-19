#include "paintednode.h"

bool PaintedNode::someoneHover  = false;
bool PaintedNode::someoneSelect = false;

PaintedNode::PaintedNode(QSqlRecord rec) :
    GraphTerm (rec)
{

}
