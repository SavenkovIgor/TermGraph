#include "graphicitemterm.h"

GraphicItemTerm::GraphicItemTerm(QSqlRecord rec):
    PaintedTerm(rec),
    GraphicItem()
{
    adjustRectSizeForName();
    setZValue(1);
}

GraphicItemTerm::~GraphicItemTerm() { }

QPointF GraphicItemTerm::getPos() const
{
    return this->pos();
}

QPointF GraphicItemTerm::getScenePos() const
{
    return this->scenePos();
}

void GraphicItemTerm::movePosBy(qreal dx, qreal dy)
{
    moveBy(dx,dy);
}
